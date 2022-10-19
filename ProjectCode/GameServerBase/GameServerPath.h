#pragma once
#include <filesystem>

class GameServerPath
{
protected:
	std::filesystem::path path_;

public:		
	GameServerPath(); 
	GameServerPath(std::filesystem::path _path);
	~GameServerPath();

public:		
	GameServerPath(const GameServerPath& _other);
	GameServerPath(GameServerPath&& _other) noexcept;

public:		
	GameServerPath& operator=(const GameServerPath& _other) = delete;
	GameServerPath& operator=(const GameServerPath&& _other) = delete; 

public:
	static std::string GetFileName(std::string _Path);
	std::string GetFileName();

	std::string GetFileNameWithOutExtension();

public:	
	bool IsExist();

	std::string GetFullPath();

};

