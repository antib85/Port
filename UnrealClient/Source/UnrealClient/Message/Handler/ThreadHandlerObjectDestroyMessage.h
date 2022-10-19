#pragma once
#include "../ThreadHandlerBase.h"

class ThreadHandlerObjectDestroyMessage : public ThreadHandlerBase<ObjectDestroyMessage>
{
public: 
	void Start();
};

