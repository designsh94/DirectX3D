#include "PreCompile.h"
#include "GameEngineDirectory.h"
#include "GameEngineFile.h"
#include "GameEngineDebug.h"
#include "GameEngineString.h"

GameEngineDirectory::GameEngineDirectory()
{
	path_ = std::filesystem::current_path();
}

GameEngineDirectory::GameEngineDirectory(const std::string& _Path)
{
	path_ = _Path;
}

GameEngineDirectory::GameEngineDirectory(const GameEngineDirectory& _other) :
	GameEnginePath(_other)
{
}

GameEngineDirectory::~GameEngineDirectory()
{
}

GameEngineDirectory::GameEngineDirectory(GameEngineDirectory&& _other) noexcept :
	GameEnginePath(_other)
{
}

std::string GameEngineDirectory::DirectoryName() 
{
	return path_.filename().string();
}

std::string GameEngineDirectory::PathToPlusFileName(const std::string& _FileName)
{
	std::filesystem::path NewPath = path_;
	NewPath.append(_FileName);

	return NewPath.string();
}

bool GameEngineDirectory::IsRoot() 
{
	return path_.root_directory() == path_;
}

void GameEngineDirectory::MoveParent()
{
	path_ = path_.parent_path();
}

bool GameEngineDirectory::MoveParent(const std::string& _DirName)
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

bool GameEngineDirectory::MoveChild(const std::string& _DirName)
{
	path_.append(_DirName);

	if (false == IsExist())
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 경로로 이동했습니다.");

		return false;
	}

	return true;
}

std::vector<GameEngineFile> GameEngineDirectory::GetAllFile(const std::string& _filter)
{
	std::string Filter = "";
	if (std::string::npos == _filter.find('.'))
	{
		Filter = ".";
	}

	Filter += _filter;

	GameEngineString::toupper(Filter);

	std::vector<GameEngineFile> Return;

	std::filesystem::directory_iterator DirIter = std::filesystem::directory_iterator(path_);

	for (const std::filesystem::directory_entry& File : DirIter)
	{
		if (true == File.is_directory())
		{
			continue;
		}
		
		std::string Ext = File.path().extension().string();
		GameEngineString::toupper(Ext);

		if (_filter != "*" && Filter != Ext)
		{
			continue;
		}

		Return.push_back(GameEngineFile(File.path()));
	}

	return Return;
}

std::vector<GameEngineDirectory> GameEngineDirectory::GetAllDirectory(const std::string& _filter)
{
	std::string Filter = _filter;
	GameEngineString::toupper(Filter);

	std::vector<GameEngineDirectory> Return;
	std::filesystem::directory_iterator DirIter = std::filesystem::directory_iterator(path_);
	for (const std::filesystem::directory_entry& File : DirIter)
	{
		if (true != File.is_directory())
		{
			continue;
		}

		std::string FileName = File.path().filename().string();
		GameEngineString::toupper(FileName);
		if (std::string::npos == File.path().string().find(Filter))
		{
			continue;
		}

		Return.push_back(GameEngineDirectory(File.path().string()));
	}

	return Return;
}

std::vector<GameEngineDirectory> GameEngineDirectory::GetAllDirectoryRecursive(const std::string& _filter) 
{
	std::string Filter = _filter;
	GameEngineString::toupper(Filter);

	std::vector<GameEngineDirectory> Return;
	Return.push_back(GameEngineDirectory(GetFullPath()));
	std::filesystem::recursive_directory_iterator DirIter = std::filesystem::recursive_directory_iterator(path_);
	for (const std::filesystem::directory_entry& File : DirIter)
	{
		if (false == File.is_directory())
		{
			continue;
		}

		std::string FileName = File.path().filename().string();
		GameEngineString::toupper(FileName);

		if (std::string::npos == File.path().string().find(Filter))
		{
			continue;
		}

		Return.push_back(GameEngineDirectory(File.path().string()));
	}

	return Return;
}
