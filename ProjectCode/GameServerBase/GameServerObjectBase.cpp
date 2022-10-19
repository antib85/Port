#include "PreCompile.h"
#include "GameServerObjectBase.h"

GameServerObjectBase::GameServerObjectBase() 
	: parent_(nullptr)
	, IsUpdate_(true)
	, IsDeath_(false)
	, Frame(0)
{
	linkObject_.reserve(5);
}

GameServerObjectBase::~GameServerObjectBase() 
{

}

GameServerObjectBase::GameServerObjectBase(GameServerObjectBase&& _Other) noexcept
	: parent_(_Other.parent_)
{

}



bool GameServerObjectBase::IsLowLevelValid()
{
	if (nullptr == this)
	{
		return false;
	}

	return true;
}