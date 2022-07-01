#pragma once
#include "GameEngineSoundManager.h"
#include "GameEngineObjectNameBase.h"

// 분류 : FMOD Sound 객체관리
// 용도 : 
// 설명 : FMOD Sound 객체 생성 및 Sound File Load 기능제공(단, Sound 객체와 Sound File은 1:1로 대응된다.)
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

