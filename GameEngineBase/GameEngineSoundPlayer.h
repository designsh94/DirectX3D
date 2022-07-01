#pragma once
#include "GameEngineSound.h"
#include "GameEngineSoundManager.h"

// 분류 : FMOD Sound System 조작
// 용도 :
// 설명 : Sound Channel에 담긴 Sound를 Control하는 기능을 제공
class GameEngineSoundPlayer
{
	friend GameEngineSoundManager;

private:
	GameEngineSoundManager* playSoundFile_;
	FMOD::Channel* playChannel_;

private:
	int PlayCount;
	float Volume_;

private:		
	GameEngineSoundPlayer();
	~GameEngineSoundPlayer();

public:
	GameEngineSoundPlayer(const GameEngineSoundPlayer& _other) = delete;
	GameEngineSoundPlayer(GameEngineSoundPlayer&& _other) noexcept;

public:
	GameEngineSoundPlayer& operator=(const GameEngineSoundPlayer& _other) = delete;
	GameEngineSoundPlayer& operator=(const GameEngineSoundPlayer&& _other) = delete;

public:
	bool IsPlay();

public:
	void PlayCountReset(int _Count = -1);
	void PlayOverLap(const std::string& _name, int _LoopCount = 1); // 기존 사운드와 관계없이 겹쳐서 재생
	void PlayAlone(const std::string& _name, int _LoopCount = 1); // 기존의 사운드가 재생종료시 재생

public:
	void Stop();

public:
	void SetVolume(float _Volume);
	void VolumeUp();
	void VolumeDown();
};

