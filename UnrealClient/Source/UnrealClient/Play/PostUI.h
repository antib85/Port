// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PostUI.generated.h"

UCLASS()
class UNREALCLIENT_API UPostUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat")
	class UListView* PostListView_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat")
	class UPostBox* PostBox_;

	UFUNCTION(BlueprintCallable, Category = "Create")
	void NewPostAdd(UObject* _Object, UUserWidget* _Widget);

	UFUNCTION(BlueprintCallable, Category = "Create")
	void SetListVisibility();

	UFUNCTION(BlueprintCallable, Category = "Create")
	void SetLetterVisibility();

protected:
	void NativeConstruct() override;
};
