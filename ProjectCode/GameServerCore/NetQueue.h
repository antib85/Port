#pragma once
#include <GameServerBase/GameServerQueue.h>

class NetQueue
{
private: 

public: 
	NetQueue();
	~NetQueue();

	NetQueue(const NetQueue& _Other) = delete;
	NetQueue(NetQueue&& _Other) noexcept = delete;

protected:
	NetQueue& operator=(const NetQueue& _Other) = delete;
	NetQueue& operator=(NetQueue&& _Other) = delete;


private: 
	static GameServerQueue JobQueue;

public:
	static GameServerQueue& GetQueue() 
	{
		return JobQueue;
	}

	static void Destroy();

public:
	static void Init();
	static void Queue(const std::function<void()>& CallBack);
};

