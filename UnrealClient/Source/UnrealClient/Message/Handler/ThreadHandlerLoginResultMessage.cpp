#include "ThreadHandlerLoginResultMessage.h"
#include "../../Global/ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"


void ThreadHandlerLoginResultMessage::Start()
{
	if (EGameServerCode::OK == Message_->Code)
	{
		return;
	}

	Inst_->LoginProcess_ = false;

}

