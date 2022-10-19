#include "PreCompile.h"
#include "FightZone.h"

FightZone::FightZone() 
{
}

FightZone::~FightZone() 
{
}


void FightZone::UserUpdate()
{
	if (0 == GetAiActors().size())
	{
		CreateActor<Monster>();
	}

}

bool FightZone::Init() 
{
	return true;
}