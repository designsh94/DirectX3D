#pragma once
#include "GameEngineRendererBase.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

// 분류 : 렌더러(Renderer)
// 용도 : 
// 설명 : 
class CameraComponent;
class GameEngineLevel;
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineRenderingPipeLine;
class GameEngineRenderer : public GameEngineRendererBase
{
private:
	friend GameEngineLevel;

public:
	GameEngineShaderResHelper ShaderHelper;

private:
	GameEngineRenderingPipeLine* PipeLine_;

public:
	GameEngineRenderer();
	~GameEngineRenderer();

protected:
	GameEngineRenderer(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer(GameEngineRenderer&& _Other) noexcept = delete;

private:
	GameEngineRenderer& operator=(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer& operator=(GameEngineRenderer&& _Other) noexcept = delete;

private:
	void Update(float _DeltaTime) override;

public:
	inline GameEngineRenderingPipeLine* GetGameEngineRenderingPipeLine()
	{
		return PipeLine_;
	}

public:
	void SetRenderingPipeLine(const std::string& _Value);
	void SetMesh(const std::string& _Vtx, const std::string& _Idx);
	void SetMesh(const std::string& _Value);
	void SetMesh(GameEngineVertexBuffer* _Vtx, GameEngineIndexBuffer* _Idx);

protected:
	void Render(float _DeltaTime) override;

public:
	virtual void SetRenderingPipeLineSettingNext() {}
};

