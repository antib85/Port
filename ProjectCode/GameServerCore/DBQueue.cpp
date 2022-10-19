#include "PreCompile.h"
#include "DBQueue.h"
#include <GameServerNet\DBConnecter.h>
#include "GameServerCore.h"
#include <GameServerNet\RedisConnecter.h>

DBQueue* DBQueue::Inst_ = new DBQueue();

DBQueue::DBQueue() 
{

}

DBQueue::~DBQueue() 
{
	JobQueue.Destroy();
}

std::mutex ConnectionRock;

void InitDBConnecter()
{
	ConnectionRock.lock();

	DBConnecter::InitConntor(GameServerCore::GetDBHost(),
		GameServerCore::GetDBUser(),
		GameServerCore::GetDBPW(),
		GameServerCore::GetDBName(),
		GameServerCore::GetDBPort());

	RedisConnecter::InitConntor("127.0.0.1", 6379);

	ConnectionRock.unlock();
}

void DBQueue::Init() 
{
	Inst_->JobQueue.Initialize(GameServerQueue::WORK_TYPE::Default, 20, "DBThread", InitDBConnecter);
}

void DBQueue::Queue(const std::function<void()>& CallBack)
{
	Inst_->JobQueue.EnQueue(CallBack);
}

void DBQueue::Destroy() 
{
	if (nullptr != Inst_)
	{
		delete Inst_;
	}
}