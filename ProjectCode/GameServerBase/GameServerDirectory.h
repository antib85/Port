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
	// C: D:���� Ȯ��
	bool IsRoot();

	// �θ������� �̵�
	void MoveParent();

	// �θ������� _DirName�� ���� ������ �̵�
	bool MoveParent(const std::string& _DirName);

	// �ڽ� ������ _DirName���� �̵�
	bool MoveChild(const std::string& _DirName);

public:
	// �� ������ + �����̸� ��θ� �������ִ� �Լ�
	std::string PathToPlusFileName(const std::string& _FileName) const;

	bool IsExitsFile(const std::string& _FileName) const;

	std::vector<GameServerFile> GetAllFile(const std::string& _filter = "*");
};

