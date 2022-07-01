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
	float4 ViewLightDir; // ����Ʈ�� ������
	float4 ViewNegLightDir;	// ����Ʈ�� �������� �ݴ����
	float4 ViewLightPosition; // ����Ʈ�� ��ġ
	float4 AmbientLight;

	float4 DiffuseLightColor;
	float4 AmbientLightColor;
	float4 SpacularLightColor;
	float4 SpacularLightPow;

	float4 LightPower; // x�� ��ǻ�� ����Ʈ�� ���� y�� ����ŧ���� ���� z�� �ں��Ʈ�� ���� w�� ��� ����};
};

struct LightsData 
{
	int LightCount;
	LightData Lights[128];
};

// �з� : ����Ʈ
// �뵵 : 
// ���� : 
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

