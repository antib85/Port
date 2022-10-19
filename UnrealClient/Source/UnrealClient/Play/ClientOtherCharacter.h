// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NonePlayableCharacter.h"
#include "ClientEnums.h"
#include "ClientOtherCharacter.generated.h"


UCLASS()
class UNREALCLIENT_API AClientOtherCharacter : public ANonePlayableCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AClientOtherCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AnimationEnd(ClientAnimationType _Value);
};
