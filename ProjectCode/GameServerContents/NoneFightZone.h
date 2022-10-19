#pragma once
#include <GameServerCore\GameServerSection.h>


class NoneFightZone : public GameServerSection
{
public:

	NoneFightZone();
	~NoneFightZone();


	NoneFightZone(const NoneFightZone& _Other) = delete;
	NoneFightZone(NoneFightZone&& _Other) noexcept = delete;
	NoneFightZone& operator=(const NoneFightZone& _Other) = delete;
	NoneFightZone& operator=(NoneFightZone&& _Other) noexcept = delete;

protected:

private:
	void UserUpdate();

	bool Init() override;
};

