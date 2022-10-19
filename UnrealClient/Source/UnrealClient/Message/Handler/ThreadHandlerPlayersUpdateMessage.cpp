#include "ThreadHandlerPlayersUpdateMessage.h"
#include "../../Global/ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../../CharacterSelect/CharacterListItemObject.h"
#include "../../CharacterSelect/CharacterListItem.h"
#include "../../Global/ClientBlueprintFunctionLibrary.h"
#include "Components/ListView.h"
#include "../../UnrealClient.h"
#include "../../Play/PlayGameMode.h"
#include "../../Play/ClientCharacter.h"

void ThreadHandlerPlayersUpdateMessage::Start()
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

	for (size_t i = 0; i < Message_->Datas.size(); i++)
	{
		std::shared_ptr<PlayerUpdateMessage> PlayerMessage = std::make_shared<PlayerUpdateMessage>();

		PlayerMessage->Data = Message_->Datas[i];

		if (false == PGameMode->IsRegist(PlayerMessage->Data.ObjectIndex))
		{
			FTransform Transform = FTransform(PlayerMessage->Data.Pos);

			AClientCharacter* NewCharacter = World_->SpawnActorDeferred<AClientCharacter>(CharacterSubClass.Get(), Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			
			NewCharacter->FinishSpawning(Transform);
			NewCharacter->SetId(PlayerMessage->Data.ObjectIndex);
			NewCharacter->SetObjectType(EGameObjectType::Player);
			PGameMode->RegistObject(PlayerMessage->Data.ObjectIndex, EGameObjectType::Player, NewCharacter);
			NewCharacter->ObjectInit();
		}

		PGameMode->ObjectPushMessage(PlayerMessage->Data.ObjectIndex, Message_);

	}
}

