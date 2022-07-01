#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

// 분류 : 렌더러 베이스
// 용도 : 상속
// 설명 : 모든 렌더러의 베이스가 되는 클래스
class CameraComponent;
class GameEngineRendererBase : public GameEngineTransformComponent
{
	friend CameraComponent;

public:
	GameEngineRendererBase();
	~GameEngineRendererBase();

protected:
	GameEngineRendererBase(const GameEngineRendererBase& _Other) = delete;
	GameEngineRendererBase(GameEngineRendererBase&& _Other) noexcept = delete;

private:
	GameEngineRendererBase& operator=(const GameEngineRendererBase& _Other) = delete;
	GameEngineRendererBase& operator=(GameEngineRendererBase&& _Other) noexcept = delete;

protected:
	void Start() override;

protected:
	virtual void SetRenderGroup(int _Order);
	virtual void Render(float _DeltaTime);

public:
};

