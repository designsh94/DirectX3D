#pragma once

// 분류 : 
// 용도 : 
// 설명 : 
class KeyboardEvent
{
public:
	enum class EventType
	{
		Press,
		Release,
		Invalid
	};

private:
	EventType type;
	unsigned char key;

public:
	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);

public:
	bool IsPress() const;
	bool IsRelease() const;
	bool IsValid() const;
	unsigned char GetKeyCode() const;
};

