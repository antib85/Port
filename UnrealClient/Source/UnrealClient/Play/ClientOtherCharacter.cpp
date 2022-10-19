#include "ClientOtherCharacter.h"
#include "../UnrealClient.h"
#include "ObjectMessageComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Global/ClientGameInstance.h"
#include "../Play/PlayGameMode.h"
#include "../Global/ClientGameInstance.h"
#include "Animation/AnimMontage.h"
#include "ClientAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "../Message/ServerToClient.h"
#include "../Message/ServerAndClient.h"
#include "../Message/ClientToServer.h"


// Sets default values
AClientOtherCharacter::AClientOtherCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AClientOtherCharacter::BeginPlay()
{
	Super::BeginPlay();


	GetClientAnimInstance()->AddEndFunctionBind(std::bind(&AClientOtherCharacter::AnimationEnd, this, std::placeholders::_1));
}

// Called every frame
void AClientOtherCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UClientGameInstance* GameInst = Cast<UClientGameInstance>(GetGameInstance());

	if (true == GameInst->GetClientMode())
	{
	}

	while (false == GetMessage()->IsEmpty())
	{
		std::shared_ptr<GameServerMessage> Message = GetMessage()->Dequeue();

		if (MessageId::PlayerUpdate == Message->GetId<MessageId>())
		{

			std::shared_ptr<PlayerUpdateMessage> UpdateMessage = std::static_pointer_cast<PlayerUpdateMessage>(Message);

			GetClientAnimInstance()->ChangeAnimation(UpdateMessage->Data.GetState<ClientAnimationType>());

			FVector4 Rot = UpdateMessage->Data.Rot;
			FQuat RotData = FQuat(Rot.X, Rot.Y, Rot.Z, Rot.W);
			SetActorRotation(RotData);

			SetActorLocation(UpdateMessage->Data.Pos);

		}
		else if (MessageId::ObjectDestroy == Message->GetId<MessageId>())
		{
			Destroy();
		}
		else if (MessageId::LevelMove == Message->GetId<MessageId>())
		{
			Destroy();
		}

	}
}


void AClientOtherCharacter::AnimationEnd(ClientAnimationType _Value)
{
	if (_Value == ClientAnimationType::Attack)
	{
		UClientGameInstance* Inst = Cast<UClientGameInstance>(GetGameInstance());
		GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Idle);
	}
}
