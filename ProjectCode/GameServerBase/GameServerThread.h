#pragma once
#include <thread>
#include <tuple>
#include <typeinfo>
#include "GameServerDebug.h"
#include "GameServerNameBase.h"
#include "GameServerTime.h"

class GameServerThread  : public GameServerNameBase
{
private:
	static void ThreadNameSetting(const std::string& _Name);

private:
	static void ThreadFunction(GameServerThread* Thread)
	{
		GameServerThread::ThreadNameSetting(Thread->GetNameCopy());

		if (nullptr != Thread->StartFunction)
		{
			Thread->StartFunction();
		}

		if (nullptr != Thread->MainFunction)
		{
			Thread->MainFunction();
		}

		if (nullptr != Thread->EndFunction)
		{
			Thread->EndFunction();
		}

	}

	static thread_local std::string name_;
	static thread_local unsigned int Order_;
	static thread_local std::map<size_t, const std::type_info*> LocalDataType_;
	static thread_local std::map<size_t, std::vector<char>> Data_;

protected:
	static thread_local GameServerTime Timer_;

public:
	static std::string GetThreadName();

	inline std::thread::id ThreadId()
	{
		return thread_.get_id();
	}

	static inline void SetThreadOrder(unsigned int _Order)
	{
		Order_ = _Order;
	}

	static int GetThreadOrder() 
	{
		return Order_;
	}

	template<typename LocalDataType>
	static LocalDataType* CreateThreadLocalData(size_t _Index)
	{
		if (LocalDataType_.end() == LocalDataType_.find(_Index))
		{
			LocalDataType_[_Index] = nullptr;
		}

		if (nullptr != LocalDataType_[_Index])
		{
			GameServerDebug::AssertDebugMsg("Local Data Is Not Null");
			return nullptr;
		}

		LocalDataType_[_Index] = &typeid(LocalDataType);

		Data_[_Index].resize(sizeof(LocalDataType));

		// return new (&Data_[0]) LocalDataType();

		return reinterpret_cast<LocalDataType*>(&Data_[_Index][0]);
	}

	template<typename LocalDataType>
	static LocalDataType* GetLocalData(size_t _Index)
	{
		if (LocalDataType_[_Index] != &typeid(LocalDataType))
		{
			GameServerDebug::AssertDebugMsg("Local Data Type Is Not Equal");
		}

		return (LocalDataType*)&Data_[_Index][0];
	}

public:
	GameServerThread(std::string _Name, std::function<void()> _MainFunction, std::function<void()> _StartFunction = nullptr, std::function<void()> _EndFunction = nullptr)
	{
		Start(_Name, _MainFunction, _StartFunction , _EndFunction);
	}

	GameServerThread()
		: MainFunction(nullptr)
		, EndFunction(nullptr)
		, StartFunction(nullptr)
	{


	}

	~GameServerThread();

	GameServerThread(const GameServerThread& _Other) = delete;
	GameServerThread(GameServerThread&& _Other) noexcept;

public: 
	void join();
	void Start(std::string _Name, std::function<void()> _MainFunction, std::function<void()> _StartFunction = nullptr, std::function<void()> _EndFunction = nullptr)
	{
		MainFunction = _MainFunction;
		StartFunction = _StartFunction;
		EndFunction = _EndFunction;

		SetName(_Name);
		thread_ = std::thread(&GameServerThread::ThreadFunction, this);
	}

protected:
	GameServerThread& operator=(const GameServerThread& _Other) = delete;
	GameServerThread& operator=(GameServerThread&& _Other) = delete;

private:
	std::thread thread_;

	std::function<void()> StartFunction;
	std::function<void()> MainFunction;
	std::function<void()> EndFunction;



};

