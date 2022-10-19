#pragma once
#include "IPEndPoint.h"
#include "TypeDefine.h"
#include <deque>
#include <GameServerBase\GameServerObjectPool.h>
#include <GameServerBase\GameServerObjectBase.h>

class TCPSession;
class GameServerQueue;
class AcceptExOverlapped;
class TCPListener : public GameServerObjectBase
{
	friend TCPSession;


public: 
	TCPListener();
	~TCPListener();

	TCPListener(const TCPListener& _Other) = delete;
	TCPListener(TCPListener&& _Other) noexcept;

	bool Initialize(const IPEndPoint& _EndPoint, const std::function<void(std::shared_ptr<TCPSession>)>& CallBack_);

	bool BindQueue(GameServerQueue& _JobQueue);

	bool StartAccept(int _BackLog);

	void Close();

	void BroadCast(std::vector<unsigned char> _Data, std::shared_ptr<TCPSession> _Ignore);

protected:
	TCPListener& operator=(const TCPListener& _Other) = delete;
	TCPListener& operator=(TCPListener&& _Other) = delete;

	void AsyncAccept();

private: 
	SOCKET ListenSocket_;

	IPEndPoint ListenEndPoint_;

	GameServerQueue* JobQueue_;

	std::function<void(std::shared_ptr<TCPSession>)> AcceptCallBack_;

	GameServerObjectFindPool<AcceptExOverlapped> IocpAcceptExOverlappedPool_;
	GameServerObjectPool<AcceptExOverlapped> AcceptExOverlappedPool_;

	std::mutex connectPoolLock_;
	std::deque<std::shared_ptr<TCPSession>> connectionPool_;

	std::mutex connectsLock_;
	std::unordered_map<__int64, std::shared_ptr<TCPSession>> connections_;

	void CloseSession(PtrSTCPSession _Session);

	void OnAccept(BOOL, DWORD, LPOVERLAPPED);
};

