#include "PreCompile.h"
#include "ThreadHandlerRankRequestMessage.h"
#include "ContentsSystemEnum.h"
#include <GameServerBase/GameServerDebug.h>
#include <GameServerBase/GameServerString.h>
#include <GameServerNet\DBConnecter.h>
#include <GameServerNet\RedisConnecter.h>
#include "UserTable.h"
#include "CharacterTable.h"
#include <GameServerCore\DBQueue.h>
#include <functional>
#include <GameServerCore\NetQueue.h>
#include <GameServerCore\GameServerSectionManager.h>
#include "ContentsUserData.h"

ThreadHandlerRankRequestMessage::ThreadHandlerRankRequestMessage()
{

}

ThreadHandlerRankRequestMessage::~ThreadHandlerRankRequestMessage()
{

}

void ThreadHandlerRankRequestMessage::Start()
{
	if (nullptr == Session_)
	{
		GameServerDebug::LogError("Login TCPSession Error");
	}

	DBWork(&ThreadHandlerRankRequestMessage::DBCheck);
}

void ThreadHandlerRankRequestMessage::DBCheck()
{
	std::shared_ptr<RedisConnecter> Connecter = RedisConnecter::GetConnector();

	RedisCommend::ZRANGE TopRange = RedisCommend::ZRANGE("UserRank", 0, 4, ZRANGEOPTION::GREATER, ZRANGESCOREOPTION::WITHSCORES);
	TopRange.Process(*Connecter);

	std::string MyNick = GameServerString::UTF8ToAnsi(Message_->NickName);

	RedisCommend::ZRANK MyRank = RedisCommend::ZRANK("UserRank", MyNick, ZRANGEOPTION::GREATER);
	MyRank.Process(*Connecter);

	if (MyRank.Rank == -1)
	{
		NetWork(&ThreadHandlerRankRequestMessage::ResultSend);
		return;
	}

	RedisCommend::ZRANGE MyRange = RedisCommend::ZRANGE("UserRank", MyRank.Rank - 2, MyRank.Rank + 2, ZRANGEOPTION::GREATER, ZRANGESCOREOPTION::WITHSCORES);
	MyRange.Process(*Connecter);

	Result.TopUserName = TopRange.Members;
	Result.TopUserScore = TopRange.Scores;

	Result.MyUserName = MyRange.Members;
	Result.MyUserScore = MyRange.Scores;


	NetWork(&ThreadHandlerRankRequestMessage::ResultSend);
}

void ThreadHandlerRankRequestMessage::ResultSend()
{
	GameServerSerializer Sr;
	Result.Serialize(Sr);
	Session_->Send(Sr.GetData());

}

