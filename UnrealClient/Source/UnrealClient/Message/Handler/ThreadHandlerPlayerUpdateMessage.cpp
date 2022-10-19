#include "ThreadHandlerPlayerUpdateMessage.h"
#include "../../Global/ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../../CharacterSelect/CharacterListItemObject.h"
#include "../../CharacterSelect/CharacterListItem.h"
#include "../../Global/ClientBlueprintFunctionLibrary.h"
#include "Components/ListView.h"
#include "../../UnrealClient.h"
#include "../../Play/PlayGameMode.h"
#include "../../Play/ClientCharacter.h"

void ThreadHandlerPlayerUpdateMessage::Start()
{
	APlayGameMode* PGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(World_));

	if (nullptr == PGameMode
		|| false == PGameMode->IsValidLowLevel())
	{
		UE_LOG(ClientLog, Error, TEXT("%S(%u) > GameMode Is nullptr"), __FUNCTION__, __LINE__);
		return;
	}

	TSubclassOf<AClientCharacter> CharacterSubClass = PGameMode->GetOtherPlayerClass();

	if (nullptr == CharacterSubClass
		|| false == CharacterSubClass->IsValidLowLevel())
	{
		UE_LOG(ClientLog, Error, TEXT("%S(%u) > Character SubClass Is nullptr"), __FUNCTION__, __LINE__);
		return;
	}

	if (false == PGameMode->IsRegist(Message_->Data.ObjectIndex))
	{
		FTransform Transform = FTransform(Message_->Data.Pos);

		AClientCharacter* NewCharacter = World_->SpawnActorDeferred<AClientCharacter>(CharacterSubClass.Get(), Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		NewCharacter->FinishSpawning(Transform);
		NewCharacter->SetId(Message_->Data.ObjectIndex);
		NewCharacter->SetObjectType(EGameObjectType::Player);
		PGameMode->RegistObject(Message_->Data.ObjectIndex, EGameObjectType::Player, NewCharacter);
		PGameMode->ObjectPushMessage(Message_->Data.ObjectIndex, Message_);
		NewCharacter->ObjectInit();
		return;
	}

	PGameMode->ObjectPushMessage(Message_->Data.ObjectIndex, Message_);
}

