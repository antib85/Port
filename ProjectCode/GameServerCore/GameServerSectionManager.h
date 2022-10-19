#pragma once
#include <vector>
#include <GameServerBase\GameServerThread.h>
#include <unordered_map>
#include "GameServerSection.h"
#include "GameServerSectionThread.h"
#include <GameServerMessage\GameServerMessage.h>

class GameServerSectionManager
{
private:
	static GameServerSectionManager* Inst_;

public:
	static GameServerSectionManager* GetInst() 
	{
		return Inst_;
	}

	static void Destroy() 
	{
		if (nullptr != Inst_)
		{
			delete Inst_;
			Inst_ = nullptr;
		}
	}

	template<typename SectionType, typename EnumType, typename ... Parameter>
	std::shared_ptr<SectionType> CreateSection(int _ThreadIndex, EnumType _Index, Parameter ... Arg)
	{
		return CreateSection<SectionType>(_ThreadIndex, static_cast<uint64_t>(_Index), Arg...);
	}

	template<typename SectionType, typename ... Parameter>
	std::shared_ptr<SectionType> CreateSection(int _ThreadIndex, uint64_t _Index, Parameter ... Arg)
	{
		std::shared_ptr<SectionType> NewSection = std::make_shared<SectionType>(Arg...);
		InsertSection(_ThreadIndex, _Index, NewSection);
		return NewSection;
	}

	void InsertSection(int _ThreadIndex, uint64_t Key, std::shared_ptr<GameServerSection> _Section);

	uint32_t RemoveSection(int _ThreadIndex, uint32_t _SectionNumber);

	std::shared_ptr<GameServerSection> FindSection(uint64_t Key);

	GameServerSectionManager(const GameServerSectionManager& _Other) = delete;
	GameServerSectionManager(GameServerSectionManager&& _Other) noexcept = delete;
	GameServerSectionManager& operator=(const GameServerSectionManager& _Other) = delete;
	GameServerSectionManager& operator=(GameServerSectionManager&& _Other) noexcept = delete;

	void Init(int _Count, const std::string& _Name);

	void MessagePost(uint64_t ThreadIndex, std::function<void()> _CallBack);

	void ActorPost(uint64_t ThreadIndex, uint64_t SectionIndex, uint64_t ObjectId, std::shared_ptr<GameServerMessage> _Message);

	void ActorPointPost(uint64_t ThreadIndex, uint64_t SectionIndex, uint64_t ObjectId, const IPEndPoint& _Point, std::shared_ptr<GameServerMessage> _Message);

protected:

private:
	std::vector<std::shared_ptr<GameServerSectionThread>> SecitonThread_;

	std::mutex SectionMutex_;
	std::unordered_map<uint64_t, std::shared_ptr<GameServerSection>> AllSection_;

	GameServerSectionManager();
	~GameServerSectionManager();
};

