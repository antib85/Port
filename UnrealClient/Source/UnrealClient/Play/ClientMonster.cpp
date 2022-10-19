// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientMonster.h"
#include "ObjectMessageComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Global/ClientGameInstance.h"
#include "../Play/PlayGameMode.h"
#include "../Global/ClientGameInstance.h"
#include "Animation/AnimMontage.h"
#include "ClientAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "../UnrealClient.h"

#include "Kismet/KismetMathLibrary.h"

void AClientMonster::Att(float _DelataTime)
{
	GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Attack);
}

void AClientMonster::AnimationStart(ClientAnimationType _Value) 
{
}

void AClientMonster::AnimationEnd(ClientAnimationType _Value) 
{
	if (_Value == ClientAnimationType::Attack)
	{
		GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Idle);
	}
}

void AClientMonster::BeginPlay() 
{
	Super::BeginPlay();

	State_ = EMonsterState::NONE;

	GetClientAnimInstance()->AddEndFunctionBind(std::bind(&AClientMonster::AnimationEnd, this, std::placeholders::_1));

	GetClientAnimInstance()->AddStartFunctionBind(std::bind(&AClientMonster::AnimationStart, this, std::placeholders::_1));
	Ratio = 0.0f;
}

void AClientMonster::ObjectInit() 
{


	CurrentUpdateData.Pos = GetActorLocation();

	ClientObjectInit();
}

void AClientMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UClientGameInstance* GameInst = Cast<UClientGameInstance>(GetGameInstance());

	while (false == GetMessage()->IsEmpty())
	{
		std::shared_ptr<GameServerMessage> Message = GetMessage()->Dequeue();

		if (MessageId::MonsterUpdate == Message->GetId<MessageId>())
		{
			std::shared_ptr<MonsterUpdateMessage> UpdateMessage = std::static_pointer_cast<MonsterUpdateMessage>(Message);

			if (nullptr == UpdateMessage)
			{
				continue;
			}

			State_ = static_cast<EMonsterState>(UpdateMessage->Data.State);

			switch (State_)
			{
			case EMonsterState::NONE:
				break;
			case EMonsterState::MState_Idle:
				GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Idle);
				break;
			case EMonsterState::MState_Trace:
				GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Move);
				break;
			case EMonsterState::MState_Att:
				GetClientAnimInstance()->ChangeAnimation(ClientAnimationType::Attack);
				break;
			case EMonsterState::MAX:
				break;
			default:
				break;
			}

			UpDataData.Add(UpdateMessage->Data);

			LookTargetZ(UpdateMessage->Data.Dir);
			SetActorLocation(UpdateMessage->Data.Pos, false, nullptr, ETeleportType::ResetPhysics);
		}
		else if (MessageId::ObjectDestroy == Message->GetId<MessageId>())
		{
			Destroy();
		}
	}
}



void AClientMonster::SetClientDamage(float _Damage)
{
	UClientGameInstance* Inst = Cast<UClientGameInstance>(GetGameInstance());

	Info.Hp -= _Damage;

	if (false == Inst->GetClientMode())
	{
		return;
	}

	if (0 >= Info.Hp)
	{
#include "../Message/ServerToClient.h"
		std::shared_ptr<ObjectDestroyMessage> Message = std::make_shared<ObjectDestroyMessage>();
		Message->ObjectID = GetObjectId();
		Inst->PushClientMessage(Message);
	}
}


void AClientMonster::ClientUpdate(float _DelataTime)
{
}


void AClientMonster::ClientObjectInit() 
{
}