#pragma once
#include <GameServerBase/GameServerObjectBase.h>
#include <GameServerBase/GameServerObjectPool.h>
#include "IPEndPoint.h"

enum class SessionMode
{
	Sync,
	ASync,
};

class TCPListener;
class SendOverlapped;
class RecvOverlapped;
class GameServerQueue;
class AcceptExOverlapped;
class DisconnectOverlapped;
class TCPSession : public GameServerObjectBase
{
	friend TCPListener;
	friend AcceptExOverlapped;
	friend RecvOverlapped;
	friend SendOverlapped;

private: 
	bool isAcceptBind_;
	__int64 ConnectId_;
	SOCKET SessionSocket_;
	IPEndPoint LocalAddress_;
	IPEndPoint RemoteAddress_;
	SessionMode SyncMode;

	RecvOverlapped* recvOverlapped_;
	DisconnectOverlapped* disconnectOverlapped_;
	GameServerObjectPool<SendOverlapped> SendPool_;

	std::atomic_bool CallClose_;

	std::function<void(std::shared_ptr<TCPSession>, const std::vector<unsigned char>&)> RecvCallBack_;
	std::function<void(std::shared_ptr<TCPSession>)> CloseCallBack_;

public: 
	TCPSession();
	~TCPSession();

	TCPSession(const TCPSession& _Other) = delete;
	TCPSession(TCPSession&& _Other) noexcept;

	SOCKET GetSocket();

	__int64 GetConnectId();

	void UnregisterSession();

	bool Send(const std::vector<unsigned char>& _Buffer);

	bool SendSync(const std::vector<unsigned char>& _Buffer);

	bool SendSync(const char* _Data, size_t _Len);

	bool RecvSync(std::vector<unsigned char>& _Buffer);

	bool RecvSync(char* _Buffer, size_t Len);

	void SetCallBack(std::function<void(std::shared_ptr<TCPSession>, const std::vector<unsigned char>&)> _CallBack, std::function<void(std::shared_ptr<TCPSession>)> _CloseCallBack);

	void SetCloseCallBack(std::function<void(std::shared_ptr<TCPSession>)> _CallBack);
	void SetRecvCallBack(std::function<void(std::shared_ptr<TCPSession>, const std::vector<unsigned char>&)> _CallBack);

	bool ConnectSync(std::string _Ip, unsigned int _Port);

	bool Initialize();

	void Close(bool _forceClsoe = false);

protected:
	TCPSession& operator=(const TCPSession& _Other) = delete;
	TCPSession& operator=(TCPSession&& _Other) = delete;

private:
	static void OnCallBack(std::weak_ptr<TCPSession> _weakSession, BOOL _Result, DWORD numberrOfBytes, LPOVERLAPPED _lpOverlapped);
	void CloseSocket();
	void OnRecv(const char*, DWORD);

	void OnSendComplete(SendOverlapped* _Send);

	void RecvRequest();
	bool BindQueue(const GameServerQueue& _jobQueue);
	void DisconnectSocket();
	void AcceptBindOn();

};

