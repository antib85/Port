#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerToClient.h"

class ThreadHandlerCreateCharacterMessage final
	: public ThreadHandlerBase<CreateCharacterMessage>
{
	CharacterCreateResultMessage Result_;

public:
	ThreadHandlerCreateCharacterMessage();
	~ThreadHandlerCreateCharacterMessage();

	ThreadHandlerCreateCharacterMessage(const ThreadHandlerCreateCharacterMessage& _Other) = delete;
	ThreadHandlerCreateCharacterMessage(ThreadHandlerCreateCharacterMessage&& _Other) noexcept = delete;
	ThreadHandlerCreateCharacterMessage& operator=(const ThreadHandlerCreateCharacterMessage& _Other) = delete;
	ThreadHandlerCreateCharacterMessage& operator=(ThreadHandlerCreateCharacterMessage&& _Other) noexcept = delete;

	void Start() override;

	void DBCheck();

	void Result();

protected:

private:

};

