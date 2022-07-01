#pragma once
#include "GameEngineComponent.h"
#include "GameEngineTransform.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineTransformComponent : public GameEngineComponent
{
	friend GameEngineActor;

private:
	GameEngineTransform Transform_;

public:
	GameEngineTransformComponent();
	virtual ~GameEngineTransformComponent() = 0;

protected:
	GameEngineTransformComponent(const GameEngineTransformComponent& _Other) = delete;
	GameEngineTransformComponent(GameEngineTransformComponent&& _Other) noexcept = delete;

private:
	GameEngineTransformComponent& operator=(const GameEngineTransformComponent& _Other) = delete;
	GameEngineTransformComponent& operator=(GameEngineTransformComponent&& _Other) noexcept = delete;

public:
	GameEngineTransform* GetTransform()
	{
		return &Transform_;
	}

public:
	void AttachTransform(GameEngineTransform* _Transform);
};

