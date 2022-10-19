#include "PreCompile.h"
#include "GameServerDebug.h"
#include <Windows.h>
#include <iostream>
#include "GameServerIocp.h"

const char* GameServerDebug::TypeText[4] = {"ERROR	: ", "WARNING	: ", "INFO	: ", "LASTERROR	: " };

std::atomic<int> GameServerDebug::LogCount;
GameServerIocp* GameServerDebug::LogIocp = new GameServerIocp();

GameServerDebug::GameServerDebug() 
{

}

GameServerDebug::~GameServerDebug() 
{

}

GameServerDebug::GameServerDebug(GameServerDebug&& _Other) noexcept
{

}

void GameServerDebug::AssertDebugMsg(const std::string& _Msg) 
{
	MessageBoxA(nullptr, _Msg.c_str(), "∞Ê∞Ì√¢", MB_OK);
	GameServerDebug::AssertDebug();
}

void GameServerDebug::AssertDebug() 
{
	assert(false);
}

void GameServerDebug::LogThread(std::shared_ptr<GameServerIocpWorker> _Work)
{
	while (true)
	{
		_Work->Wait();

		if (-1 == _Work->GetNumberOfBytes())
		{
			return;
		}
		++GameServerDebug::LogCount;
		std::unique_ptr<LogJob> LogPtr = std::unique_ptr<LogJob>(_Work->GetConvertCompletionKey<LogJob*>());
		std::cout << TypeText[static_cast<int>(LogPtr->Type_)] << LogPtr->LogText_ << std::endl;
	}
}


void GameServerDebug::Initialize() 
{
	static bool Check = false;
	if (true == Check)
	{
		return;
	}

	Check = true;

	LogIocp->Initialize("LogThread", &GameServerDebug::LogThread, nullptr, nullptr, INFINITE, 1);
}

void GameServerDebug::Destroy() 
{
	LogIocp->Post(-1, 0);
	delete LogIocp;
	Sleep(1);
}

void GameServerDebug::Log(LOGTYPE _Type, const std::string& _Text)
{
	std::unique_ptr<LogJob> LogPtr = std::make_unique<LogJob>();
	LogPtr->Type_ = _Type;
	LogPtr->LogText_ = _Text;
	LogIocp->Post(0, reinterpret_cast<ULONG_PTR>(LogPtr.get()));
	LogPtr.release();
}

void GameServerDebug::LogAssert(LOGTYPE _Type, const std::string& _Text)
{
	Log(_Type, _Text);
	AssertDebugMsg(_Text);
}

void GameServerDebug::LogErrorAssert(const std::string& _Text)
{
	LogAssert(LOGTYPE::LOGTYPE_ERROR, _Text);
}

void GameServerDebug::LogError(const std::string& _Text) 
{
	Log(LOGTYPE::LOGTYPE_ERROR, _Text);
}
void GameServerDebug::LogWarning(const std::string& _Text) 
{
	Log(LOGTYPE::LOGTYPE_WARNING, _Text);
}
void GameServerDebug::LogInfo(const std::string& _Text)
{
	Log(LOGTYPE::LOGTYPE_INFO, _Text);
}

void GameServerDebug::LogLastError(const std::string& _Text)
{
	Log(LOGTYPE::LOGTYPE_LASTERROR, _Text);
}

void GameServerDebug::GetLastErrorPrint() 
{
	DWORD error = WSAGetLastError();
	char* message = nullptr;

	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		nullptr, 
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&message,
		0,
		nullptr);

	if (nullptr != message)
	{
		std::string Text = "Code : ";
		Text += std::to_string(error);
		Text += " Message : ";
		Text += message;

		LogLastError(Text);

		LocalFree(message);
	}
}