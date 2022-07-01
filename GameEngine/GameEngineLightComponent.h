#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

enum class LightShapeType
{
	Direction,
	Mesh,
};

struct LightData
{
	float4 ViewLightDir; // 라이트의 포워드
	float4 ViewNegLightDir;	// 라이트의 포워드의 반대방향
	float4 ViewLightPosition; // 라이트의 위치
	float4 AmbientLight;

	float4 DiffuseLightColor;
	float4 AmbientLightColor;
	float4 SpacularLightColor;
	float4 SpacularLightPow;

	float4 LightPower; // x는 디퓨즈 라이트의 강도 y는 스펙큘러의 강도 z는 앰비언트의 강도 w는 모든 강도};
};

struct LightsData 
{
	int LightCount;
	LightData Lights[128];
};

// 분류 : 라이트
// 용도 : 
// 설명 : 
class GameEngineLightComponent : public GameEngineTransformComponent
{
private:
	LightShapeType ShapeType;
	LightData LightDataObject;

public:
	GameEngineLightComponent();
	~GameEngineLightComponent();

protected:
	GameEngineLightComponent(const GameEngineLightComponent& _Other) = delete;
	GameEngineLightComponent(GameEngineLightComponent&& _Other) noexcept = delete;

private:
	GameEngineLightComponent& operator=(const GameEngineLightComponent& _Other) = delete;
	GameEngineLightComponent& operator=(GameEngineLightComponent&& _Other) noexcept = delete;

public:
	const LightData& GetLightData() 
	{
		return LightDataObject;
	}

	void SetDiffusePower(float _Power) 
	{
		LightDataObject.LightPower.x = _Power;
	}

	void SetSpacularLightPow(float _Pow)
	{
		LightDataObject.SpacularLightPow.x = _Pow;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
};

