#pragma once
#include "ContentsStructure.h"
#include "UserTable.h"
#include <GameServerBase\GameServerObjectBase.h>

class Player;
class ContentsPlayerData : public GameServerObjectBase
{

public:
	Player* ConnectPlayer;

	ContentsPlayerData() {}
	~ContentsPlayerData() {}

	ContentsPlayerData(const ContentsPlayerData& _Other) = delete;
	ContentsPlayerData(ContentsPlayerData&& _Other) noexcept = delete;
	ContentsPlayerData& operator=(const ContentsPlayerData& _Other) = delete;
	ContentsPlayerData& operator=(ContentsPlayerData&& _Other) noexcept = delete;

protected:

private:

};

