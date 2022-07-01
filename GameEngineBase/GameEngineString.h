#pragma once

// �з� : ����
// �뵵 : ���ڿ����� ����
// ���� : ���ڿ� ��/�ҹ��� ��ȯ �� ���ڿ� ��� �ʿ��� ��� ����
class GameEngineString
{
public:
	static std::string toupper(const std::string& _Text);
	static void AnsiToUnicode(const std::string& Text, std::wstring& _Out);
	static void UniCodeToUTF8(const std::wstring& _Text, std::string& _Out);
	static bool UniCodeToAnsi(const std::wstring& _Unicode, std::string& _Ansi);
	static void AnsiToUTF8(const std::string& Text, std::string& _Out);
	static bool UTF8ToAnsi(const std::string& _UTF8, std::string& _Ansi);
	static std::string UTF8ToAnsiReturn(const std::string& _UTF8);
	static bool UTF8ToUniCode(const std::string& _UTF8, std::wstring& _Unicode);
	static std::string AnsiToUTF8Return(const std::string& Text);
	static std::wstring StringToWStringReturn(const std::string& Text);

private:
	GameEngineString();
	virtual ~GameEngineString();

protected:
	GameEngineString(const GameEngineString& _other) = delete;
	GameEngineString(GameEngineString&& _other) noexcept;

private:
	GameEngineString& operator=(const GameEngineString& _other) = delete;
	GameEngineString& operator=(const GameEngineString&& _other) = delete;

public:
};

