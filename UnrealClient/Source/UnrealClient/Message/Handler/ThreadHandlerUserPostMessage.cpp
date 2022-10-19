#include "ThreadHandlerUserPostMessage.h"
#include "../../Global/ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../../CharacterSelect/CharacterListItem.h"
#include "../../Global/ClientBlueprintFunctionLibrary.h"
#include "Components/ListView.h"
#include "../../UnrealClient.h"
#include "../../Play/PlayGameMode.h"
#include "../../Play/ClientCharacter.h"
#include "../../Play/PostItemObject.h"
#include "../../Play/PostListItem.h"
#include "Components/ListView.h"
#include "../../Play/PostUI.h"


void ThreadHandlerUserPostMessage::Start()
{
	APlayGameMode* PGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(World_));

	if (nullptr == PGameMode
		|| false == PGameMode->IsValidLowLevel())
	{
		UE_LOG(ClientLog, Error, TEXT("%S(%u) > GameMode Is nullptr"), __FUNCTION__, __LINE__);
		return;
	}

	Inst_->PostListView_->ClearListItems();

	for (size_t i = 0; i < Message_->Posts.size(); i++)
	{
		UPostItemObject* PostObject = NewObject<UPostItemObject>();
		UClientBlueprintFunctionLibrary::UTF8ToFString(Message_->Posts[i].ToNickName, PostObject->ConvertNickName);
		UClientBlueprintFunctionLibrary::UTF8ToFString(Message_->Posts[i].Letters, PostObject->Letters);
		UClientBlueprintFunctionLibrary::UTF8ToFString(Message_->Posts[i].PostTime, PostObject->PostTime);
		Inst_->PostListView_->AddItem(PostObject);
	}
}

