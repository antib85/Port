#pragma once
#include <GameServerBase\GameServerObjectBase.h>
#include <GameServerBase\GameServerUnique.h>
#include <GameServerNet\IPEndPoint.h>

class GameServerCollision;
class TCPSession;
class GameServerActor;
class GameServerMessage;
class GameServerSection : public GameServerObjectBase, public GameServerNameBase
{
	friend class GameServerSectionThread;
	friend class GameServerSectionManager;
	friend class GameServerCollision;

private:
	class MoveActorData
	{
	public:
		std::shared_ptr<GameServerActor> MoveActor;
		GameServerSection* NextSection;
	};

public:
	GameServerSection();
	virtual ~GameServerSection();

	GameServerSection(const GameServerSection& _Other) = delete;
	GameServerSection(GameServerSection&& _Other) noexcept = delete;
	GameServerSection& operator=(const GameServerSection& _Other) = delete;
	GameServerSection& operator=(GameServerSection&& _Other) noexcept = delete;

	virtual bool Init() = 0;

	bool Update(float _Time);
	void Release();

	inline uint64_t GetIndex()
	{
		return Index_;
	}

	uint64_t GetThreadIndex()
	{
		return ThreadIndex_;
	}

	template<typename ActorType, typename ... Parameter>
	std::shared_ptr<ActorType> CreateActor(Parameter ... Arg)
	{
		uint64_t ReturnId = GameServerUnique::GetNextUniqueId();
		std::shared_ptr<ActorType> NewActor = CreateActor<ActorType>(ReturnId, Arg...);
		return NewActor;
	}

	template<typename ActorType, typename ... Parameter>
	std::shared_ptr<ActorType> CreateActor(std::shared_ptr<TCPSession> _Session, Parameter ... Arg)
	{
		uint64_t ReturnId = GameServerUnique::GetNextUniqueId();
		std::shared_ptr<ActorType> NewActor = CreateActor<ActorType>(_Session, ReturnId, Arg...);
		return NewActor;
	}

	template<typename ActorType, typename ... Parameter>
	std::shared_ptr<ActorType> CreateActor(uint64_t _ID, Parameter ... Arg)
	{
		std::shared_ptr<ActorType> NewActor = std::make_shared<ActorType>(Arg...);
		NewActor->SetIndex(_ID);
		InsertActor(_ID, std::dynamic_pointer_cast<GameServerActor>(NewActor));
		return NewActor;
	}

	template<typename ActorType, typename ... Parameter>
	std::shared_ptr<ActorType> CreateActor(std::shared_ptr<TCPSession> _Session, uint64_t _ID, Parameter ... Arg)
	{
		std::shared_ptr<ActorType> NewActor = std::make_shared<ActorType>(Arg...);
		NewActor->SetIndex(_ID);
		NewActor->SetTCPSession(_Session);
		NewActor->SetUDPSession();
		InsertActor(_ID, std::dynamic_pointer_cast<GameServerActor>(NewActor));
		return NewActor;
	}

	void TCPBroadcasting(const std::vector<unsigned char>& _Buffer, uint64_t _IgnoreIndex = -1);

	void UDPBroadcasting(const std::vector<unsigned char>& _Buffer, uint64_t _IgnoreIndex = -1);

	const std::list<std::shared_ptr<GameServerActor>>& GetPlayableActor()
	{
		return PlayableActor_;
	}

	void MoveActor(std::shared_ptr<GameServerActor> _Actor);

	void SectionMove(std::shared_ptr<GameServerActor> _MoveActor, GameServerSection* _MoveSection );

protected:




private:
	inline void SetIndex(uint64_t _Index)
	{
		Index_ = _Index;
	}

	inline void SetThreadIndex(uint64_t _Index)
	{
		ThreadIndex_ = _Index;
	}

	uint64_t Index_; 

	uint64_t ThreadIndex_; 

	GameServerSectionThread*  GetThread()
	{
		return Thread_;
	}

private:
	virtual void UserUpdate() = 0;

	void InsertActor(uint64_t _ID, std::shared_ptr<GameServerActor> _Actor);

	GameServerSectionThread* Thread_;

	void SetThread(GameServerSectionThread* _Thread)
	{
		Thread_ = _Thread;
	}

	std::map<uint64_t, std::shared_ptr<GameServerActor>> KeyActor_;

	std::list<std::shared_ptr<GameServerActor>> AIActor_;
	std::list<std::shared_ptr<GameServerActor>> PlayableActor_;

	std::atomic<size_t> WaitActorCount_;
	std::mutex WaitLock;
	std::vector<std::shared_ptr<GameServerActor>> WaitActor_;

	std::vector<MoveActorData> MoveActors_;

public:
	void ActorPost(uint64_t _ObjectIndex, std::shared_ptr<GameServerMessage> _Message);
	void ActorPointPost(uint64_t _ObjectIndex, const IPEndPoint& _EndPoint, std::shared_ptr<GameServerMessage> _Message);

	const std::list<std::shared_ptr<GameServerActor>>& GetAiActors() 
	{
		return AIActor_;
	}

private:
	std::map<int, std::list<GameServerCollision*>> CollisionList;

public:
	template<typename EnumType>
	GameServerCollision* CreateCollision(EnumType _Order, GameServerActor* _Owner)
	{
		return CreateCollision(static_cast<int>(_Order), _Owner);
	}
	GameServerCollision* CreateCollision(int Order, GameServerActor* _Owner);
};

