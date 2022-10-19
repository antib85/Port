// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameMode.h"
#include "../UnrealClient.h"
#include "../Play/ObjectMessageComponent.h"
#include "../Play/ClientMonster.h"

APlayGameMode::APlayGameMode() 
	: ClientUniqueId_(0)
{
	for (size_t i = 0; i < static_cast<size_t>(EGameObjectType::MAX); i++)
	{
		TypeOfAllObject_.Add(static_cast<EGameObjectType>(i), TArray<AActor*>());
	}
}

TSubclassOf<AClientMonster> APlayGameMode::GetMonsterClass(int _MonsterType)
{
	if (0 > _MonsterType)
	{
		return nullptr;
	}

	if (_MonsterType >= ArrMonsterClass_.Num())
	{
		return nullptr;
	}

	return ArrMonsterClass_[_MonsterType];
}

bool APlayGameMode::IsRegist(int _ObjectID)
{
	return AllObject_.Contains(_ObjectID);
}

bool APlayGameMode::RegistObject(int _ObjectID, EGameObjectType _Type, AActor* _Actor)
{
	if (nullptr == _Actor)
	{
		return false;
	}

	if (true == IsRegist(_ObjectID))
	{
		return false;
	}

	UObjectMessageComponent* Message = _Actor->FindComponentByClass<UObjectMessageComponent>();

	if (nullptr == Message
		|| false == Message->IsValidLowLevel())
	{
		UE_LOG(ClientLog, Error, TEXT("%S(%u) > nullptr UObjectMessageComponent"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	AllObject_.Add(_ObjectID, { _Actor, Message });

	TypeOfAllObject_[_Type].Add(_Actor);

	return true;
}

bool APlayGameMode::ObjectPushMessage(int _ObjectID, std::shared_ptr<GameServerMessage> _Message)
{
	if (nullptr == _Message)
	{
		return false;
	}

	if (false == IsRegist(_ObjectID))
	{
		return false;
	}

	if (nullptr == AllObject_[_ObjectID].Actor
		|| false == AllObject_[_ObjectID].Actor->IsValidLowLevel())
	{
		return false;
	}

	if (nullptr == AllObject_[_ObjectID].Message
		|| false == AllObject_[_ObjectID].Message->IsValidLowLevel())
	{
		return false;
	}

	AllObject_[_ObjectID].Message->Enqueue(_Message);

	return true;
}

bool APlayGameMode::UnRegistObject(int _ObjectID, EGameObjectType _Type, AActor* _Actor)
{
	if (nullptr == _Actor)
	{
		return false;
	}

	if (false == IsRegist(_ObjectID))
	{
		return false;
	}

	if (AllObject_[_ObjectID].Actor != _Actor)
	{
		UE_LOG(ClientLog, Error, TEXT("%S(%u) > nullptr UObjectMessageComponent"), __FUNCTION__, __LINE__);
	}

	AllObject_.Remove(_ObjectID);

	TypeOfAllObject_[_Type].Remove(_Actor);

	return true;
}


int APlayGameMode::GetClientUniqueId() 
{
	return ClientUniqueId_++;
}

TArray<AActor*> APlayGameMode::GetObjectGroup(EGameObjectType _Type) 
{
	return TypeOfAllObject_[_Type];
}

TArray<AClientMonster*> APlayGameMode::GetObjectGroupMonster()
{
	TArray<AClientMonster*> Return;
	for (int i = 0; i < TypeOfAllObject_[EGameObjectType::Monster].Num(); i++)
	{
		Return.Add(Cast<AClientMonster>(TypeOfAllObject_[EGameObjectType::Monster][i]));
	}

	return Return;
}

void APlayGameMode::MonsterCollisionCallBack(const AActor* _Actor, float _Radius, float _Angle, std::function<void(AClientMonster*)> _CallBack)
{
	for (int i = 0; i < TypeOfAllObject_[EGameObjectType::Monster].Num(); i++)
	{
		AClientMonster* Monster = Cast<AClientMonster>(TypeOfAllObject_[EGameObjectType::Monster][i]);

		if (nullptr == Monster)
		{
			continue;
		}

		FVector Dir = Monster->GetActorLocation() - _Actor->GetActorLocation();

		if (Dir.Size() > _Radius)
		{
			continue;
		}

		FVector LookVector = Dir.GetSafeNormal();

		FVector CurVector = _Actor->GetActorForwardVector().GetSafeNormal();

		FVector CorssVector = FVector::CrossProduct(CurVector, LookVector);
		float DotValue = FVector::DotProduct(CurVector, LookVector);

		float ACos = FMath::Acos(DotValue);

		if (CorssVector.Z < 0)
		{
			ACos *= -1.0f;
		}

		float LookAngle = FMath::RadiansToDegrees(ACos);

		if (_Angle * 0.5f < FMath::Abs(LookAngle))
		{
			continue;
		}

		_CallBack(Monster);
	}
}

TArray<AActor*>& APlayGameMode::GetObjectGroupRef(EGameObjectType _Type) 
{
	return TypeOfAllObject_[_Type];
}

AActor* APlayGameMode::GetGameObject(int _ObjectID) 
{
	if (false == AllObject_.Contains(_ObjectID))
	{
		return nullptr;
	}

	return AllObject_.Find(_ObjectID)->Actor;
}

TArray<AActor*> APlayGameMode::CirCleCollisionGroup(const FVector& _Postion, float _Radius, EGameObjectType _Type)
{
	TArray<AActor*>& Group = TypeOfAllObject_[_Type];

	if (0 == Group.Num())
	{
		return TArray<AActor*>();
	}


	FVector CheckPos = _Postion;

	CheckPos.Z = 0.0f;

	TArray<AActor*> Col;

	for (AActor* _Actor : Group)
	{
		FVector Pos = _Actor->GetActorLocation();
		Pos.Z = 0.0f;
		if ((_Actor->GetActorLocation() - _Postion).Size() > _Radius)
		{
			continue;
		}

		Col.Add(_Actor);
	}

	return Col;
}
