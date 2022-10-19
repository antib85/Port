#pragma once
#include <unordered_map>

class DBManager
{
public:
	DBManager();
	~DBManager();

	DBManager(const DBManager& _Other) = delete;
	DBManager(DBManager&& _Other) noexcept = delete;
	DBManager& operator=(const DBManager& _Other) = delete;
	DBManager& operator=(DBManager&& _Other) noexcept = delete;

protected:

private:
};

