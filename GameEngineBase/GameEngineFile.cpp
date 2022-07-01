#include "PreCompile.h"
#include "GameEngineFile.h"
#include "GameEngineDebug.h"

GameEngineFile::GameEngineFile() :
	OpenMode(""), 
	fileHandle_(nullptr)
{
}

GameEngineFile::GameEngineFile(const std::filesystem::path& _Path)  :
	GameEnginePath(_Path), 
	fileHandle_(nullptr)
{

}

GameEngineFile::GameEngineFile(const std::string& _Path) :
	fileHandle_(nullptr)
{
	path_ = _Path;
	if (false == IsExist())
	{
		GameEngineDebug::AssertFalse();
	}
}

GameEngineFile::GameEngineFile(const std::string& _Path, const std::string& _Mode)
{
	path_ = _Path;

	Open(_Mode);
	if (false == IsExist())
	{
		GameEngineDebug::AssertFalse();
	}
}

GameEngineFile::~GameEngineFile()
{
	Close();
}

GameEngineFile::GameEngineFile(GameEngineFile&& _other) noexcept :
	GameEnginePath(_other),
	fileHandle_(nullptr)
{
}

uintmax_t GameEngineFile::GetFileSize()
{
	return std::filesystem::file_size(path_);
}

std::string GameEngineFile::FileName()
{
	return path_.filename().string();
}

std::string GameEngineFile::GetString()
{
	std::string AllString = std::string();
	AllString.resize(GetFileSize());
	Read(&AllString[0], AllString.size(), AllString.size());

	return AllString;
}

void GameEngineFile::Open(const std::string& _Mode) 
{
	OpenMode = _Mode;
	fopen_s(&fileHandle_, path_.string().c_str(), _Mode.c_str());
	if (nullptr == fileHandle_)
	{
		GameEngineDebug::AssertFalse();
	}
}

void GameEngineFile::Close() 
{
	if (nullptr != fileHandle_)
	{
		fclose(fileHandle_);
		fileHandle_ = nullptr;
	}
}

void GameEngineFile::Write(const void* _Data, size_t _Size) 
{
	if (OpenMode[0] != 'w')
	{
		GameEngineDebug::AssertFalse();
		return;
	}

	if (nullptr == fileHandle_)
	{
		GameEngineDebug::AssertFalse();
		return;
	}

	fwrite(_Data, _Size, 1, fileHandle_);
}

void GameEngineFile::Write(const std::string& _Data)
{
	int Size = static_cast<int>(_Data.size());

	Write(&Size, sizeof(int));
	Write(_Data.c_str(), _Data.size());
}

void GameEngineFile::Write(const int& _Data)
{
	Write(&_Data, sizeof(int));
}

void GameEngineFile::Read(void* _Buffer, size_t _BufferSize, size_t _DataSize)
{
	if (OpenMode[0] != 'r')
	{
		GameEngineDebug::AssertFalse();
		return;
	}

	if (nullptr == fileHandle_)
	{
		GameEngineDebug::AssertFalse();
		return;
	}

	fread_s(_Buffer, _BufferSize, _DataSize, 1, fileHandle_);
}

void GameEngineFile::Read(std::string& _Data)
{
	int Size = 0;
	Read(&Size, sizeof(int), sizeof(int));

	_Data.resize(Size);
	Read(&_Data[0], Size, Size);
}

void GameEngineFile::Read(int& _Data)
{
	Read(&_Data, sizeof(int), sizeof(int));
}
