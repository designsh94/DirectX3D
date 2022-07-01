#pragma once

// 분류 : 경로관리
// 용도 : 경로관리
// 설명 : GameEngineFile의 객체가 해당 클래스를 상속받아 해당 파일의 경로를 관리
class GameEnginePath
{
protected:
	std::filesystem::path path_;

public:		
	GameEnginePath();
	GameEnginePath(std::filesystem::path _path);
	~GameEnginePath();

public:
	GameEnginePath(const GameEnginePath& _other);
	GameEnginePath(GameEnginePath&& _other) noexcept;

public:
	GameEnginePath& operator=(const GameEnginePath& _other) = delete;
	GameEnginePath& operator=(const GameEnginePath&& _other) = delete;

public:
	bool IsExist();
	std::string GetFullPath();

public:
	static std::string GetFileName(std::string _Path);
	std::string GetFileName();
	std::string GetFileNameWithOutExtension();
	std::string GetExtension();
};

