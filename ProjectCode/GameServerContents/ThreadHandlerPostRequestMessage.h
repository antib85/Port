#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerToClient.h"
#include "ContentsUserData.h"
#include <memory>

class ThreadHandlerPostRequestMessage final
	: public ThreadHandlerBase<PostRequestMessage>
{
private: 
	UserPostMessage Result_;

public: 
	ThreadHandlerPostRequestMessage();
	~ThreadHandlerPostRequestMessage();
	ThreadHandlerPostRequestMessage(const ThreadHandlerPostRequestMessage& _Other) = delete;
	ThreadHandlerPostRequestMessage(ThreadHandlerPostRequestMessage&& _Other) noexcept = delete;

	void Start() override;

public:

private:
	void DBCheck();

	void ResultSend();

protected:
	ThreadHandlerPostRequestMessage& operator=(const ThreadHandlerPostRequestMessage& _Other) = delete;
	ThreadHandlerPostRequestMessage& operator=(ThreadHandlerPostRequestMessage&& _Other) = delete;

};

