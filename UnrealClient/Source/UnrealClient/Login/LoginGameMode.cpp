// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameMode.h"
#include "../Global/ClientGameInstance.h"
#include "../UnrealClient.h"


ALoginGameMode::ALoginGameMode() 
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;


	int Test = sizeof(FPlayerUpdateData);

	UE_LOG(ClientLog, Error, TEXT("%S(%u) > %d size"), __FUNCTION__, __LINE__, Test);
}

void ALoginGameMode::Tick(float _Delta) 
{
	Super::Tick(_Delta);

	int a = 0;
}