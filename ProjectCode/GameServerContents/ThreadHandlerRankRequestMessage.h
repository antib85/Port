#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerToClient.h"
#include "ContentsUserData.h"
#include <memory>

class ThreadHandlerRankRequestMessage final
	: public ThreadHandlerBase<RankRequestMessage>
{
private: 
	UserRankWindowDataMessage Result;

public: 
	ThreadHandlerRankRequestMessage();
	~ThreadHandlerRankRequestMessage();
	ThreadHandlerRankRequestMessage(const ThreadHandlerRankRequestMessage& _Other) = delete;
	ThreadHandlerRankRequestMessage(ThreadHandlerRankRequestMessage&& _Other) noexcept = delete;

	void Start() override;

public:

private:
	void DBCheck();

	void ResultSend();

protected:
	ThreadHandlerRankRequestMessage& operator=(const ThreadHandlerRankRequestMessage& _Other) = delete;
	ThreadHandlerRankRequestMessage& operator=(ThreadHandlerRankRequestMessage&& _Other) = delete;

};

