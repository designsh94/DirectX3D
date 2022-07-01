#pragma once

// �з� : ��ΰ���
// �뵵 : ��ΰ���
// ���� : GameEngineFile�� ��ü�� �ش� Ŭ������ ��ӹ޾� �ش� ������ ��θ� ����
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

