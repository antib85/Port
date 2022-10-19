#include "PreCompile.h"
#include "ThreadHandlerLoginMessage.h"
#include "ContentsSystemEnum.h"
#include <GameServerBase/GameServerDebug.h>
#include <GameServerBase/GameServerString.h>
#include <GameServerNet\DBConnecter.h>
#include "UserTable.h"
#include "CharacterTable.h"
#include <GameServerCore\DBQueue.h>
#include <functional>
#include <GameServerCore\NetQueue.h>
#include <GameServerCore\GameServerSectionManager.h>
#include "ContentsUserData.h"

ThreadHandlerLoginMessage::ThreadHandlerLoginMessage()
{

}

ThreadHandlerLoginMessage::~ThreadHandlerLoginMessage()
{

}

void ThreadHandlerLoginMessage::Start()
{
	if (nullptr == Session_)
	{
		GameServerDebug::LogError("Login TCPSession Error");
	}

	LoginResult_.Code = EGameServerCode::LoginError;
	DBWork(&ThreadHandlerLoginMessage::DBCheck);
}

void ThreadHandlerLoginMessage::DBCheck()
{
	std::string Name = GameServerThread::GetThreadName();

	UserTable_SelectIDToUserInfo SelectQuery(Message_->ID);
	SelectQuery.DoQuery();

	if (nullptr == SelectQuery.RowData)
	{
		UserTable_InsertUserInfo Insert = UserTable_InsertUserInfo(Message_->ID, Message_->PW);
		if (true == Insert.DoQuery())
		{
			UserTable_SelectIDToUserInfo InsertPostSelectQuery(Message_->ID);
			InsertPostSelectQuery.DoQuery();
			SelectQuery.RowData = InsertPostSelectQuery.RowData;
		}
	}

	LoginResult_.Code = EGameServerCode::OK;
	RowData = SelectQuery.RowData;

	NetWork(&ThreadHandlerLoginMessage::ResultSend);
}

void ThreadHandlerLoginMessage::ResultSend()
{
	if (LoginResult_.Code == EGameServerCode::OK)
	{
		UserData = std::make_shared<ContentsUserData>();
		UserData->Data = *RowData;
		Session_->SetLink(EDataIndex::USERDATA, UserData);
		UserIndex_ = UserData->Data.Index;
	}

	GameServerSerializer Sr;
	LoginResult_.Serialize(Sr);
	Session_->Send(Sr.GetData());

	GameServerDebug::LogInfo("Login Result Send");

	if (LoginResult_.Code == EGameServerCode::OK)
	{
		DBWork(&ThreadHandlerLoginMessage::DBCharacterListCheck);
	}
}


void ThreadHandlerLoginMessage::DBCharacterListCheck()
{
	UserData = Session_->GetLink<ContentsUserData>(EDataIndex::USERDATA);

	CharacterTable_SelectUserCharacters SelectQuery(UserIndex_);
	SelectQuery.DoQuery();
	
	Characters.Characters.resize(SelectQuery.RowDatas.size());

	for (size_t i = 0; i < SelectQuery.RowDatas.size(); i++)
	{
		Characters.Characters[i] = SelectQuery.RowDatas[i]->Info;
	}

	UserData->Characters = Characters.Characters;

	NetWork(&ThreadHandlerLoginMessage::CharctersSend);


}

void ThreadHandlerLoginMessage::CharctersSend()
{
	GameServerSerializer Sr;
	Characters.Serialize(Sr);
	Session_->Send(Sr.GetData());

	if (LoginResult_.Code == EGameServerCode::OK)
	{
		std::shared_ptr<ContentsUserData> Ptr = Session_->GetLink<ContentsUserData>(EDataIndex::USERDATA);
		Ptr->Data = *RowData;
	}

	GameServerDebug::LogInfo("Charcter List Send");
}