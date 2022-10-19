#include "PreCompile.h"
#include "UDPSession.h"
#include "TypeDefine.h"
#include "GameServerOverlapped.h"
#include "TCPListener.h"
#include "SocketAddress.h"

UDPSession::UDPSession()
	: flag_(0)
{
	memset(&remoteAddr_, 0x00, sizeof(remoteAddr_));
	AddrSize_ = static_cast<int>(sizeof(remoteAddr_));
}

UDPSession::~UDPSession()
{
}

bool UDPSession::BindQueue(const GameServerQueue& _jobQueue)
{
	std::weak_ptr<UDPSession> weakSession = DynamicCast<UDPSession>();
	return _jobQueue.NetWorkBind(SessionSocket_, std::bind(&UDPSession::OnCallBack, weakSession, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

bool UDPSession::Initialize(const IPEndPoint& _Endpoint, std::function<void(std::shared_ptr<UDPSession>, const std::vector<unsigned char>&, IPEndPoint&)> _CallBack)
{
	RecvCallBack_ = _CallBack;

	LocalEndpoint_ = _Endpoint;

	RecvOveralpped = new UDPRecvOverlapped(DynamicCast<UDPSession>());

	SessionSocket_ = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO::IPPROTO_UDP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == SessionSocket_)
	{
		GameServerDebug::GetLastErrorPrint();
		return false;
	}

	SocketAddress socketAddress = _Endpoint.Serialize();

	int Error = bind(SessionSocket_, reinterpret_cast<const sockaddr*>(socketAddress.GetBuffer()), static_cast<int>(socketAddress.GetBufferLength()));

	if (Error == SOCKET_ERROR)
	{
		GameServerDebug::GetLastErrorPrint();
		return false;
	}

	return true;
}

void UDPSession::OnCallBack(std::weak_ptr<UDPSession> _weakSession, BOOL _Result, DWORD numberrOfBytes, LPOVERLAPPED _lpOverlapped)
{
	if (nullptr == _lpOverlapped)
	{
		return;
	}

	GameServerOverlapped* GameServerOver = reinterpret_cast<GameServerOverlapped*>(reinterpret_cast<char*>(_lpOverlapped) - sizeof(void*));
	GameServerOver->Execute(_Result, numberrOfBytes);
}

bool UDPSession::Recv() 
{
	if (INVALID_SOCKET == SessionSocket_)
	{
		return false;
	}

	int Result = WSARecvFrom(SessionSocket_, RecvOveralpped->GetWSABuffer(), 1, RecvOveralpped->GetBufferLength()
		, &flag_, reinterpret_cast<sockaddr*>(&remoteAddr_), &AddrSize_, RecvOveralpped->GetOverlapped(), nullptr);

	if (SOCKET_ERROR == Result)
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			GameServerDebug::GetLastErrorPrint();
			return false;
		}
	}

	return true;
}

bool UDPSession::Send(const std::vector<unsigned char>& _sendBytes, const IPEndPoint& _remoteEndPoint) 
{
	UDPSendOverlapped* SendOver = SendPool_.Pop();

	SendOver->SetUDPSession(DynamicCast<UDPSession>());

	SendOver->CopyFrom(_sendBytes);

	SocketAddress sockaddress = _remoteEndPoint.Serialize();

	int Result = WSASendTo(SessionSocket_, SendOver->GetWSABuffer(), 1, SendOver->GetBufferLength()
	, 0, reinterpret_cast<const sockaddr*>(sockaddress.GetBuffer()), static_cast<int>(sockaddress.GetBufferLength()), SendOver->GetOverlapped(), nullptr);
	
	if (SOCKET_ERROR == Result)
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			GameServerDebug::GetLastErrorPrint();
			return false;
		}
	}

	return true;
}

void UDPSession::OnSendComplate(UDPSendOverlapped* _SendOverlapped)
{
	SendPool_.Push(_SendOverlapped);
}

bool UDPSession::OnRecv(const char* _buffer, DWORD _numberofBytes)
{
	if (nullptr != RecvCallBack_)
	{
		std::vector<unsigned char> Buffer;

		Buffer.resize(_numberofBytes);

		memcpy_s(&Buffer[0], Buffer.size(), _buffer, _numberofBytes);

		IPEndPoint remoteEndPoint(remoteAddr_.sin_addr.s_addr, htons(remoteAddr_.sin_port));

		RecvCallBack_(DynamicCast<UDPSession>(), Buffer, remoteEndPoint);
	}

	return Recv();
}

