#pragma once
#include <mutex>
#include <unordered_set>

class ContentsGlobalValue
{
public:
	static bool RegistPlayable(int _CharacterIndex);
	static bool UnRegistPlayable(int _CharacterIndex);

private:
	static std::mutex PlayableMutex;
	static std::unordered_set<int> PlayerCharacterIndex;

	ContentsGlobalValue();
	~ContentsGlobalValue();

	ContentsGlobalValue(const ContentsGlobalValue& _Other) = delete;
	ContentsGlobalValue(ContentsGlobalValue&& _Other) noexcept = delete;
	ContentsGlobalValue& operator=(const ContentsGlobalValue& _Other) = delete;
	ContentsGlobalValue& operator=(ContentsGlobalValue&& _Other) noexcept = delete;

};

