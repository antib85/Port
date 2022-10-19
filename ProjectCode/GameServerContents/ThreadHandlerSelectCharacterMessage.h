#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerToClient.h"

class ThreadHandlerSelectCharacterMessage final
	: public ThreadHandlerBase<SelectCharacterMessage>
{
	CharacterSelectResultMessage ResultMessage;

public:
	ThreadHandlerSelectCharacterMessage();
	~ThreadHandlerSelectCharacterMessage();

	ThreadHandlerSelectCharacterMessage(const ThreadHandlerSelectCharacterMessage& _Other) = delete;
	ThreadHandlerSelectCharacterMessage(ThreadHandlerSelectCharacterMessage&& _Other) noexcept = delete;
	ThreadHandlerSelectCharacterMessage& operator=(const ThreadHandlerSelectCharacterMessage& _Other) = delete;
	ThreadHandlerSelectCharacterMessage& operator=(ThreadHandlerSelectCharacterMessage&& _Other) noexcept = delete;

	void Start() override;

	void DBCheck();

	void SelectResult();

	void SectionInsert();

protected:

private:

};

