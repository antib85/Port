#pragma once
#include "../ThreadHandlerBase.h"

class ThreadHandlerMonsterUpdateMessage : public ThreadHandlerBase<MonsterUpdateMessage>
{
public: 
	void Start();
};

