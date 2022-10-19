#include "PreCompile.h"
#include "TCPSession.h"
#include "TypeDefine.h"
#include "GameServerOverlapped.h"
#include "TCPListener.h"
#include "ServerHelper.h"

TCPSession::TCPSession() 
	: ConnectId_(0),
	SessionSocket_(INVALID_SOCKET),
	CallClose_(false),
	RecvCallBack_(nullptr),
	recvOverlapped_(nullptr),
	disconnectOverlapped_(nullptr),
	isAcceptBind_(false),
	SyncMode(SessionMode::ASync)
{
}

TCPSession::~TCPSession() 
{
	if (nullptr != recvOverlapped_)
	{
		delete recvOverlapped_;
	}

	if (nullptr != disconnectOverlapped_)
	{
		delete disconnectOverlapped_;
	}

	SendPool_;
}

TCPSession::TCPSession(TCPSession&& _Other) noexcept
{

}


void TCPSession::AcceptBindOn() 
{
	isAcceptBind_ = true;
}

__int64 TCPSession::GetConnectId() 
{
	return ConnectId_;
}

SOCKET TCPSession::GetSocket() 
{
	return SessionSocket_;
}

bool TCPSession::Initialize() 
{
	SessionSocket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == SessionSocket_)
	{
		GameServerDebug::GetLastErrorPrint();
		return false;
	}

	int ErrorCode = 0;

	{
		BOOL bNodelayOption = TRUE;
		ErrorCode = setsockopt(SessionSocket_, IPPROTO::IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&bNodelayOption), sizeof(bNodelayOption));
		if (SOCKET_ERROR == ErrorCode)
		{
			GameServerDebug::GetLastErrorPrint();
			Close();
			return false;
		}
	}

	{
		BOOL bResueOption = TRUE;
		ErrorCode = setsockopt(SessionSocket_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&bResueOption), sizeof(bResueOption));
		if (SOCKET_ERROR == ErrorCode)
		{
			GameServerDebug::GetLastErrorPrint();
			Close();
			return false;
		}
	}

	{
		LINGER lin;

		lin.l_onoff = 1;

		lin.l_linger = 0;

		ErrorCode = setsockopt(SessionSocket_, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&lin), sizeof(lin));
		if (SOCKET_ERROR == ErrorCode)
		{
			GameServerDebug::GetLastErrorPrint();
			Close();
			return false;
		}
	}

	ConnectId_ = GameServerUnique::GetNextUniqueId();

	if (nullptr == recvOverlapped_)
	{
		recvOverlapped_ = new RecvOverlapped(this);
	}

	if (nullptr == disconnectOverlapped_)
	{
		disconnectOverlapped_ = new DisconnectOverlapped(this);
	}

	SyncMode = SessionMode::ASync;

	return true;
}

bool TCPSession::ConnectSync(std::string _Ip, unsigned int _Port)
{
	ServerHelper::StartEngineStartUp();

	SessionSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == SessionSocket_)
	{
		return false;
	}
	SOCKADDR_IN Add = { 0, };
	Add.sin_family = AF_INET;
	Add.sin_port = htons(_Port);
	if (SOCKET_ERROR == inet_pton(AF_INET, _Ip.c_str(), &Add.sin_addr))
	{
		return false;
	}
	int Len = sizeof(SOCKADDR_IN);

	if (SOCKET_ERROR == connect(SessionSocket_, (const sockaddr*)&Add, Len))
	{
		GameServerDebug::AssertDebugMsg("커넥트에 실패했습니다.");
		return 0;
	}

	SyncMode = SessionMode::Sync;

	return true;
}

void TCPSession::Close(bool _forceClose) 
{
	if (nullptr != CloseCallBack_)
	{
		CloseCallBack_(std::dynamic_pointer_cast<TCPSession>(shared_from_this()));
	}

	CallClose_ = true;

	if (SyncMode == SessionMode::ASync && false == _forceClose)
	{
		DisconnectSocket();
	}
	else 
	{
		CloseSocket();
	}
}

void TCPSession::OnCallBack(std::weak_ptr<TCPSession> _weakSession, BOOL _Result, DWORD numberrOfBytes, LPOVERLAPPED _lpOverlapped)
{
	if (nullptr == _lpOverlapped)
	{
		return;
	}

	GameServerOverlapped* GameServerOver = reinterpret_cast<GameServerOverlapped*>(reinterpret_cast<char*>(_lpOverlapped) - sizeof(void*));
	GameServerOver->Execute(_Result, numberrOfBytes);
}

