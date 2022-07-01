#pragma once
#include "GameEnginePath.h"

// 분류 : 디렉터리(폴더) 관리
// 용도 : 
// 설명 : 해당 파일이 속한 최종디렉터리경로관리 및 디렉터리 이동기능 제공
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

