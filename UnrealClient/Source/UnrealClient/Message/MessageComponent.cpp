// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageComponent.h"
#include <Memory>
#include <functional>
#include "Handler/HandlerHeader.h"
#include "../Global/ClientGameInstance.h"


// Sets default values for this component's properties
UMessageComponent::UMessageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UMessageComponent::BeginPlay()
{
	Super::BeginPlay();

	UClientGameInstance* Inst = Cast<UClientGameInstance>(GetOwner()->GetGameInstance());

	Dis = NewObject<UDispatcher>();

	CheckHandler(*Dis,Inst, GetWorld());

	// ...
	
}


// Called every frame
void UMessageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (false == GetOwner()->IsValidLowLevel())
	{
		return;
	}

	UClientGameInstance* Inst = Cast<UClientGameInstance>(GetOwner()->GetGameInstance());

	while (false == Inst->IsEmptyMessage())
	{
		std::shared_ptr<GameServerMessage> Message = Inst->PopMessage();

		if (nullptr == Message)
		{
			continue;
		}

		MessageHandler MsgHandler;

		MessageId Id = Message->GetId<MessageId>();

		Dis->GetHandler(Id, MsgHandler);

		if (nullptr == MsgHandler)
		{
			return;
		}

		MsgHandler(Message);
	}
}

