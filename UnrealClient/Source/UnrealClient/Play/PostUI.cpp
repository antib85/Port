// Fill out your copyright notice in the Description page of Project Settings.


#include "PostUI.h"
#include "../Play/PostItemObject.h"
#include "../Play/PostListItem.h"
#include "../Play/PostBox.h"
#include "../Global/ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../Global/ClientBlueprintFunctionLibrary.h"
#include "../Message/Messages.h"
#include "../UnrealClient.h"
#include "../Message/ServerToClient.h"
#include "../Message/ClientToServer.h"
#include "../Message/ServerAndClient.h"


void UPostUI::NewPostAdd(UObject* _Object, UUserWidget* _Widget)
{
	UPostItemObject* Info = Cast<UPostItemObject>(_Object);
	UPostListItem* Widget = Cast<UPostListItem>(_Widget);

	if (nullptr == Info
		|| false == Info->IsValidLowLevel())
	{
		return;
	}

	if (nullptr == Widget
		|| false == Widget->IsValidLowLevel())
	{
		return;
	}

	Widget->SetNickName(Info->ConvertNickName);
	Widget->SetScore(Info->Letters);
	Widget->SetPostTime(Info->PostTime);
	
}

void UPostUI::SetListVisibility()
{
	UClientGameInstance* Inst = Cast<UClientGameInstance>(GetGameInstance());

	if (Inst->PostListView_->GetVisibility() == ESlateVisibility::Hidden)
	{
		Inst->PostListView_->SetVisibility(ESlateVisibility::Visible);
		Inst->PostBox_->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPostUI::SetLetterVisibility()
{
	UClientGameInstance* Inst = Cast<UClientGameInstance>(GetGameInstance());

	if (Inst->PostListView_->GetVisibility() != ESlateVisibility::Hidden)
	{
		Inst->PostListView_->SetVisibility(ESlateVisibility::Hidden);
		Inst->PostBox_->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPostUI::NativeConstruct()
{

	Super::NativeConstruct();

	UClientGameInstance* Inst = Cast<UClientGameInstance>(GetGameInstance());

	Inst->PostListView_ = Cast<UListView>(GetWidgetFromName(TEXT("PostList")));
	Inst->PostBox_ = Cast<UPostBox>(GetWidgetFromName(TEXT("PostBox")));

	PostListView_ = Inst->PostListView_;
	PostBox_ = Inst->PostBox_;

	if (nullptr == Inst->PostListView_
		|| false == Inst->PostListView_->IsValidLowLevel())
	{
		int a = 0;
		return;
	}

	if (nullptr == Inst->PostBox_
		|| false == Inst->PostBox_->IsValidLowLevel())
	{
		int a = 0;
		return;
	}

	Inst->PostListView_->ClearListItems();

}
