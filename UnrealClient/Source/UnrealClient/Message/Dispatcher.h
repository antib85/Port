#pragma once
#include "EngineMinimal.h"
#include <memory>
#include <map>
#include <functional>
#include "Messages.h"
#include "MessageIdEnum.h"
#include "Dispatcher.generated.h"

using  MessageHandler = std::function<void(std::shared_ptr<GameServerMessage>)>;

UCLASS(ClassGroup = (Custom))
class UDispatcher : public UObject
{
	GENERATED_BODY()

private: 
	std::map<unsigned int, MessageHandler> Handlers_;

public: 
	UDispatcher() {}
	~UDispatcher() {}

	bool AddHandler(MessageId Id, const MessageHandler _Handler)
	{
		return AddHandler(static_cast<unsigned int>(Id), _Handler);
	}

	bool AddHandler(unsigned int Id, const MessageHandler _Handler)
	{
		Handlers_.insert(std::make_pair(Id, _Handler));

		return true;
	}

	bool GetHandler(MessageId Id, MessageHandler& _Handler) 
	{
		return GetHandler(static_cast<unsigned int>(Id), _Handler);
	}

	bool GetHandler(uint32_t Id, MessageHandler& _Handler) 
	{
		auto FindData = Handlers_.find(Id);

		if (Handlers_.end() == FindData)
		{
			return false;
		}

		_Handler = FindData->second;

		return true;
	}


public: 
};

