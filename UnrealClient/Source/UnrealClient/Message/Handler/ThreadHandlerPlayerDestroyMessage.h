#pragma once
#include "../ThreadHandlerBase.h"

class ThreadHandlerPlayerDestroyMessage : public ThreadHandlerBase<PlayerDestroyMessage>
{
public: 
	void Start();
};

