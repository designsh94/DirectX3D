#include "PreCompile.h"
#include "GameEngineRendererBase.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineTransform.h"
#include "GameEngineVertexShader.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "CameraComponent.h"

GameEngineRendererBase::GameEngineRendererBase() 
{
}

GameEngineRendererBase::~GameEngineRendererBase() 
{
}

void GameEngineRendererBase::Start() 
{
	GetLevel()->GetMainCamera()->PushRenderer(GetOrder(), this);
}

void GameEngineRendererBase::Render(float _DeltaTime) 
{
}

void GameEngineRendererBase::SetRenderGroup(int _Order)
{
	GetLevel()->GetMainCamera()->ChangeRendererGroup(_Order, this);
}