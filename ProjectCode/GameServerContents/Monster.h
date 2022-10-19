#pragma once
#include <GameServerCore\GameServerActor.h>
#include "ServerToClient.h"
#include "Monster.h"

class Monster : public GameServerActor
{
public:
	Monster();
	~Monster();

	Monster(const Monster& _Other) = delete;
	Monster(Monster&& _Other) noexcept = delete;
	Monster& operator=(const Monster& _Other) = delete;
	Monster& operator=(Monster&& _Other) noexcept = delete;

protected:
	MonsterUpdateMessage Message_;
	GameServerSerializer Serializer_;

	GameServerActor* Target;


	float UpdateTime;
	float AttTime;

	GameServerSerializer& GetSerializeMonsterUpdateMessage();
	void BroadcastingMonsterUpdateMessage();

	GameServerCollision* PlayerSensorCollision;
	GameServerCollision* HitCollision;

private:
	void Update(float _Time) override;

	void SectionInitialize() override;
	void TCPSessionInitialize() override;
	void UDPSessionInitialize() override;

	bool InsertSection() override;

	void DeathEvent() override;

public:
	void ChangeState(EMonsterState _State);

	void StateUpdate(float _DeltaTime);

	void IdleStart();
	void TraceStart();
	void AttStart();

	void IdleUpdate(float _DeltaTime);
	void TraceUpdate(float _DeltaTime);
	void AttUpdate(float _DeltaTime);
};

