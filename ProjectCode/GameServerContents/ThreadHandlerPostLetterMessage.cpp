#include "PreCompile.h"
#include "ThreadHandlerPostLetterMessage.h"
#include "ContentsSystemEnum.h"
#include <GameServerBase/GameServerDebug.h>
#include <GameServerBase/GameServerString.h>
#include <GameServerNet\DBConnecter.h>
#include <GameServerNet\RedisConnecter.h>
#include "UserTable.h"
#include "CharacterTable.h"
#include "PostTable.h"
#include <GameServerCore\DBQueue.h>
#include <functional>
#include <GameServerCore\NetQueue.h>
#include <GameServerCore\GameServerSectionManager.h>
#include "ContentsUserData.h"
#include <GameServerBase/GameServerTime.h>

ThreadHandlerPostLetterMessage::ThreadHandlerPostLetterMessage()
{

}

ThreadHandlerPostLetterMessage::~ThreadHandlerPostLetterMessage()
{

}

void ThreadHandlerPostLetterMessage::Start()
{
	if (nullptr == Session_)
	{
		GameServerDebug::LogError("Login TCPSession Error");
	}

	DBWork(&ThreadHandlerPostLetterMessage::DBCheck);
}

void ThreadHandlerPostLetterMessage::DBCheck()
{
	std::shared_ptr<ContentsUserData> Ptr = Session_->GetLink<ContentsUserData>(EDataIndex::USERDATA);

	PostTable_SelectIDToUserInfo ToNickQuery = PostTable_SelectIDToUserInfo(Message_->ToNickName);
	PostTable_SelectIDToUserInfo FromNickQuery = PostTable_SelectIDToUserInfo(Message_->FromNickName);
	CharacterTable_SelectNickName FindToNick = CharacterTable_SelectNickName(Message_->ToNickName);
	CharacterTable_SelectNickName FindFromNick = CharacterTable_SelectNickName(Message_->FromNickName);

	if (false != FindToNick.DoQuery() && false != FindFromNick.DoQuery())
	{
		if (Message_->ToNickName == Message_->FromNickName)
			return;

		GameServerTime Times;

		std::string Time = Times.GetNowTime();

		PostTable_InsertUserInfo Insert = PostTable_InsertUserInfo(Message_->ToNickName, Message_->Letters, Message_->FromNickName, Time);
	
		if (false != Insert.DoQuery())
		{
		
		}
	}
	
}

void ThreadHandlerPostLetterMessage::ResultSend()
{

}