bool TCPSession::BindQueue(const GameServerQueue& _jobQueue)
{
	std::weak_ptr<TCPSession> Ptr = std::dynamic_pointer_cast<TCPSession>(shared_from_this());

	return _jobQueue.NetWorkBind(SessionSocket_, std::bind(&TCPSession::OnCallBack, Ptr, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void TCPSession::CloseSocket()
{
	if (INVALID_SOCKET != SessionSocket_)
	{
		shutdown(SessionSocket_, SD_BOTH);
		closesocket(SessionSocket_);
		SessionSocket_ = INVALID_SOCKET;
	}
}

void TCPSession::RecvRequest() 
{
	DWORD recvByte = 0;
	DWORD dwFlags = 0;

	recvOverlapped_->Clear();

	if (SOCKET_ERROR == WSARecv(
		SessionSocket_,
		recvOverlapped_->GetWSABuffer(),
		1,
		&recvByte,
		&dwFlags,
		recvOverlapped_->GetOverlapped(),
		nullptr))
	{
		int Error = WSAGetLastError();
		if (WSA_IO_PENDING != Error)
		{
			GameServerDebug::GetLastErrorPrint();
			return;
		}
	}
}

void TCPSession::OnRecv(const char* _Data, DWORD _ByteSize)
{
	std::vector<unsigned char> outBuffer = std::vector<unsigned char>(_ByteSize);
	memcpy_s(&outBuffer[0], _ByteSize, _Data, _ByteSize);

	if (nullptr != RecvCallBack_)
	{
		RecvCallBack_(std::dynamic_pointer_cast<TCPSession>(shared_from_this()), outBuffer);
	}

	if (false == CallClose_)
	{
		RecvRequest();
	}
	
}

void TCPSession::SetCallBack(std::function<void(std::shared_ptr<TCPSession>, const std::vector<unsigned char>&)> _RecvCallBack, std::function<void(std::shared_ptr<TCPSession>)> _CloseCallBack)
{
	if (nullptr == _RecvCallBack)
	{
		GameServerDebug::LogError("RecvCallBack Is Nullptr");
	}

	RecvCallBack_ = _RecvCallBack;
	CloseCallBack_ = _CloseCallBack;
}

void TCPSession::SetCloseCallBack(std::function<void(std::shared_ptr<TCPSession>)> _CallBack)
{
	CloseCallBack_ = _CallBack;
}
void TCPSession::SetRecvCallBack(std::function<void(std::shared_ptr<TCPSession>, const std::vector<unsigned char>&)> _CallBack)
{
	RecvCallBack_ = _CallBack;
}

void TCPSession::UnregisterSession() 
{
	TCPListener* Parent = GetParent<TCPListener>();

	if (nullptr == Parent)
	{
		GameServerDebug::AssertDebugMsg("리스너와 연결되지 않은 세션이 존재합니다.");
	}

	std::string TextLog = std::to_string(static_cast<int>(GetSocket()));

	Parent->CloseSession(std::dynamic_pointer_cast<TCPSession>(shared_from_this()));

	CallClose_ = false;

	ConnectId_ = GameServerUnique::GetNextUniqueId();

	GameServerDebug::LogInfo(TextLog + " 소켓 재활용 처리 완료");
}

void TCPSession::DisconnectSocket()
{
	if (INVALID_SOCKET == SessionSocket_)
	{
		GameServerDebug::AssertDebugMsg("유효하지 않은 소켓을 디스커넥트 하려고 했습니다.");
		return;
	}

	if (nullptr == disconnectOverlapped_)
	{
		GameServerDebug::AssertDebugMsg("디스커넥트 오버랩드가 제대로 만들어지지 않았습니다..");
		return;
	}

	BOOL bResult = TransmitFile(SessionSocket_, 0, 0, 0, disconnectOverlapped_->GetOverlapped(), NULL, TF_DISCONNECT | TF_REUSE_SOCKET);

	if (FALSE == bResult)
	{
		int iError = WSAGetLastError();
		if (ERROR_IO_PENDING != iError)
		{
			GameServerDebug::GetLastErrorPrint();
		}
	}
}

bool TCPSession::Send(const std::vector<unsigned char>& _Buffer)
{
	if (_Buffer.empty())
	{
		return false;
	}

	DWORD ByteSize = 0;
	DWORD flag = 0;

	SendOverlapped* SendOverLapped = SendPool_.Pop();
	SendOverLapped->SetTCPSession(this);
	SendOverLapped->CopyFrom(_Buffer);

	int socketError = WSASend(SessionSocket_, SendOverLapped->GetBuffer(), 1, &ByteSize, flag, SendOverLapped->GetOverlapped(), nullptr);

	if (SOCKET_ERROR == socketError)
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			GameServerDebug::GetLastErrorPrint();
			return false;
		}
	}

	return true;
}

bool TCPSession::SendSync(const std::vector<unsigned char>& _Buffer)
{
	int socketError = send(SessionSocket_, reinterpret_cast<const char*>(& _Buffer[0]), _Buffer.size(), 0);

	if (SOCKET_ERROR == socketError)
	{
		int a = 0;
		return false;
	}

	return true;
}

bool TCPSession::SendSync(const char* _Data, size_t _Len) 
{
	int socketError = send(SessionSocket_, _Data, _Len, 0);

	if (SOCKET_ERROR == socketError)
	{
		int a = 0;
		return false;
	}

	return true;
}

bool TCPSession::RecvSync(std::vector<unsigned char>& _Buffer)
{
	return RecvSync(reinterpret_cast<char*>(&_Buffer[0]), _Buffer.size());
}

bool TCPSession::RecvSync(char* _Buffer, size_t Len)
{
	int socketError = recv(SessionSocket_, _Buffer, Len, 0);

	if (SOCKET_ERROR == socketError)
	{
		return false;
	}

	return true;
}

void TCPSession::OnSendComplete(SendOverlapped* _Send) 
{
	SendPool_.Push(_Send);
}