#include "PreCompile.h"
#include "GameEnginePostProcessRender.h"
#include "GameEngineWindow.h"

GameEnginePostProcessRender::GameEnginePostProcessRender() :
	Target_(nullptr),
	Effect_(nullptr),
	IsResultCreate_(false),
	Result_(nullptr)
{
}

GameEnginePostProcessRender::~GameEnginePostProcessRender() 
{
	if (true == Effect_->IsClone())
	{
		delete Effect_;
		Effect_ = nullptr;
	}

	if (true == IsResultCreate_)
	{
		delete Result_;
		Result_ = nullptr;
	}
}

void GameEnginePostProcessRender::CreateResultTarget()
{
	Result_ = new GameEngineRenderTarget();
	Result_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE);
	Result_->CreateDepthBuffer(GameEngineWindow::GetInst().GetSize());
	IsResultCreate_ = true;
}

void GameEnginePostProcessRender::CreateResultTarget(const float4& _Size)
{
	Result_ = new GameEngineRenderTarget();
	Result_->Create(_Size, float4::NONE);
	Result_->CreateDepthBuffer(_Size);
	IsResultCreate_ = true;
}
