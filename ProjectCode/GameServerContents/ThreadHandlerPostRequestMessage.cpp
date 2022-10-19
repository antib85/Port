#include "PreCompile.h"
#include "ThreadHandlerPostRequestMessage.h"
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

ThreadHandlerPostRequestMessage::ThreadHandlerPostRequestMessage()
{

}

ThreadHandlerPostRequestMessage::~ThreadHandlerPostRequestMessage()
{

}

void ThreadHandlerPostRequestMessage::Start()
{
	if (nullptr == Session_)
	{
		GameServerDebug::LogError("Login TCPSession Error");
	}

	DBWork(&ThreadHandlerPostRequestMessage::DBCheck);
}

void ThreadHandlerPostRequestMessage::DBCheck()
{
	std::shared_ptr<ContentsUserData> Ptr = Session_->GetLink<ContentsUserData>(EDataIndex::USERDATA);

	CharacterTable_SelectNickName SelectQuery = CharacterTable_SelectNickName(Message_->NickName);
	if (false != SelectQuery.DoQuery())
	{
		PostTable_SelectIDToPostInfo SelectPost = PostTable_SelectIDToPostInfo(Message_->NickName);
		if (false != SelectPost.DoQuery())
		{
			for (int i = 0; i < SelectPost.RowDatas.size(); i++)
				Result_.Posts.push_back(SelectPost.RowDatas[i]->PostData);
		}

	}

	NetWork(&ThreadHandlerPostRequestMessage::ResultSend);
}

void ThreadHandlerPostRequestMessage::ResultSend()
{
	GameServerSerializer Sr;
	Result_.Serialize(Sr);
	Session_->Send(Sr.GetData());

}

