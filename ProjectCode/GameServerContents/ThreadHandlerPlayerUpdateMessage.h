#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerAndClient.h"
#include "ServerToClient.h"

class ThreadHandlerPlayerUpdateMessage final
	: public ThreadHandlerBase<PlayerUpdateMessage>
{

public:
	ThreadHandlerPlayerUpdateMessage();
	~ThreadHandlerPlayerUpdateMessage();

	ThreadHandlerPlayerUpdateMessage(const ThreadHandlerPlayerUpdateMessage& _Other) = delete;
	ThreadHandlerPlayerUpdateMessage(ThreadHandlerPlayerUpdateMessage&& _Other) noexcept = delete;
	ThreadHandlerPlayerUpdateMessage& operator=(const ThreadHandlerPlayerUpdateMessage& _Other) = delete;
	ThreadHandlerPlayerUpdateMessage& operator=(ThreadHandlerPlayerUpdateMessage&& _Other) noexcept = delete;

	void Start() override;

protected:

private:

};

