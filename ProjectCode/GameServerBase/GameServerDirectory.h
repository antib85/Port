#pragma once
#include "GameServerPath.h"

class GameServerFile;
class GameServerDirectory : public GameServerPath
{
public:
	GameServerDirectory(); 
	~GameServerDirectory(); 

public:
	GameServerDirectory(const GameServerDirectory& _other); 
	GameServerDirectory(GameServerDirectory&& _other) noexcept;

public:
	GameServerDirectory& operator=(const GameServerDirectory& _other) = delete;
	GameServerDirectory& operator=(const GameServerDirectory&& _other) = delete;

public:
	std::string DirectoryName();

public:
	// C: D:인지 확인
	bool IsRoot();

	// 부모폴더로 이동
	void MoveParent();

	// 부모폴더중 _DirName을 가진 폴더로 이동
	bool MoveParent(const std::string& _DirName);

	// 자식 폴더중 _DirName으로 이동
	bool MoveChild(const std::string& _DirName);

public:
	// 내 폴더에 + 파일이름 경로를 리턴해주는 함수
	std::string PathToPlusFileName(const std::string& _FileName) const;

	bool IsExitsFile(const std::string& _FileName) const;

	std::vector<GameServerFile> GetAllFile(const std::string& _filter = "*");
};

