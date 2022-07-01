#pragma once
#include "GameEngineActor.h"
#include "GameEngineLightComponent.h"

// 분류 : 
// 용도 : 
// 설명 : 
class LightActor : public GameEngineActor
{
private:
	GameEngineLightComponent* Light_;

public:
	LightActor();
	~LightActor();

protected:
	LightActor(const LightActor& _Other) = delete;
	LightActor(LightActor&& _Other) noexcept = delete;

private:
	LightActor& operator=(const LightActor& _Other) = delete;
	LightActor& operator=(LightActor&& _Other) noexcept = delete;

public:
	GameEngineLightComponent* GetLight() 
	{
		return Light_;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
};

