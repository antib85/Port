#pragma once
#include "GameServerObjectBase.h"
#include <string>

class GameServerNameBase 
{
private: 
	std::string name_;

public:
	virtual void SetName(const std::string&);

	std::string_view GetNameView();
	std::string GetNameCopy();
	const char* GetNamePtr();

public:
	GameServerNameBase();
	~GameServerNameBase();

	GameServerNameBase(const GameServerNameBase& _Other) = delete;
	GameServerNameBase(GameServerNameBase&& _Other) noexcept;

protected:
	GameServerNameBase& operator=(const GameServerNameBase& _Other) = delete;
	GameServerNameBase& operator=(GameServerNameBase&& _Other) = delete;

private:

public: 
};

