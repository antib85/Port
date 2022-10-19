#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerToClient.h"
#include "ContentsUserData.h"
#include <memory>

// 그 세션도 여기서 받는다.
class ThreadHandlerUserPostMessage final
	: public ThreadHandlerBase<UserPostMessage>
{
private: // Member Var

public: // Default
	ThreadHandlerUserPostMessage();
	~ThreadHandlerUserPostMessage();
	ThreadHandlerUserPostMessage(const ThreadHandlerUserPostMessage& _Other) = delete;
	ThreadHandlerUserPostMessage(ThreadHandlerUserPostMessage&& _Other) noexcept = delete;

	// 이 메세지를 처음 받았을때 실행하는 함수
	void Start() override;

public:

private:
	void DBCheck();

	void ResultSend();

protected:
	ThreadHandlerUserPostMessage& operator=(const ThreadHandlerUserPostMessage& _Other) = delete;
	ThreadHandlerUserPostMessage& operator=(ThreadHandlerUserPostMessage&& _Other) = delete;

};

