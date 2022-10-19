// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PostListItem.generated.h"

UCLASS()
class UNREALCLIENT_API UPostListItem : public UUserWidget
{
	GENERATED_BODY()


public:
	FORCEINLINE void SetNickName(const FString& _NickName)
	{
		NickName = _NickName;
	}

	FORCEINLINE void SetScore(const FString& _Letter)
	{
		Letter = _Letter;
	}

	FORCEINLINE void SetPostTime(const FString& _PostTime)
	{
		PostTime = _PostTime;
	}

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Data", meta = (AllowPrivateAccess = "true"))
	FString NickName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Data", meta = (AllowPrivateAccess = "true"))
	FString Letter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Data", meta = (AllowPrivateAccess = "true"))
	FString PostTime;
	
};
