#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerAndClient.h"
#include "ServerToClient.h"

class ThreadHandlerLevelMoveReplyMessage final
	: public ThreadHandlerBase<LevelMoveReplyMessage>
{

public:
	ThreadHandlerLevelMoveReplyMessage();
	~ThreadHandlerLevelMoveReplyMessage();

	ThreadHandlerLevelMoveReplyMessage(const ThreadHandlerLevelMoveReplyMessage& _Other) = delete;
	ThreadHandlerLevelMoveReplyMessage(ThreadHandlerLevelMoveReplyMessage&& _Other) noexcept = delete;
	ThreadHandlerLevelMoveReplyMessage& operator=(const ThreadHandlerLevelMoveReplyMessage& _Other) = delete;
	ThreadHandlerLevelMoveReplyMessage& operator=(ThreadHandlerLevelMoveReplyMessage&& _Other) noexcept = delete;

	void Start() override;

protected:

private:

};

