#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerToClient.h"
#include "ContentsUserData.h"
#include <memory>

// �� ���ǵ� ���⼭ �޴´�.
class ThreadHandlerUserPostMessage final
	: public ThreadHandlerBase<UserPostMessage>
{
private: // Member Var

public: // Default
	ThreadHandlerUserPostMessage();
	~ThreadHandlerUserPostMessage();
	ThreadHandlerUserPostMessage(const ThreadHandlerUserPostMessage& _Other) = delete;
	ThreadHandlerUserPostMessage(ThreadHandlerUserPostMessage&& _Other) noexcept = delete;

	// �� �޼����� ó�� �޾����� �����ϴ� �Լ�
	void Start() override;

public:

private:
	void DBCheck();

	void ResultSend();

protected:
	ThreadHandlerUserPostMessage& operator=(const ThreadHandlerUserPostMessage& _Other) = delete;
	ThreadHandlerUserPostMessage& operator=(ThreadHandlerUserPostMessage&& _Other) = delete;

};

