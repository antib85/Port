#include "ClientPlayCharacter.h"
#include "../UnrealClient.h"
#include "ObjectMessageComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Global/ClientGameInstance.h"
#include "../Play/PlayGameMode.h"
#include "../Play/ClientMonster.h"
#include "../Message/ClientToServer.h"
#include "../Message/ServerToClient.h"
#include "../Message/ServerAndClient.h"
#include "Animation/AnimMontage.h"
#include "ClientAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerInput.h"
#include "DrawDebugHelpers.h"


// Sets default values
AClientPlayCharacter::AClientPlayCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UDPReady = false;
	ServerPost = false;
}

// Called when the game starts or when spawned
void AClientPlayCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayGameMode* GameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (nullptr == GameMode || false == GameMode->IsValidLowLevel())
	{
		return;
	}

	UClientGameInstance* Inst = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (nullptr == Inst || false == Inst->IsValidLowLevel())
	{
		return;
	}

	SetObjectType(EGameObjectType::Player);
	SetId(GameMode->GetUniqueID());
	GameMode->RegistObject(Inst->ObjectIndex, EGameObjectType::Player, this);

	GetClientAnimInstance()->AddEndFunctionBind(std::bind(&AClientPlayCharacter::AnimationEnd, this, std::placeholders::_1));
	GetClientAnimInstance()->AddEndFunctionBind(std::bind(&AClientPlayCharacter::AnimationStart, this, std::placeholders::_1));

	ClientToReadyMessage Msg;

	Msg.ObjectIndex = Inst->ObjectIndex;
	Msg.ThreadIndex = Inst->ThreadIndex;
	Msg.SectionIndex = Inst->SectionIndex;
	GameServerSerializer Sr;
	Msg.Serialize(Sr);
	if (false != Inst->Send(Sr.GetData()))
	{
		Inst->LoginProcess_ = true;
	}

}

void AClientPlayCharacter::AnimationStart(ClientAnimationType _Value)
{
	if (_Value == ClientAnimationType::Attack)
	{

	}
}

void AClientPlayCharacter::AnimationEnd(ClientAnimationType _Value)
{
	if (_Value == ClientAnimationType::Attack)
	{
		GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Idle);
	}
}

void AClientPlayCharacter::SendPlayerUpdatePacket()
{
	UClientGameInstance* Inst = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (nullptr == Inst || false == Inst->IsValidLowLevel())
	{
		return;
	}

	PlayerUpdateMessage UpdateMsg;
	UpdateMsg.Data.Dir = GetActorForwardVector();
	UpdateMsg.Data.Pos = GetActorLocation();
	FQuat RotData = GetActorQuat();
	UpdateMsg.Data.Rot = FVector4(RotData.X, RotData.Y, RotData.Z, RotData.W);
	UpdateMsg.Data.ObjectIndex = Inst->ObjectIndex;
	UpdateMsg.Data.SectionIndex = Inst->SectionIndex;
	UpdateMsg.Data.ThreadIndex = Inst->ThreadIndex;
	UpdateMsg.Data.SetState(GetClientAnimInstance()->GetAnimationType());

	GameServerSerializer Sr;
	UpdateMsg.Serialize(Sr);
	if (false == Inst->SendTo(Sr.GetData()))
	{
		int a = 0;
	}
}

// Called every frame
void AClientPlayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AttTimeCheck -= DeltaTime;

	APlayGameMode* GameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (nullptr == GameMode || false == GameMode->IsValidLowLevel())
	{
		return;
	}

	UClientGameInstance* Inst = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (nullptr == Inst || false == Inst->IsValidLowLevel())
	{
		return;
	}

	while (false == GetMessage()->IsEmpty())
	{
		std::shared_ptr<GameServerMessage> Message = GetMessage()->Dequeue();

		if (MessageId::LevelMove == Message->GetId<MessageId>())
		{
			ServerPost = false;
			LevelMoveReplyMessage MoveReplyMsg;
			MoveReplyMsg.ObjectIndex = Inst->ObjectIndex;
			MoveReplyMsg.SectionIndex = Inst->SectionIndex;
			MoveReplyMsg.ThreadIndex = Inst->ThreadIndex;

			GameServerSerializer Sr;
			MoveReplyMsg.Serialize(Sr);
			if (false == Inst->Send(Sr.GetData()))
			{
				int a = 0;
			}
		}
		else if (MessageId::UDPReadyOK == Message->GetId<MessageId>())
		{
			UDPReady = true;
		}
		else if (MessageId::ObjectDestroy == Message->GetId<MessageId>())
		{
			Destroy();
		}
		else if (MessageId::PlayerUpdate == Message->GetId<MessageId>())
		{
			ServerPost = true;
		}
		else 
		{
			int a = 0;
		}
	}

	if (false == ServerPost)
	{
		return;
	}

	if (false == UDPReady)
	{
		SendPlayerUpdatePacket();
		return;
	}

	if (PrevVector == GetActorLocation())
	{
		SendPlayerUpdatePacket();
		return;
	}

	SendPlayerUpdatePacket();
	PrevVector = GetActorLocation();

}

