#include "ThreadHandlerMonsterUpdateMessage.h"
#include "Kismet/GameplayStatics.h"
#include "../../Global/ClientGameInstance.h"
#include "../../Play/PlayGameMode.h"
#include "../../Play/ChatWindow.h"
#include "../../UnrealClient.h"
#include "../../Play/ClientMonster.h"
#include "../../Global/ClientBlueprintFunctionLibrary.h"
#include "../../Play/ObjectMessageComponent.h"



void ThreadHandlerMonsterUpdateMessage::Start()
{
	APlayGameMode* PGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(World_));

	if (nullptr == PGameMode
		|| false == PGameMode->IsValidLowLevel())
	{
		UE_LOG(ClientLog, Error, TEXT("%S(%u) > GameMode Is nullptr"), __FUNCTION__, __LINE__);
		return;
	}

	TSubclassOf<AClientMonster> Monster = PGameMode->GetMonsterClass(0);

	if (nullptr == Monster
		|| false == Monster->IsValidLowLevel())
	{
		UE_LOG(ClientLog, Error, TEXT("%S(%u) > Monster SubClass Is nullptr"), __FUNCTION__, __LINE__);
		return;
	}

	if (false == PGameMode->IsRegist(Message_->Data.ObjectIndex))
	{
		FTransform Transform = FTransform(Message_->Data.Pos);

		AClientMonster* NewMonster = World_->SpawnActorDeferred<AClientMonster>(Monster.Get(), Transform);

		NewMonster->FinishSpawning(Transform);
		NewMonster->SetId(Message_->Data.ObjectIndex);
		NewMonster->SetObjectType(EGameObjectType::Monster);
		PGameMode->RegistObject(Message_->Data.ObjectIndex, EGameObjectType::Monster, NewMonster);
		PGameMode->ObjectPushMessage(Message_->Data.ObjectIndex, Message_);
		NewMonster->ObjectInit();
		return;
	}

	PGameMode->ObjectPushMessage(Message_->Data.ObjectIndex, Message_);
}

