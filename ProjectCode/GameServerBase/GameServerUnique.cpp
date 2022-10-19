#include "PreCompile.h"
#include "GameServerUnique.h"
#include "DateTime.h"


std::atomic<uint64_t> GameServerUnique::UniqueId = 0;

GameServerUnique::GameServerUnique() 
{

}

GameServerUnique::~GameServerUnique() 
{

}

GameServerUnique::GameServerUnique(GameServerUnique&& _Other) noexcept
{

}



uint64_t GameServerUnique::GetNextUniqueId()
{
	++UniqueId;
	return UniqueId;
}