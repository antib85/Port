#include "PreCompile.h"
#include "GameServerString.h"
#include "GameServerDebug.h"
#include <sstream>

GameServerString::GameServerString() 
{

}

GameServerString::~GameServerString() 
{

}

GameServerString::GameServerString(GameServerString&& _Other) noexcept
{

}

bool GameServerString::UTF8ToUniCode(const std::string& _UTF8, std::wstring& _UnicodeResult)
{
	_UnicodeResult.clear();

	int32_t Len = MultiByteToWideChar(
		CP_UTF8, 
		0, 
		_UTF8.c_str(), 
		static_cast<int32_t>(_UTF8.size()),
		nullptr,
		0
	);

	if (0 >= Len)
	{
		GameServerDebug::GetLastErrorPrint();
		return false;
	}

	_UnicodeResult.resize(Len);

	Len = MultiByteToWideChar(
		CP_UTF8,
		0,
		_UTF8.c_str(),
		static_cast<int32_t>(_UTF8.size()),
		&_UnicodeResult[0],
		Len
	);

	if (0 >= Len)
	{
		GameServerDebug::GetLastErrorPrint();
		return false;
	}

	return true;
}


bool GameServerString::UniCodeToAnsi(const std::wstring& _Unicode, std::string& _AnsiString)
{
	int32_t Len = WideCharToMultiByte(
		CP_ACP,
		0,
		_Unicode.c_str(),
		static_cast<int32_t>(_Unicode.size()),
		nullptr,
		0,
		nullptr,
		nullptr
	);


	if (0 >= Len)
	{
		GameServerDebug::GetLastErrorPrint();
		return false;
	}

	_AnsiString.resize(Len);

	Len = WideCharToMultiByte(
		CP_ACP,
		0,
		_Unicode.c_str(),
		static_cast<int32_t>(_Unicode.size()),
		&_AnsiString[0],
		static_cast<int32_t>(_AnsiString.size()),
		nullptr,
		nullptr
	);

	return true;
}


bool GameServerString::AnsiToUnicode(const std::string& _Ansi, std::wstring& _Unicode)
{
	int32_t Len = MultiByteToWideChar(
		CP_ACP,
		0,
		_Ansi.c_str(),
		static_cast<int32_t>(_Ansi.size()),
		nullptr,
		0
	);


	if (0 >= Len)
	{
		GameServerDebug::GetLastErrorPrint();
		return false;
	}

	_Unicode.resize(Len);

	Len = MultiByteToWideChar(
		CP_ACP,
		0,
		_Ansi.c_str(),
		static_cast<int32_t>(_Ansi.size()),
		&_Unicode[0],
		static_cast<int32_t>(_Unicode.size())
	);

	return true;
}


bool GameServerString::UniCodeToUTF8(const std::wstring& _Unicode, std::string& _UTF8) {

	int32_t Len = WideCharToMultiByte(
		CP_UTF8,
		0,
		_Unicode.c_str(),
		static_cast<int32_t>(_Unicode.size()),
		nullptr,
		0,
		nullptr,
		nullptr
	);


	if (0 >= Len)
	{
		GameServerDebug::GetLastErrorPrint();
		return false;
	}

	_UTF8.resize(Len);

	Len = WideCharToMultiByte(
		CP_UTF8,
		0,
		_Unicode.c_str(),
		static_cast<int32_t>(_Unicode.size()),
		&_UTF8[0],
		static_cast<int32_t>(_UTF8.size()),
		nullptr,
		nullptr
	);

	return true;
}

std::string GameServerString::UTF8ToAnsi(const std::string& _UTF8)
{
	std::string Ansi;
	std::wstring UniCode;
	if (false == UTF8ToUniCode(_UTF8, UniCode))
	{
		GameServerDebug::AssertDebugMsg("UTF8 => Ansi ConvertError");
	}
	if (false == UniCodeToAnsi(UniCode, Ansi))
	{
		GameServerDebug::AssertDebugMsg("UniCode => Ansi ConvertError");
	}

	return Ansi;
}

bool GameServerString::UTF8ToAnsi(const std::string& _UTF8, std::string& _Ansi) 
{
	std::wstring UniCode;
	if (false == UTF8ToUniCode(_UTF8, UniCode))
	{
		GameServerDebug::AssertDebugMsg("UTF8 => Ansi ConvertError");
	}
	if(false == UniCodeToAnsi(UniCode, _Ansi))
	{
		GameServerDebug::AssertDebugMsg("UniCode => Ansi ConvertError");
	}

	return true;
}

bool GameServerString::AnsiToUTF8(const std::string& _Ansi, std::string& _UTF8)
{
	std::wstring UniCode;
	if (false == AnsiToUnicode(_Ansi, UniCode))
	{
		GameServerDebug::AssertDebugMsg("UTF8 => Ansi ConvertError");
	}
	if (false == UniCodeToUTF8(UniCode, _UTF8))
	{
		GameServerDebug::AssertDebugMsg("UniCode => Ansi ConvertError");
	}

	return true;
}

std::vector<std::string> GameServerString::split(const std::string& input, char delimiter) 
{
	std::vector<std::string> answer;
	std::stringstream ss(input);
	std::string temp;

	while (getline(ss, temp, delimiter)) 
	{
		if (temp == "")
		{
			continue;
		}
		answer.push_back(temp);
	}

	return answer;
}

void GameServerString::Replace(std::string& _Text, const std::string& _Prev, const std::string& _Next, int _Count /*= 1*/)
{
	int cnt = 0;
	while (_Count > cnt++)
	{
		size_t startIdx = _Text.find(_Prev);
		if (startIdx == std::string::npos)
			return;
		_Text.replace(startIdx, _Prev.length(), _Next);
	}
	return;
}

void GameServerString::Insert(std::string& _Text, int _Index, const std::string& _InsertText)
{
	if (_Text.length() < _Index || _Index < 0)
		return;
	_Text.insert(_Index, _InsertText);
}

void GameServerString::TrimRemove(std::string& _Text)
{
	Replace(_Text, " ", "", static_cast<int>(_Text.length()));
}

void GameServerString::ToUpper(std::string& _Text)
{
	std::transform(_Text.begin(), _Text.end(), _Text.begin(), [](const char& c) { return std::toupper(c); });
}

void GameServerString::Remove(std::string& _Text, const std::string& _DeleteText)
{
	Replace(_Text, _DeleteText, "", static_cast<int>(_Text.length()));
}

void GameServerString::ClearText(std::string& _Text)
{
	Replace(_Text, "\t", "", static_cast<int>(_Text.length()));
	Replace(_Text, "\n", "", static_cast<int>(_Text.length()));
}