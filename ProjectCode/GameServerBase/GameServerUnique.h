#pragma once

class GameServerUnique
{
private: 
	static std::atomic<uint64_t> UniqueId;

public:
	static uint64_t GetNextUniqueId();

private:
	GameServerUnique();
	~GameServerUnique();

	GameServerUnique(const GameServerUnique& _Other) = delete;
	GameServerUnique(GameServerUnique&& _Other) noexcept;

protected:
	GameServerUnique& operator=(const GameServerUnique& _Other) = delete;
	GameServerUnique& operator=(GameServerUnique&& _Other) = delete;

private:

public: 
};

