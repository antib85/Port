#pragma once
#include "../ThreadHandlerBase.h"

class ThreadHandlerUDPReadyOKMessage : public ThreadHandlerBase<UDPReadyOKMessage>
{
public:
	void Start();
};

