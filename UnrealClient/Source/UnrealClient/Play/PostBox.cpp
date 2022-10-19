// Fill out your copyright notice in the Description page of Project Settings.


#include "PostBox.h"
#include "../Global/ClientGameInstance.h"
#include "../Message/Messages.h"
#include "../Message/ServerToClient.h"
#include "../Message/ServerAndClient.h"
#include "../Message/ClientToServer.h"
#include "../Global/ClientBlueprintFunctionLibrary.h"
#include "../UnrealClient.h"

void UPostBox::PostNickName(FString _NickName, ETextCommit::Type _Type)
{
	UClientGameInstance* Inst = Cast<UClientGameInstance>(GetGameInstance());

	if (ETextCommit::Type::OnUserMovedFocus != _Type)
	{
		return;
	}

	ToNickName_ = _NickName;
}

void UPostBox::PostLetters(FString _Text, ETextCommit::Type _Type)
{
	UClientGameInstance* Inst = Cast<UClientGameInstance>(GetGameInstance());

	if (ETextCommit::Type::OnUserMovedFocus != _Type)
	{
		return;
	}

	Letters_ = _Text;
}

void UPostBox::PostSend()
{
	UClientGameInstance* Inst = Cast<UClientGameInstance>(GetGameInstance());

	PostLetterMessage PostMessage;

	std::string NickName;
	std::string Message;
	std::string FromNickName;
	UClientBlueprintFunctionLibrary::FStringToUTF8(ToNickName_, NickName);
	UClientBlueprintFunctionLibrary::FStringToUTF8(Letters_, Message);
	UClientBlueprintFunctionLibrary::FStringToUTF8(Inst->NickName, FromNickName);

	PostMessage.ToNickName = NickName;
	PostMessage.Letters = Message;
	PostMessage.FromNickName = FromNickName;

	GameServerSerializer Sr;
	PostMessage.Serialize(Sr);

	Inst->Send(Sr.GetData());

	ToNickName_ = nullptr;
	Letters_ = nullptr;
}

void UPostBox::NativeConstruct()
{
	Super::NativeConstruct();


}
