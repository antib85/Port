#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerToClient.h"
#include "ContentsUserData.h"
#include <memory>

class UserRow;
class ThreadHandlerLoginMessage final
	: public ThreadHandlerBase<LoginMessage>
{
private: 
	LoginResultMessage LoginResult_;
	CharacterListMessage Characters;
	std::shared_ptr<ContentsUserData> UserData;

public: 
	ThreadHandlerLoginMessage();
	~ThreadHandlerLoginMessage();
	ThreadHandlerLoginMessage(const ThreadHandlerLoginMessage& _Other) = delete;
	ThreadHandlerLoginMessage(ThreadHandlerLoginMessage&& _Other) noexcept = delete;

	void Start() override;

public:
	std::shared_ptr<UserRow> RowData;

	int UserIndex_;

private:
	void DBCheck();

	void ResultSend();

	void DBCharacterListCheck();

	void CharctersSend();

protected:
	ThreadHandlerLoginMessage& operator=(const ThreadHandlerLoginMessage& _Other) = delete;
	ThreadHandlerLoginMessage& operator=(ThreadHandlerLoginMessage&& _Other) = delete;

};

