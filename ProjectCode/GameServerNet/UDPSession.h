#pragma once
#include <GameServerBase/GameServerObjectBase.h>
#include <GameServerBase/GameServerObjectPool.h>
#include "IPEndPoint.h"

class UDPSession : public GameServerObjectBase
{
private:
	friend class UDPRecvOverlapped;
	friend class UDPSendOverlapped;

public:
	UDPSession();
	~UDPSession();

	bool Initialize(const IPEndPoint& _Endpoint, std::function<void(std::shared_ptr<UDPSession>, const std::vector<unsigned char>&, IPEndPoint&)> _CallBack);

	bool Recv();
	bool Send(const std::vector<unsigned char>& sendBytes, const IPEndPoint& _remoteEndPoint);

	bool BindQueue(const GameServerQueue& _jobQueue);

private:
	void OnSendComplate(UDPSendOverlapped* _SendOverlapped);

	bool OnRecv(const char* _buffer, DWORD _numberofBytes);

	static void OnCallBack(std::weak_ptr<UDPSession> _weakSession, BOOL _Result, DWORD numberrOfBytes, LPOVERLAPPED _lpOverlapped);

	IPEndPoint LocalEndpoint_;

	SOCKADDR_IN remoteAddr_;

	SOCKET SessionSocket_;

	DWORD flag_;
	int AddrSize_;

	UDPRecvOverlapped* RecvOveralpped;
	GameServerObjectPool<UDPSendOverlapped> SendPool_;

	std::function<void(std::shared_ptr<UDPSession>, const std::vector<unsigned char>&, IPEndPoint&)> RecvCallBack_;
};

