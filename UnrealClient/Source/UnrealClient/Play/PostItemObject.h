// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PostItemObject.generated.h"

UCLASS()
class UNREALCLIENT_API UPostItemObject : public UObject
{
	GENERATED_BODY()

public:
	FString ConvertNickName;
	FString Letters;
	FString PostTime;
	
};
