#include "PreCompile.h"
#include "KeyboardClass.h"

KeyboardClass* KeyboardClass::Inst = new KeyboardClass();

KeyboardClass::KeyboardClass()
{
	for (int i = 0; i < 256; ++i)
	{
		KeyState[i] = false;
	}
}

KeyboardClass::~KeyboardClass()
{
	// 키 버퍼 소멸
	while (!keyBuffer.empty())
	{
		keyBuffer.pop();
	}

	// 문자 버퍼 소멸
	while (!charBuffer.empty())
	{
		charBuffer.pop();
	}
}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode)
{
	return KeyState[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty()
{
	return keyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty()
{
	return charBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
{
	if (keyBuffer.empty())
	{
		return KeyboardEvent();
	}
	else
	{
		KeyboardEvent e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
}

unsigned char KeyboardClass::ReadChar()
{
	if (charBuffer.empty())
	{
		return 0;
	}
	else
	{
		unsigned char e = charBuffer.front();
		charBuffer.pop();

		return e;
	}
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
	KeyState[key] = true;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
	KeyState[key] = false;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
	charBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys()
{
	autoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys()
{
	autoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
	autoRepeatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
	autoRepeatChars = false;
}

bool KeyboardClass::IsKeysAutoRepeat()
{
	return autoRepeatKeys;
}

bool KeyboardClass::IsCharsAutoRepeat()
{
	return autoRepeatChars;
}

void KeyboardClass::DeleteCharBuffer()
{
	while (!charBuffer.empty())
	{
		charBuffer.pop();
	}
}
