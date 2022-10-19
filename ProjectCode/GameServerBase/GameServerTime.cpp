#include "PreCompile.h"
#include "GameServerTime.h"
#include <chrono>
#include <iomanip>

GameServerTime::GameServerTime() 
{
}

GameServerTime::~GameServerTime() 
{
}



void GameServerTime::Reset()
{
	Current_ = Timer_.now();
	Prev_ = Timer_.now();
}

float GameServerTime::Update()
{
	Current_ = Timer_.now();
	DoubleDeltaTime = std::chrono::duration<double>(Current_ - Prev_).count();
	Prev_ = Current_;

	DeltaTime = static_cast<float>(DoubleDeltaTime);
	return DeltaTime;
}

const std::string GameServerTime::GetNowTime()
{
	/*std::chrono::system_clock::time_point tpoint{ std::chrono::system_clock::now() };
	time_t tt{ std::chrono::system_clock::to_time_t(tpoint) };
	tm t{};
	localtime_s(&t, &tt);
	std::stringstream ss;
	ss << std::put_time(&t, "%F %H:%M:%S");
	std::string stringTime(ss.str());
	char buffer[80]{ 0 };
	strftime(buffer, sizeof(buffer), "%F %H:%M:%S", &t);

	std::string Time = stringTime;*/

	std::chrono::system_clock::time_point tpoint{ NowTime_.now() };
	time_t tt{ std::chrono::system_clock::to_time_t(tpoint) };
	tm t{};
	localtime_s(&t, &tt);
	std::stringstream ss;
	ss << std::put_time(&t, "%F %H:%M:%S");
	NowTime = ss.str();
	std::string stringTime(ss.str());
	
	/*char buffer[80]{ 0 };
	strftime(buffer, sizeof(buffer), "%F %H:%M:%S", &t);*/

	return NowTime;
}