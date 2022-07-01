#pragma once

// �з� : �����
// �뵵 : �޸𸮰��� �޼��� ��� �α�
// ���� : ��� ����� ���� ����� ���⿡ ��Ƴ��´�.
class GameEngineDebug
{ 
private:

public:
	static void LeakCheckOn();
	static void MsgBoxError(const std::string& _Text);
	static void AssertFalse();
	static void OutPutDebugString(const std::string& _Text);
	static void MsgBox(const std::string& _Ptr);

public:
	static void GetLastErrorPrint()
	{
		DWORD error = GetLastError();
		char* message = nullptr;

		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&message, 0, nullptr);
		if (nullptr != message)
		{
			std::string Text = "Code : ";
			Text += std::to_string(error);
			Text += " Message : ";
			Text += message;

			MsgBoxError(Text);
			LocalFree(message);
		}
	}

private: // member Var

protected:		
	GameEngineDebug();
	~GameEngineDebug();

protected:
	GameEngineDebug(const GameEngineDebug& _other) = delete;
	GameEngineDebug(GameEngineDebug&& _other) noexcept;

private:
	GameEngineDebug& operator=(const GameEngineDebug& _other) = delete;
	GameEngineDebug& operator=(const GameEngineDebug&& _other) = delete;

public: // member Function

};

