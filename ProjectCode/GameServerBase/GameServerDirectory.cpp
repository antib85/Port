#include "PreCompile.h"
#include "GameServerDirectory.h"
#include "GameServerFile.h"
#include "GameServerDebug.h"
#include "GameServerString.h"
#include <filesystem>

GameServerDirectory::GameServerDirectory()
{
	path_ = std::filesystem::current_path();
}

GameServerDirectory::GameServerDirectory(const GameServerDirectory& _other)
	: GameServerPath(_other)
{

}

GameServerDirectory::~GameServerDirectory()
{
}

GameServerDirectory::GameServerDirectory(GameServerDirectory&& _other) noexcept
{
}

std::string GameServerDirectory::DirectoryName() 
{
	return path_.filename().string();
}

void GameServerDirectory::MoveParent() 
{
	path_ = path_.parent_path();
}

bool GameServerDirectory::IsRoot() 
{
	return path_.root_directory() == path_;
}

bool GameServerDirectory::MoveParent(const std::string& _DirName)
{
	while (false == IsRoot())
	{
		if (path_.filename().string() == _DirName)
		{
			return true;
		}

		MoveParent();
	}
	return false;
}


bool GameServerDirectory::MoveChild(const std::string& _DirName)
{
	path_.append(_DirName);

	if (false == IsExist())
	{
		GameServerDebug::AssertDebugMsg("존재하지 않는 경로로 이동했습니다.");
		return false;
	}

	return true;
}

std::string GameServerDirectory::PathToPlusFileName(const std::string& _FileName) const
{
	std::filesystem::path NewPath = path_;
	NewPath.append(_FileName);
	return NewPath.string();
}

std::vector<GameServerFile> GameServerDirectory::GetAllFile(const std::string& _filter /*= "*"*/)
{
	std::string Filter = "";
	if (std::string::npos == _filter.find('.'))
	{
		Filter = ".";
	}

	Filter += _filter;

	GameServerString::ToUpper(Filter);

	std::vector<GameServerFile> Return;

	std::filesystem::directory_iterator DirIter = std::filesystem::directory_iterator(path_);

	for (const std::filesystem::directory_entry& File : DirIter)
	{
		std::string Ext = File.path().extension().string();
		GameServerString::ToUpper(Ext);

		if (_filter != "*" && Filter != Ext)
		{
			continue;
		}

		Return.push_back(GameServerFile(File.path()));

	}

	
	return Return;
}

bool GameServerDirectory::IsExitsFile(const std::string& _FileName) const
{
	GameServerFile CheckFile = GameServerFile(PathToPlusFileName(_FileName));
	return CheckFile.IsExist();
}