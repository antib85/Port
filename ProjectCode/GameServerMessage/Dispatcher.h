#pragma once
#include <memory>
#include <functional>
#include "Messages.h"
#include "GameServerNet/TCPSession.h"

template<class OWNER>
using  MessageHandler = std::function<void(std::shared_ptr<OWNER>, std::shared_ptr<GameServerMessage>)>;

template<class OWNER>
class Dispatcher final
{
private: 
	std::unordered_map<uint32_t, MessageHandler<OWNER>> Handlers_;

public:
	Dispatcher() {}
	~Dispatcher() {}

	bool AddHandler(uint32_t Id, const MessageHandler<OWNER> Handler)	{
		auto Result = Handlers_.insert(std::make_pair(Id, Handler));

		return Result.second;
	}

	bool GetHandler(uint32_t Id, MessageHandler<OWNER>& _Handler) 
	{
		auto Iter = Handlers_.find(Id);

		if (Iter == Handlers_.end())
		{
			return false;
		}

		_Handler = Iter->second;

		return true;
	}


	Dispatcher(const Dispatcher& _Other) = delete;
	Dispatcher(Dispatcher&& _Other) noexcept = delete; 

protected:
	Dispatcher& operator=(const Dispatcher& _Other) = delete;
	Dispatcher& operator=(Dispatcher&& _Other) = delete;
};

