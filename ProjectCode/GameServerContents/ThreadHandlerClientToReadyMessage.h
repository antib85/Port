#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerToClient.h"

class ThreadHandlerClientToReadyMessage final
	: public ThreadHandlerBase<ClientToReadyMessage>
{
	CharacterSelectResultMessage ResultMessage;

public:
	ThreadHandlerClientToReadyMessage();
	~ThreadHandlerClientToReadyMessage();

	ThreadHandlerClientToReadyMessage(const ThreadHandlerClientToReadyMessage& _Other) = delete;
	ThreadHandlerClientToReadyMessage(ThreadHandlerClientToReadyMessage&& _Other) noexcept = delete;
	ThreadHandlerClientToReadyMessage& operator=(const ThreadHandlerClientToReadyMessage& _Other) = delete;
	ThreadHandlerClientToReadyMessage& operator=(ThreadHandlerClientToReadyMessage&& _Other) noexcept = delete;

	void Start() override;

protected:

private:

};

