#pragma once
#include "../ThreadHandlerBase.h"

class ThreadHandlerChatMessage : public ThreadHandlerBase<ChatMessage>
{
public:
	void Start();
};

