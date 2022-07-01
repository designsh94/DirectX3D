#pragma once
#include "GameEngineSound.h"
#include "GameEngineSoundManager.h"

// �з� : FMOD Sound System ����
// �뵵 :
// ���� : Sound Channel�� ��� Sound�� Control�ϴ� ����� ����
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
	void PlayOverLap(const std::string& _name, int _LoopCount = 1); // ���� ����� ������� ���ļ� ���
	void PlayAlone(const std::string& _name, int _LoopCount = 1); // ������ ���尡 �������� ���

public:
	void Stop();

public:
	void SetVolume(float _Volume);
	void VolumeUp();
	void VolumeDown();
};

