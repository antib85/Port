#pragma once
#include "../ThreadHandlerBase.h"

class ThreadHandlerLevelMoveMessage : public ThreadHandlerBase<LevelMoveMessage>
{
public: 
	void Start();
};

