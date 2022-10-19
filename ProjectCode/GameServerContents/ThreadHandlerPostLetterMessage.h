#pragma once
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerToClient.h"
#include "ContentsUserData.h"
#include <memory>

class PostRow;
class ThreadHandlerPostLetterMessage final
	: public ThreadHandlerBase<PostLetterMessage>
{
private: 

public: 
	ThreadHandlerPostLetterMessage();
	~ThreadHandlerPostLetterMessage();
	ThreadHandlerPostLetterMessage(const ThreadHandlerPostLetterMessage& _Other) = delete;
	ThreadHandlerPostLetterMessage(ThreadHandlerPostLetterMessage&& _Other) noexcept = delete;

	void Start() override;

public:

private:
	void DBCheck();

	void ResultSend();

protected:
	ThreadHandlerPostLetterMessage& operator=(const ThreadHandlerPostLetterMessage& _Other) = delete;
	ThreadHandlerPostLetterMessage& operator=(ThreadHandlerPostLetterMessage&& _Other) = delete;

};

