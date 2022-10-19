// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NonePlayableCharacter.h"
#include "..\Message\ContentsEnum.h"
#include "../Message/ServerToClient.h"
#include "../Message/ServerAndClient.h"
#include "../Message/ClientToServer.h"
#include "ClientMonster.generated.h"

struct MonsterInfo
{
	float LookRange;

	float TraceRange;

	float AttRange;

	float Att;
	float Hp;
	float Speed;
};

UCLASS()
class UNREALCLIENT_API AClientMonster : public ANonePlayableCharacter
{
	GENERATED_BODY()


public:
	virtual void ObjectInit() override;

protected:
	virtual void BeginPlay() override;

	void Tick(float DeltaTime);

private:
	EMonsterState State_;

	AActor* Target;

	MonsterInfo Info;
	float Ratio;

	TArray<FMonsterUpdateData> UpDataData;
	FMonsterUpdateData CurrentUpdateData;

	void Att(float _DelataTime);
	void Idle(float _DelataTime);
	void Trace(float _DelataTime);

	void AnimationStart(ClientAnimationType _Value);
	void AnimationEnd(ClientAnimationType _Value);

public:
	void ClientUpdate(float _DelataTime);
	void SetClientDamage(float Damage);
	void ClientObjectInit();
};
