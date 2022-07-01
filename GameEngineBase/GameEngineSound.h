#pragma once
#include "GameEngineSoundManager.h"
#include "GameEngineObjectNameBase.h"

// �з� : FMOD Sound ��ü����
// �뵵 : 
// ���� : FMOD Sound ��ü ���� �� Sound File Load �������(��, Sound ��ü�� Sound File�� 1:1�� �����ȴ�.)
class GameEngineSoundPlayer;
class GameEngineSound : public GameEngineObjectNameBase
{
	friend GameEngineSoundManager;
	friend GameEngineSoundPlayer;

private:
	FMOD::Sound* sound_;

private:		
	GameEngineSound();
	~GameEngineSound();

public:
	GameEngineSound(const GameEngineSound& _other) = delete;
	GameEngineSound(GameEngineSound&& _other) noexcept;

public:
	GameEngineSound& operator=(const GameEngineSound& _other) = delete;
	GameEngineSound& operator=(const GameEngineSound&& _other) = delete;

public:
	bool Load(const std::string& _Path);
};

