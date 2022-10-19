#pragma once
#include <GameServerBase/GameServerQueue.h>
class DBQueue
{
private:
	static DBQueue* Inst_;

private:
	DBQueue();
	~DBQueue();

	DBQueue(const DBQueue& _Other) = delete;
	DBQueue(DBQueue&& _Other) noexcept = delete;

protected:
	DBQueue& operator=(const DBQueue& _Other) = delete;
	DBQueue& operator=(DBQueue&& _Other) = delete;


private: 
	GameServerQueue JobQueue;

public:
	static void Destroy();

public:
	static void Init();
	static void Queue(const std::function<void()>& CallBack);
};

