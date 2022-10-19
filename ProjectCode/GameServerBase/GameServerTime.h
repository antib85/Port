#pragma once
#include <chrono>

// Ό³Έν :
class GameServerTime
{
public:
	// constrcuter destructer
	GameServerTime();
	~GameServerTime();

	// delete Function
	GameServerTime(const GameServerTime& _Other) = delete;
	GameServerTime(GameServerTime&& _Other) noexcept = delete;
	GameServerTime& operator=(const GameServerTime& _Other) = delete;
	GameServerTime& operator=(GameServerTime&& _Other) noexcept = delete;

	std::chrono::high_resolution_clock Timer_;
	std::chrono::steady_clock::time_point Current_;
	std::chrono::steady_clock::time_point Prev_;
	std::chrono::system_clock NowTime_;

	double DoubleDeltaTime;
	float DeltaTime;
	std::string NowTime;

	void Reset();

	float Update();

	float GetDeltaTime() 
	{
		return DeltaTime;
	}

	const std::string GetNowTime();

protected:
	

private:

};

