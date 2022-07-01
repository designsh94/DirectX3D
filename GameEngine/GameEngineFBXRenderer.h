#pragma once
#include "GameEngineRendererBase.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

class RenderSet 
{
public:
	GameEngineShaderResHelper* ShaderHelper;
	GameEngineRenderingPipeLine* PipeLine_;
};

// 설명 : 하나의 랜더 단위를 표현
class CameraComponent;
class GameEngineLevel;
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineRenderingPipeLine;
class GameEngineFBXRenderer : public GameEngineRendererBase
{
private:
	friend GameEngineLevel;
	friend CameraComponent;

private:
	std::vector<RenderSet> RenderSets;

public:
	GameEngineFBXRenderer();
	~GameEngineFBXRenderer();

protected:
	GameEngineFBXRenderer(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer(GameEngineFBXRenderer&& _Other) noexcept = delete;

private:
	GameEngineFBXRenderer& operator=(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer& operator=(GameEngineFBXRenderer&& _Other) noexcept = delete;

private:
	void Update(float _DeltaTime) override;

public:
	size_t GetRenderSetCount()
	{
		return RenderSets.size();
	}

	RenderSet& GetRenderSet(unsigned int _Index)
	{
		return RenderSets[_Index];
	}

public:
	void SetFBXMesh(const std::string& _Value, std::string _PipeLine);

protected:
	void Start() override;
	void Render(float _DeltaTime) override;

public:
};

