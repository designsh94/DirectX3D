#pragma once
#include "GameEngineTransform.h"
#include "GameEngineTransformComponent.h"
#include "GameEngineShaderResHelper.h"

// �з� : ����� ���� ������
// �뵵 : ����׿� �浹ü �޽� ����
// ���� : 
class GameEngineLevel;
class GameEngineRenderingPipeLine;
class GameEngineDebugRenderData
{
	friend class CameraComponent;

private:
	TransformData Data_;
	float4 Color_;
	GameEngineShaderResHelper ShaderHelper_;
	GameEngineRenderingPipeLine* PipeLine_;

public:
	GameEngineDebugRenderData();
	GameEngineDebugRenderData(const GameEngineDebugRenderData& _Other) :
		Data_(_Other.Data_),
		ShaderHelper_(_Other.ShaderHelper_),
		PipeLine_(_Other.PipeLine_)
	{
	}

	~GameEngineDebugRenderData();
};