// Called to bind functionality to input
void AClientPlayCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("ClientPlayer_MoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("ClientPlayer_MoveForward", EKeys::S, -1.f));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ClientPlayer_Move", EKeys::W));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ClientPlayer_Move", EKeys::S));


		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("ClientPlayer_MoveRight", EKeys::D, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("ClientPlayer_MoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ClientPlayer_Move", EKeys::D));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ClientPlayer_Move", EKeys::A));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ClientPlayer_Attack", EKeys::LeftMouseButton));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket0", EKeys::NumPadZero));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket1", EKeys::NumPadOne));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket2", EKeys::NumPadTwo));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket3", EKeys::NumPadThree));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket4", EKeys::NumPadFour));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket5", EKeys::NumPadFive));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket6", EKeys::NumPadSix));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket7", EKeys::NumPadSeven));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket8", EKeys::NumPadEight));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket9", EKeys::NumPadNine));
	}


	PlayerInputComponent->BindAxis("DefaultPawn_MoveForward", this, &AClientPlayCharacter::MoveForward);
	PlayerInputComponent->BindAxis("ClientPlayer_MoveRight", this, &AClientPlayCharacter::MoveRight);
	PlayerInputComponent->BindAction("ClientPlayer_Move", EInputEvent::IE_Pressed, this, &AClientPlayCharacter::MoveStart);
	PlayerInputComponent->BindAction("ClientPlayer_Move", EInputEvent::IE_Released, this, &AClientPlayCharacter::MoveEnd);

	PlayerInputComponent->BindAction("ClientPlayer_Attack", EInputEvent::IE_Released, this, &AClientPlayCharacter::Attack);

	FInputModeGameAndUI InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}



void AClientPlayCharacter::MoveRight(float _Rate) 
{
	if (false == UDPReady)
	{
		return;
	}

	if (GetClientAnimInstance()->GetAnimationType() == ClientAnimationType::Attack)
	{
		return;
	}

	if (0.0f == _Rate)
	{
		return;
	}

	AddControllerYawInput(LookZ(FVector(1.0f, 1.0f, 0.0f).GetSafeNormal() * _Rate, 0.1f));

	AddMovementInput(FVector(1.0f, 1.0f, 0.0f).GetSafeNormal(), _Rate);
	GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Move);
}

void AClientPlayCharacter::MoveForward(float _Rate) 
{
	if (false == UDPReady)
	{
		return;
	}

	if (GetClientAnimInstance()->GetAnimationType() == ClientAnimationType::Attack)
	{
		return;
	}
	if (0.0f == _Rate)
	{
		return;
	}


	AddControllerYawInput(LookZ(FVector(1.0f, -1.0f, 0.0f).GetSafeNormal() * _Rate, 0.1f));

	AddMovementInput(FVector(1.0f, -1.0f, 0.0f).GetSafeNormal(), _Rate);
	GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Move);
}

void AClientPlayCharacter::MoveStart()
{
	if (false == UDPReady)
	{
		return;
	}

	if (GetClientAnimInstance()->GetAnimationType() == ClientAnimationType::Attack)
	{
		return;
	}
	GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Move);
}

void AClientPlayCharacter::MoveEnd()
{
	if (false == UDPReady)
	{
		return;
	}

	if (GetClientAnimInstance()->GetAnimationType() == ClientAnimationType::Attack)
	{
		return;
	}
	GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Idle);
}

void AClientPlayCharacter::Attack() 
{
	if (false == UDPReady)
	{
		return;
	}

	AddControllerYawInput(LookZ(MouseVectorToWorldVector() - GetActorLocation(), 1.0F));
	GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Attack);
	SendPlayerUpdatePacket();
}


FVector AClientPlayCharacter::MouseVectorToWorldVector()
{
	if (false == UDPReady)
	{
		return FVector(TNumericLimits<float>::Max(), TNumericLimits<float>::Max(), TNumericLimits<float>::Max());
	}

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (nullptr == PC && PC->IsValidLowLevel())
	{
		UE_LOG(ClientLog, Error, TEXT("%S(%u) > nullptr Controller"), __FUNCTION__, __LINE__);

		return FVector(TNumericLimits<float>::Max(), TNumericLimits<float>::Max(), TNumericLimits<float>::Max());
	}


	FHitResult TraceHitResult;

	if (false == PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult))
	{
		return FVector(TNumericLimits<float>::Max(), TNumericLimits<float>::Max(), TNumericLimits<float>::Max());
	}
	
	return TraceHitResult.Location;
}
