// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginUI.h"
#include "../Global/ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../Global/ClientBlueprintFunctionLibrary.h"
#include "../Message/Messages.h"
#include "../UnrealClient.h"
#include "../Message/ServerToClient.h"
#include "../Message/ServerAndClient.h"
#include "../Message/ClientToServer.h"


void ULoginUI::ServerConnect()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0);
	

	UClientGameInstance* GameInst = Cast<UClientGameInstance>(GetGameInstance());

	if (false == GameInst->ServerConnect(IPString, PORTString))
	{
		ConnectStatus = TEXT("접속 실패");
		return;
	}

	ConnectStatus = TEXT("접속 성공");
}


void ULoginUI::NativeConstruct()
{
	IPString = TEXT("127.0.0.1");
	PORTString = TEXT("30001");
	IDString = TEXT("Test");
	PWString = TEXT("Test");

	ConnectStatus = TEXT("커넥트 필요");
	LoginStatus = TEXT("로그인 필요");

}

void ULoginUI::ServerLogin() 
{

	UClientGameInstance* Inst = Cast<UClientGameInstance>(GetGameInstance());

	if (true == Inst->GetClientMode())
	{
		std::shared_ptr<LoginResultMessage> Message = std::make_shared<LoginResultMessage>();
		Message->Code = EGameServerCode::OK;
		Inst->PushClientMessage(Message);
		return;
	}

	if (true == Inst->LoginProcess_)
	{
		return;
	}

	Inst->Characters_.clear();

	std::string ID;
	std::string PW;
	UClientBlueprintFunctionLibrary::FStringToUTF8(IDString, ID);
	UClientBlueprintFunctionLibrary::FStringToUTF8(PWString, PW);

	LoginMessage NewPacket;
	NewPacket.ID = ID;
	NewPacket.PW = PW;
	GameServerSerializer Sr;
	NewPacket.Serialize(Sr);

	Inst->ID = IDString;

	if (false != Inst->Send(Sr.GetData()))
	{
		Inst->LoginProcess_ = true;
	}
}

void ULoginUI::ResetInfo()
{
	IPString = TEXT("127.0.0.1");
	PORTString = TEXT("30001");
}

void ULoginUI::SetClientMode(bool _Mode)
{
	UClientGameInstance* GameInst = Cast<UClientGameInstance>(GetGameInstance());

	if (nullptr == GameInst)
	{
		return;
	}

	GameInst->SetClientMode(_Mode);
}

void ULoginUI::JoinUser() 
{
	UE_LOG(ClientLog, Error, TEXT("%S(%u) > JOIN BUTTON"), __FUNCTION__, __LINE__);
}