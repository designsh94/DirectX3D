#pragma once
#include "KeyboardEvent.h"
#include <queue>

// 분류 : 
// 용도 : 
// 설명 : 
class KeyboardClass
{
private:
	static KeyboardClass* Inst;

public:
	static KeyboardClass& GetInst()
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

private:	// member Var
	bool autoRepeatKeys = false;
	bool autoRepeatChars = false;
	bool KeyState[256];
	std::queue<KeyboardEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;

public:
	KeyboardClass(); // default constructer 디폴트 생성자
	~KeyboardClass(); // default destructer 디폴트 소멸자

public:
	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty();
	bool CharBufferIsEmpty();
	KeyboardEvent ReadKey();
	unsigned char ReadChar();
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);
	void EnableAutoRepeatKeys();
	void DisableAutoRepeatKeys();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();
	bool IsKeysAutoRepeat();
	bool IsCharsAutoRepeat();
	void DeleteCharBuffer();
};

