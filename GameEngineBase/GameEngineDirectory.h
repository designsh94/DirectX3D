#pragma once
#include "GameEnginePath.h"

// �з� : ���͸�(����) ����
// �뵵 : 
// ���� : �ش� ������ ���� �������͸���ΰ��� �� ���͸� �̵���� ����
class GameEngineFile;
class GameEngineDirectory : public GameEnginePath
{
public:
	GameEngineDirectory();
	GameEngineDirectory(const std::string& Path);
	~GameEngineDirectory();

public:
	GameEngineDirectory(const GameEngineDirectory& _other);
	GameEngineDirectory(GameEngineDirectory&& _other) noexcept;

public:
	GameEngineDirectory& operator=(const GameEngineDirectory& _other) = delete;
	GameEngineDirectory& operator=(const GameEngineDirectory&& _other) = delete;

public:
	GameEngineDirectory& operator/(const std::string& _DirName)
	{
		MoveChild(_DirName);
		return *this;
	}

public:
	std::string DirectoryName();
	std::string PathToPlusFileName(const std::string& _FileName);

public:
	bool IsRoot();
	void MoveParent();
	bool MoveParent(const std::string& _DirName);
	bool MoveChild(const std::string& _DirName);

public:
	std::vector<GameEngineFile> GetAllFile(const std::string& _filter = "*");
	std::vector<GameEngineDirectory> GetAllDirectory(const std::string& _filter = "");
	std::vector<GameEngineDirectory> GetAllDirectoryRecursive(const std::string& _filter = "");
};

