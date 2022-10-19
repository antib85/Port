#pragma once
#include "../ThreadHandlerBase.h"

class ThreadHandlerCharacterListMessage : public ThreadHandlerBase<CharacterListMessage>
{
public:
	void Start();
};

