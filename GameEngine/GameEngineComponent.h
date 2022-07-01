#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>

// 분류 : 컴포넌트
// 용도 : 
// 설명 : 
class GameEngineLevel;
class GameEngineActor;
class GameEngineComponent : public GameEngineObjectNameBase
{
	friend GameEngineActor;

private:
	GameEngineActor* Actor_;

public:
	GameEngineComponent();
	virtual ~GameEngineComponent() = 0;

protected:
	GameEngineComponent(const GameEngineComponent& _Other) = delete;
	GameEngineComponent(GameEngineComponent&& _Other) noexcept = delete;

private:
	GameEngineComponent& operator=(const GameEngineComponent& _Other) = delete;
	GameEngineComponent& operator=(GameEngineComponent&& _Other) noexcept = delete;

public:
	GameEngineActor* GetActor() 
	{
		return Actor_;
	}

	GameEngineLevel* GetLevel();

protected:
	virtual void InitComponent(GameEngineActor* Actor_);
	virtual void Start() = 0;
	virtual void Update(float _DeltaTime) = 0;
};

