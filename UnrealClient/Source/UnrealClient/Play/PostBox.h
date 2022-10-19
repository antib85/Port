// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PostBox.generated.h"

UCLASS()
class UNREALCLIENT_API UPostBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Post")
	void PostNickName(FString _NickName, ETextCommit::Type _Type);

	UFUNCTION(BlueprintCallable, Category = "Post")
	void PostLetters(FString _Text, ETextCommit::Type _Type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post")
	FString ToNickName_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post")
	FString Letters_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post")
	FString PostTime_;

	UFUNCTION(BlueprintCallable, Category = "Post")
	void PostSend();


private:
	void NativeConstruct() override;
};
