#pragma once
#include "GameEngineActor.h"

struct ResultColor 
{
public:
	float4 MulColor = float4::ONE;
	float4 PlusColor = float4::ZERO;
};

// 분류 : 
// 용도 : 
// 설명 : 
class SKySphereActor : public GameEngineActor
{
public:
	SKySphereActor();
	~SKySphereActor();

protected:
	SKySphereActor(const SKySphereActor& _Other) = delete;
	SKySphereActor(SKySphereActor&& _Other) noexcept = delete;

private:
	SKySphereActor& operator=(const SKySphereActor& _Other) = delete;
	SKySphereActor& operator=(SKySphereActor&& _Other) noexcept = delete;

private:
	GameEngineRenderer* Renderer;

public:
	ResultColor Data;

private:
	void SetRadius(float _Radius);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
};

