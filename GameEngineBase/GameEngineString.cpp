#include "PreCompile.h"
#include "GameEngineString.h"
#include "GameEngineDebug.h"

std::string GameEngineString::toupper(const std::string& _Text)
{
	std::string Text = _Text;
	std::transform(Text.begin(), Text.end(), Text.begin(), ::toupper);

	return Text;
}

void GameEngineString::AnsiToUnicode(const std::string& _Text, std::wstring& _Out)
{
	int Size = MultiByteToWideChar(CP_ACP, 0, _Text.c_str(), static_cast<int>(_Text.size()), nullptr, 0);

	if (Size == 0)
	{
		GameEngineDebug::MsgBoxError("스트링 변환에 실패했습니다.");
		return;
	}

	_Out.resize(Size);

	Size = MultiByteToWideChar(CP_ACP, 0, _Text.c_str(), static_cast<int>(_Text.size()), &_Out[0], Size);

	if (Size == 0)
	{
		GameEngineDebug::MsgBoxError("스트링 변환에 실패했습니다.");
		return;
	}
}

void GameEngineString::UniCodeToUTF8(const std::wstring& _Text, std::string& _Out) 
{
	int Size = WideCharToMultiByte(CP_UTF8, 0, _Text.c_str(), static_cast<int>(_Text.size()), nullptr, 0, nullptr, nullptr);

	if (Size == 0)
	{
		GameEngineDebug::MsgBoxError("스트링 변환에 실패했습니다.");
		return;
	}

	_Out.resize(Size);

	Size = WideCharToMultiByte(CP_UTF8, 0, _Text.c_str(), static_cast<int>(_Text.size()), &_Out[0], static_cast<int>(_Out.size()), nullptr, nullptr);

	if (Size == 0)
	{
		GameEngineDebug::MsgBoxError("스트링 변환에 실패했습니다.");
		return;
	}
}

bool GameEngineString::UniCodeToAnsi(const std::wstring& _Unicode, std::string& _AnsiString)
{
	int32_t Len = WideCharToMultiByte(CP_ACP, 0, _Unicode.c_str(), static_cast<int32_t>(_Unicode.size()), nullptr, 0, nullptr, nullptr);
	if (0 >= Len)
	{
		GameEngineDebug::GetLastErrorPrint();
		return false;
	}

	_AnsiString.resize(Len);

	Len = WideCharToMultiByte(CP_ACP, 0, _Unicode.c_str(), static_cast<int32_t>(_Unicode.size()), &_AnsiString[0], static_cast<int32_t>(_AnsiString.size()), nullptr, nullptr);

	return true;
}

void GameEngineString::AnsiToUTF8(const std::string& _Text, std::string& _Out)
{
	std::wstring WString;

	AnsiToUnicode(_Text, WString);
	UniCodeToUTF8(WString, _Out);
}

std::string GameEngineString::AnsiToUTF8Return(const std::string& _Text) 
{
	std::wstring WString;
	std::string Result;

	AnsiToUnicode(_Text, WString);
	UniCodeToUTF8(WString, Result);

	return Result;
}

std::wstring GameEngineString::StringToWStringReturn(const std::string& _Text)
{
	std::wstring Return;

	int Size = MultiByteToWideChar(CP_ACP, 0, _Text.c_str(), static_cast<int>(_Text.size()), nullptr, 0);

	if (Size == 0)
	{
		GameEngineDebug::MsgBoxError("스트링 변환에 실패했습니다.");
		return L"";
	}

	Return.resize(Size);

	Size = MultiByteToWideChar(CP_ACP, 0, _Text.c_str(), static_cast<int>(_Text.size()), &Return[0], Size);

	if (Size == 0)
	{
		GameEngineDebug::MsgBoxError("스트링 변환에 실패했습니다.");
		return L"";
	}

	return Return;
}

bool GameEngineString::UTF8ToUniCode(const std::string& _UTF8, std::wstring& _UnicodeResult)
{
	_UnicodeResult.clear();

	int32_t Len = MultiByteToWideChar(CP_UTF8, 0, _UTF8.c_str(), static_cast<int32_t>(_UTF8.size()), nullptr, 0);
	if (0 >= Len)
	{
		GameEngineDebug::GetLastErrorPrint();
		return false;
	}

	_UnicodeResult.resize(Len);

	Len = MultiByteToWideChar(CP_UTF8, 0, _UTF8.c_str(), static_cast<int32_t>(_UTF8.size()), &_UnicodeResult[0], Len);
	if (0 >= Len)
	{
		GameEngineDebug::GetLastErrorPrint();
		return false;
	}

	return true;
}

bool GameEngineString::UTF8ToAnsi(const std::string& _UTF8, std::string& _Ansi)
{
	std::wstring UniCode;
	if (false == UTF8ToUniCode(_UTF8, UniCode))
	{
		GameEngineDebug::MsgBoxError("UTF8 => Ansi ConvertError");
	}

	if (false == UniCodeToAnsi(UniCode, _Ansi))
	{
		GameEngineDebug::MsgBoxError("UniCode => Ansi ConvertError");
	}

	return true;
}

std::string GameEngineString::UTF8ToAnsiReturn(const std::string& _UTF8)
{
	std::string Return;
	UTF8ToAnsi(_UTF8, Return);

	return Return;
}

GameEngineString::GameEngineString()
{
}

GameEngineString::~GameEngineString()
{
}

GameEngineString::GameEngineString(GameEngineString&& _other) noexcept
{
}
