#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerAndClient.h"
#include "ServerToClient.h"

class ThreadHandlerUDPConnectResultMessage final
	: public ThreadHandlerBase<UDPConnectResultMessage>
{

public:
	ThreadHandlerUDPConnectResultMessage();
	~ThreadHandlerUDPConnectResultMessage();

	ThreadHandlerUDPConnectResultMessage(const ThreadHandlerUDPConnectResultMessage& _Other) = delete;
	ThreadHandlerUDPConnectResultMessage(ThreadHandlerUDPConnectResultMessage&& _Other) noexcept = delete;
	ThreadHandlerUDPConnectResultMessage& operator=(const ThreadHandlerUDPConnectResultMessage& _Other) = delete;
	ThreadHandlerUDPConnectResultMessage& operator=(ThreadHandlerUDPConnectResultMessage&& _Other) noexcept = delete;

	void Start() override;

protected:

private:

};

