#include "PreCompile.h"
#include "TCPListener.h"
#include "SocketAddress.h"
#include "ServerHelper.h"
#include "TCPSession.h"
#include "GameServerOverlapped.h"

TCPListener::TCPListener() 
	: ListenSocket_(NULL)
	, JobQueue_(nullptr)
	, AcceptCallBack_(nullptr)
{

}

TCPListener::~TCPListener() 
{

	int a = 0;

	connectionPool_;
	connections_;
	AcceptExOverlappedPool_;
	IocpAcceptExOverlappedPool_;
}

TCPListener::TCPListener(TCPListener&& _Other) noexcept
	: ListenSocket_(_Other.ListenSocket_)
{
}



bool TCPListener::Initialize(const IPEndPoint& _EndPoint, const std::function<void(std::shared_ptr<TCPSession>)>& _AcceptCallBack)
{
	ServerHelper::StartEngineStartUp();

	SocketAddress Addresss = _EndPoint.Serialize();

	ListenSocket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == ListenSocket_)
	{
		GameServerDebug::GetLastErrorPrint();
		return false;
	}

	int ErrorCode = ::bind(ListenSocket_, reinterpret_cast<const SOCKADDR*>(Addresss.GetBuffer()), static_cast<int>(Addresss.GetBufferLength()));

	if (SOCKET_ERROR == ErrorCode)
	{
		GameServerDebug::GetLastErrorPrint();
		Close();
		return false;
	}

	BOOL on = TRUE;

	ErrorCode = setsockopt(ListenSocket_, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, reinterpret_cast<const char*>(&on), sizeof(on));
	if (SOCKET_ERROR == ErrorCode)
	{
		GameServerDebug::GetLastErrorPrint();
		Close();
		return false;
	}

	ErrorCode = listen(ListenSocket_, 512);
	if (SOCKET_ERROR == ErrorCode)
	{
		GameServerDebug::GetLastErrorPrint();
		Close();
		return false;
	}

	ListenEndPoint_ = _EndPoint;
	AcceptCallBack_ = _AcceptCallBack;
	return true;
}
void TCPListener::Close()
{
	if (NULL != ListenSocket_)
	{
		closesocket(ListenSocket_);
		ListenSocket_ = NULL;
	}
}

bool TCPListener::BindQueue(GameServerQueue& _JobQueue)
{
	JobQueue_ = &_JobQueue;

	return JobQueue_->NetWorkBind(ListenSocket_, std::bind(&TCPListener::OnAccept, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

}

void TCPListener::OnAccept(BOOL _Result, DWORD _ByteSize, LPOVERLAPPED _Overlapped)
{
	AsyncAccept();

	if (nullptr == _Overlapped)
	{
		return;
	}

	std::unique_ptr<AcceptExOverlapped> AcceptExOverPtr = std::unique_ptr<AcceptExOverlapped>(reinterpret_cast<AcceptExOverlapped*>(reinterpret_cast<char*>(_Overlapped) - sizeof(void*)));


	if (nullptr == AcceptCallBack_)
	{
		return;
	}

	if (_Result != 0)
	{
		AcceptExOverPtr->Execute(TRUE, _ByteSize);

		if (nullptr == AcceptCallBack_)
		{
			return;
		}

		if (false == AcceptExOverPtr->GetTCPSession()->isAcceptBind_)
		{
			AcceptExOverPtr->GetTCPSession()->BindQueue(*JobQueue_);
			AcceptExOverPtr->GetTCPSession()->AcceptBindOn();
		}
		AcceptCallBack_(std::dynamic_pointer_cast<TCPSession>(AcceptExOverPtr->GetTCPSession()->shared_from_this()));


		AcceptExOverPtr->GetTCPSession()->RecvRequest();

		connectsLock_.lock();
		connections_.insert(std::make_pair(AcceptExOverPtr->GetTCPSession()->GetConnectId(), AcceptExOverPtr->GetTCPSession()));
		connectsLock_.unlock();

		IocpAcceptExOverlappedPool_.Erase(AcceptExOverPtr.get());

		AcceptExOverlappedPool_.Push(AcceptExOverPtr.get());
		AcceptExOverPtr.release();
	}
	else 
	{
		GameServerDebug::AssertDebugMsg("Accept Error");
	}

}

bool TCPListener::StartAccept(int _BackLog) 
{
	if (0 > _BackLog || 128 < _BackLog )
	{
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);

		_BackLog = systemInfo.dwNumberOfProcessors;
	}

	for (size_t i = 0; i < _BackLog; i++)
	{
		AsyncAccept();
	}

	return true;

}

void TCPListener::AsyncAccept()
{
	PtrSTCPSession NewSession = nullptr;
	{
		connectPoolLock_.lock();
		
		if (true == connectionPool_.empty())
		{
			NewSession = std::make_unique<TCPSession>();
			NewSession->Initialize();

			std::string LogText = std::to_string(static_cast<int>(NewSession->GetSocket()));
			GameServerDebug::LogInfo(LogText + " Socket Create");

			NewSession->SetParent(this);
		}
		else 
		{
			NewSession = connectionPool_.front();
			std::string LogText = std::to_string(static_cast<int>(NewSession->GetSocket()));
			GameServerDebug::LogInfo(LogText + " Socket Reuse");
			connectionPool_.pop_front();
		}

		connectPoolLock_.unlock();
	}

	AcceptExOverlapped* AcceptOver = nullptr;
	{
		if (true == AcceptExOverlappedPool_.IsEmpty())
		{
			AcceptOver = new AcceptExOverlapped(NewSession);
		}
		else
		{
			AcceptOver = AcceptExOverlappedPool_.Pop();
			AcceptOver->SetTCPSession(NewSession);
		}
	}
	

	DWORD dwByte = 0;

	BOOL Result = AcceptEx(ListenSocket_, NewSession->GetSocket(), AcceptOver->GetBuffer(), 0, sizeof(sockaddr_in) +16, sizeof(sockaddr_in) + 16, &dwByte, AcceptOver->GetOverlapped());
	IocpAcceptExOverlappedPool_.Push(AcceptOver);

	if (FALSE == Result)
	{
		if (ERROR_IO_PENDING != WSAGetLastError())
		{
			GameServerDebug::GetLastErrorPrint();
			return;
		}
	}
}


void TCPListener::CloseSession(PtrSTCPSession _Session)
{
	{
		std::lock_guard<std::mutex> lock(connectsLock_);
		connections_.erase(_Session->GetConnectId());
	}

	{
		std::lock_guard<std::mutex> lock(connectPoolLock_);

		connectionPool_.push_back(_Session);
	}
}

void TCPListener::BroadCast(std::vector<unsigned char> _Data, std::shared_ptr<TCPSession> _Ignore)
{
	connectsLock_.lock();
	for (auto& Session : connections_)
	{
		if (_Ignore == Session.second)
		{
			continue;
		}

		Session.second->Send(_Data);
	}
	connectsLock_.unlock();
}