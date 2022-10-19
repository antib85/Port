#pragma once
#include "ContentsStructure.h"
#include "UserTable.h"
#include <GameServerBase\GameServerObjectBase.h>

class ContentsUserData : public GameServerObjectBase
{

public:
	UserRow Data;
	std::vector<FCharacterInfo> Characters;
	FCharacterInfo SelectData;

	ContentsUserData();
	~ContentsUserData();

	ContentsUserData(const ContentsUserData& _Other) = delete;
	ContentsUserData(ContentsUserData&& _Other) noexcept = delete;
	ContentsUserData& operator=(const ContentsUserData& _Other) = delete;
	ContentsUserData& operator=(ContentsUserData&& _Other) noexcept = delete;

protected:

private:

};

