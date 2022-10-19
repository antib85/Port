#pragma once
#include <GameServerCore\GameServerSection.h>
#include "Monster.h"

class FightZone : public GameServerSection
{
public:

	FightZone();
	~FightZone();


	FightZone(const FightZone& _Other) = delete;
	FightZone(FightZone&& _Other) noexcept = delete;
	FightZone& operator=(const FightZone& _Other) = delete;
	FightZone& operator=(FightZone&& _Other) noexcept = delete;

protected:
	int MaxCount;


private:
	void UserUpdate();
	bool Init() override;
};

