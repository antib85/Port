
#include "ThreadHandlerCharacterSelectResultMessage.h"
#include "../../Global/ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../../CharacterSelect/CharacterListItemObject.h"
#include "../../CharacterSelect/CharacterListItem.h"
#include "../../Global/ClientBlueprintFunctionLibrary.h"
#include "Components/ListView.h"
#include "../../UnrealClient.h"

void ThreadHandlerCharacterSelectResultMessage::Start()
{
	FString Nick;
	UClientBlueprintFunctionLibrary::UTF8ToFString(Message_->NickName, Nick);

	if (EGameServerCode::OK == Message_->Code)
	{
		for (size_t i = 0; i < Inst_->Characters_.size(); i++)
		{

			if (Inst_->Characters_[i].NickName == Message_->NickName)
			{
				Inst_->SelectCharacter = Inst_->Characters_[i];
				return;
			}
		}

		UE_LOG(ClientLog, Error, TEXT("%S(%u) > Charcter Client Data FAIL"), __FUNCTION__, __LINE__);
		Inst_->LoginProcess_ = false;
		UGameplayStatics::OpenLevel(World_, TEXT("LoginLevel"));
	}
}

