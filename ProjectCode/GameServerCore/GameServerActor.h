#pragma once
#include <GameServerBase\GameServerObjectBase.h>
#include <GameServerBase\GameServerMath.h>
#include <GameServerNet\IPEndPoint.h>
#include "GameServerCollision.h"

class UDPSession;
class TCPSession;
class GameServerSection;
class GameServerMessage;
class GameServerActor : public GameServerObjectBase, public GameServerNameBase
{
	friend GameServerSection;

public:
	uint64_t GetIndex()
	{
		return Index_;
	}

	uint64_t GetThreadIndex()
	{
		return ThreadIndex_;
	}

	uint64_t GetSectionIndex()
	{
		return SectionIndex_;
	}

	bool IsEmptyMessage() 
	{
		return Messagequeue_.empty();
	}

	std::shared_ptr<GameServerMessage> PopMessage() 
	{
		if (true == Messagequeue_.empty())
		{
			return nullptr;
		}

		std::shared_ptr<GameServerMessage> Message = Messagequeue_.front();
		Messagequeue_.pop();
		return Message;
	}

	bool GetSectionCompare(GameServerSection* _Value)
	{
		return Section_ == _Value;
	}

	inline bool IsSectionMove()
	{
		return IsSectionMove_;
	}

	GameServerActor();
	~GameServerActor();

	GameServerActor(const GameServerActor& _Other) = delete;
	GameServerActor(GameServerActor&& _Other) noexcept = delete;
	GameServerActor& operator=(const GameServerActor& _Other) = delete;
	GameServerActor& operator=(GameServerActor&& _Other) noexcept = delete;

protected:
	GameServerSection* GetSection()
	{
		return Section_;
	}

	std::shared_ptr<TCPSession> GetTCPSession()
	{
		return TCPSession_;
	}

	std::shared_ptr<UDPSession> GetUDPSession()
	{
		return UDPSession_;
	}

	int GetUDPPort()
	{
		return UDPPort_;
	}

	virtual void SectionInitialize() = 0;
	virtual void TCPSessionInitialize() = 0;
	virtual void UDPSessionInitialize() = 0;


private:
	GameServerSection* Section_;

	void SetSection(GameServerSection* _Section)
	{
		Section_ = _Section;
		if (nullptr != Section_)
		{
			SectionInitialize();
		}
	}

	std::shared_ptr<TCPSession> TCPSession_;

	void SetTCPSession(std::shared_ptr<TCPSession> _Session)
	{
		TCPSession_ = _Session;
		TCPSessionInitialize();
	}

	std::shared_ptr<UDPSession> UDPSession_;
	int UDPPort_;

	bool IsFirstUDPMessage = false;

	std::atomic<bool> IsSectionMove_;

	inline void SectionMoveEnd()
	{
		IsSectionMove_ = false;
	}

	IPEndPoint UDPEndPoint;

	IPEndPoint GetUDPEndPoint()
	{
		return UDPEndPoint;
	}

	const IPEndPoint& GetUDPEndPointConstRef() const
	{
		return UDPEndPoint;
	}


	void SetUDPEndPoint(const IPEndPoint& _UDPEndPoint)
	{
		UDPEndPoint = _UDPEndPoint;
	}

	void SetUDPSession();



	uint64_t Index_;
	uint64_t ThreadIndex_;
	uint64_t SectionIndex_;




	void SetIndex(uint64_t _Index)
	{
		Index_ = _Index;
	}

	void SetSectionIndex(uint64_t _Index)
	{
		SectionIndex_ = _Index;
	}

	void SetThreadIndex(uint64_t _Index)
	{
		ThreadIndex_ = _Index;
	}

	virtual void Update(float _Time) = 0;

	virtual bool InsertSection() = 0;

	virtual void DeathEvent() = 0;

	virtual void PlayerbleActorEvent(GameServerActor* _Actor) {}
	virtual void AIActorEvent(GameServerActor* _Actor) {}


	std::queue<std::shared_ptr<GameServerMessage>> Messagequeue_;

private:
	FVector4 Pos_;
	FVector4 Scale_;
	FVector4 Dir_;

public:
	FVector4 GetPos()
	{
		return Pos_;
	}

	FVector4 GetDir()
	{
		return Dir_;
	}

	void SetPos(const FVector4& _Value)
	{
		Pos_ = _Value;
	}

	void SetScale(const FVector4& _Value)
	{
		Scale_ = _Value;
	}

	void SetDir(const FVector4& _Value)
	{
		Dir_ = _Value;
	}

	void Move(const FVector4& _Value)
	{
		Pos_ += _Value;
	}
};
