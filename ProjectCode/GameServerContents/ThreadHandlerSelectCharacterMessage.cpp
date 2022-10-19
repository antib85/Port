#include "PreCompile.h"
#include "ThreadHandlerSelectCharacterMessage.h"
#include "ThreadHandlerLoginMessage.h"
#include <GameServerBase/GameServerDebug.h>
#include <GameServerBase/GameServerString.h>
#include <GameServerNet\DBConnecter.h>
#include "UserTable.h"
#include <GameServerCore\DBQueue.h>
#include <functional>
#include <GameServerCore\NetQueue.h>
#include <GameServerCore\GameServerSectionManager.h>
#include "ContentsSystemEnum.h"
#include "ContentsUserData.h"
#include "ContentsPlayerData.h"
#include "CharacterTable.h"
#include <GameServerCore\GameServerSectionManager.h>
#include "Player.h"
#include "ContentsGlobalValue.h"

ThreadHandlerSelectCharacterMessage::ThreadHandlerSelectCharacterMessage() 
{
}

ThreadHandlerSelectCharacterMessage::~ThreadHandlerSelectCharacterMessage() 
{
}

void ThreadHandlerSelectCharacterMessage::Start() 
{
	if (nullptr == Session_)
	{
		GameServerDebug::LogError("Login TCPSession Error");
	}

	{
	}



	DBWork(&ThreadHandlerSelectCharacterMessage::DBCheck);
}


void ThreadHandlerSelectCharacterMessage::DBCheck() 
{
	CharacterTable_SelectNickName SelectQuery = CharacterTable_SelectNickName(Message_->NickName);

	if (false == SelectQuery.DoQuery())
	{
		ResultMessage.Code = EGameServerCode::FAIL;
		NetWork(&ThreadHandlerSelectCharacterMessage::SelectResult);
		return;
	}

	ResultMessage.Code = EGameServerCode::OK;
	NetWork(&ThreadHandlerSelectCharacterMessage::SelectResult);
}

void ThreadHandlerSelectCharacterMessage::SelectResult()
{
	std::shared_ptr<ContentsUserData> UserData = Session_->GetLink<ContentsUserData>(EDataIndex::USERDATA);

	if (ResultMessage.Code == EGameServerCode::OK)
	{
		for (size_t i = 0; i < UserData->Characters.size(); i++)
		{
			if (UserData->Characters[i].NickName == Message_->NickName)
			{
				GameServerSerializer Sr;
				ResultMessage.NickName = Message_->NickName;
				ResultMessage.Serialize(Sr);
				Session_->Send(Sr.GetData());
				GameServerDebug::LogInfo("Select Result OK Send");
				UserData->SelectData = UserData->Characters[i];
				NetWork(&ThreadHandlerSelectCharacterMessage::SectionInsert);

				return;
			}
		}
	}

	ResultMessage.Code = EGameServerCode::FAIL;
	GameServerSerializer Sr;
	ResultMessage.Serialize(Sr);
	Session_->Send(Sr.GetData());

	GameServerDebug::LogInfo("Select Result FAIL Send");
}

void ThreadHandlerSelectCharacterMessage::SectionInsert()
{
	std::shared_ptr<ContentsUserData> UserData = Session_->GetLink<ContentsUserData>(EDataIndex::USERDATA);

	size_t Key = UserData->SelectData.LastSectionID;

	if (Key == -1)
	{
		Key = static_cast<int>(ESectionType::NONEFIGHT);
	}


	std::shared_ptr<GameServerSection> LastSection = GameServerSectionManager::GetInst()->FindSection(Key);
	std::shared_ptr<Player> NewPlayer = LastSection->CreateActor<Player>(Session_);

	if (nullptr == NewPlayer)
	{
	}

	std::shared_ptr<ContentsPlayerData> NewPlayableData = Session_->CreateLink<ContentsPlayerData>(EDataIndex::PLAYABLE);

	NewPlayableData->ConnectPlayer = NewPlayer.get();
	
}