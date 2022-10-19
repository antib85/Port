#include "PreCompile.h"
#include "GameServerSectionThread.h"
#include <GameServerBase\GameServerDebug.h>

GameServerSectionThread::GameServerSectionThread() 
{
	Sections_.reserve(100);
}

GameServerSectionThread::~GameServerSectionThread() 
{
}

void GameServerSectionThread::ThreadFunction(int _Index)
{
	SetThreadOrder(_Index);

	SectionThreadQueue.Initialize(1);
	SectionThreadQueue.SetExecuteWorkType(GameServerQueue::WORK_TYPE::Default);
	Timer_.Reset();

	GameServerQueue::QUEUE_RETURN Result = GameServerQueue::QUEUE_RETURN::OK;

	while (GameServerQueue::QUEUE_RETURN::DESTROY != Result)
	{
		Result = GameServerQueue::QUEUE_RETURN::OK;

		while (Result == GameServerQueue::QUEUE_RETURN::OK)
		{
			Result = SectionThreadQueue.Execute(1);
		}

		if (GameServerQueue::QUEUE_RETURN::DESTROY == Result)
		{
			return;
		}

		if (0 != InsertSectionSize_)
		{
			std::lock_guard Lock(InsertLock_);
			for (size_t i = 0; i < InsertSections_.size(); i++)
			{
				InsertSections_[i]->AccTimeReset();
				Sections_.push_back(InsertSections_[i]);
				KeySections_.insert(std::make_pair(InsertSections_[i]->GetIndex(), InsertSections_[i]));
			}
			InsertSections_.clear();
			InsertSectionSize_ = 0;
		}

		float DelataTime =Timer_.Update();

		for (size_t i = 0; i < Sections_.size(); i++)
		{
			if (false == Sections_[i]->IsUpdate())
			{
				continue;
			}

			Sections_[i]->AccTimeUpdate(DelataTime);
			Sections_[i]->Update(DelataTime);
			Sections_[i]->Release();
		}

		Sleep(1);
	}
}

void GameServerSectionThread::AddSection(std::shared_ptr<GameServerSection> _Section) 
{
	++InsertSectionSize_;
	_Section->SetThread(this);
	InsertSections_.push_back(_Section);
}

void GameServerSectionThread::RemoveSection(std::shared_ptr<GameServerSection> _Section)
{
	if (nullptr == _Section)
	{
		GameServerDebug::LogErrorAssert("Section Is null");
		return;
	}


	for (size_t i = 0; i < Sections_.size(); i++)
	{
		if (Sections_[i] == _Section)
		{
			Sections_[i]->IsDeath();
		}
	}

	++DeleteSectionSize_;

}

void GameServerSectionThread::StartSectionThread(int _Index)
{
	Start("SectionThread", std::bind(&GameServerSectionThread::ThreadFunction, this, _Index), nullptr, nullptr);
}

void GameServerSectionThread::ActorPost(uint64_t SectionIndex, uint64_t _ObjectIndex, std::shared_ptr<GameServerMessage> _Message)
{
	if (KeySections_.end() == KeySections_.find(SectionIndex))
	{
		GameServerDebug::AssertDebugMsg("존재하지 않는 섹션에 메세지를 보냈습니다.");
		return;
	}

	KeySections_[SectionIndex]->ActorPost(_ObjectIndex, _Message);
}

void GameServerSectionThread::ActorPointPost(uint64_t SectionIndex, uint64_t _ObjectIndex, const IPEndPoint& _EndPoint, std::shared_ptr<GameServerMessage> _Message)
{
	if (KeySections_.end() == KeySections_.find(SectionIndex))
	{
		GameServerDebug::AssertDebugMsg("존재하지 않는 섹션에 메세지를 보냈습니다.");
		return;
	}

	KeySections_[SectionIndex]->ActorPointPost(_ObjectIndex, _EndPoint, _Message);
}