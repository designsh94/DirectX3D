#pragma once
#include "GameEnginePath.h"

// 분류 : 파일관리
// 용도 :
// 설명 : 파일의 경로 및 읽기/쓰기/열기 기능을 제공
class GameEngineFile : public GameEnginePath
{
private:
	FILE* fileHandle_;
	std::string OpenMode;

public:		
	GameEngineFile();
	GameEngineFile(const std::filesystem::path& _Path);
	GameEngineFile(const std::string& _Path);
	GameEngineFile(const std::string& _Path, const std::string& _Mode);
	~GameEngineFile();

public:
	GameEngineFile(const GameEngineFile& _other) = delete;
	GameEngineFile(GameEngineFile&& _other) noexcept;

public:
	GameEngineFile& operator=(const GameEngineFile& _other) = delete;
	GameEngineFile& operator=(const GameEngineFile&& _other) = delete;

public:
	uintmax_t GetFileSize();
	std::string FileName();
	std::string GetString();

public:
	void Open(const std::string& _Mode);
	void Close();

public:
	void Write(const void* _Data, size_t _Size);
	void Write(const std::string& _Data);
	void Write(const int& _Data);
	void Write(const size_t& _Data) = delete;

public:
	void Read(void* _Buffer, size_t _BufferSize, size_t _Size);
	void Read(std::string& _Data);
	void Read(int& _Data);

public:
	template<typename DataType>
	void Write(std::vector<DataType>& _Data)
	{
		int Size = static_cast<int>(_Data.size());
		Write(&Size, sizeof(int));

		for (size_t i = 0; i < _Data.size(); i++)
		{
			Write(_Data[i]);
		}
	}

	template<typename DataType>
	void Write(std::list<DataType>& _Data)
	{
		int Size = static_cast<int>(_Data.size());
		Write(&Size, sizeof(int));

		// 템플릿에서 다른 템플릿 사용할때는 형을 잘 모르므로
		// typename 붙여줘야 합니다.
		typename std::list<DataType>::iterator Start = _Data.begin();
		typename std::list<DataType>::iterator End = _Data.end();

		for (; Start != End; ++Start)
		{
			Write(*Start);
		}
	}

	template<typename KeyType, typename DataType>
	void Write(std::map<KeyType, DataType>& _Data)
	{
		int Size = static_cast<int>(_Data.size());
		Write(&Size, sizeof(int));

		typename std::map<KeyType, DataType>::iterator Start = _Data.begin();
		typename std::map<KeyType, DataType>::iterator End = _Data.end();

		for (; Start != End; ++Start)
		{
			Write(Start->first);
			Write(Start->second);
		}
	}

	template<typename DataType>
	void Read(std::vector<DataType>& _Data)
	{
		int Size = 0;
		Read(&Size, sizeof(int), sizeof(int));

		for (int i = 0; i < Size; i++)
		{
			DataType Buffer;
			Read(Buffer);
			_Data.push_back(Buffer);
		}
	}

	template<typename DataType>
	void Read(std::list<DataType>& _Data)
	{
		int Size = 0;
		Read(&Size, sizeof(int), sizeof(int));

		for (int i = 0; i < Size; i++)
		{
			DataType Buffer;
			Read(Buffer);
			_Data.push_back(Buffer);
		}
	}

	template<typename KeyType, typename DataType>
	void Read(std::map<KeyType, DataType>& _Data)
	{
		int Size = 0;
		Read(&Size, sizeof(int), sizeof(int));

		for (int i = 0; i < Size; i++)
		{
			KeyType KeyBuffer;
			Read(KeyBuffer);
			DataType ValueBuffer;
			Read(ValueBuffer);

			_Data.insert(std::map<KeyType, DataType>::value_type(KeyBuffer, ValueBuffer));
		}
	}
};

