#pragma once
#include "../ThreadHandlerBase.h"

class ThreadHandlerServerDestroyMessage : public ThreadHandlerBase<ServerDestroyMessage>
{
public: 
	void Start();
};

