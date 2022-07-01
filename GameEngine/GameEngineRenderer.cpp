#include "PreCompile.h"
#include "GameEngineRenderer.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineTransform.h"
#include "GameEngineVertexShader.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "CameraComponent.h"

GameEngineRenderer::GameEngineRenderer() :
	PipeLine_(nullptr)
{
}

GameEngineRenderer::~GameEngineRenderer() 
{
	if (nullptr != PipeLine_)
	{
		delete PipeLine_;
		PipeLine_ = nullptr;
	}
}

void GameEngineRenderer::Update(float _DeltaTime)
{
}

void GameEngineRenderer::SetRenderingPipeLine(const std::string& _Value)
{
	std::string UpperName = GameEngineString::toupper(_Value);

	ShaderHelper.Clear();

	GameEngineRenderingPipeLine* FindPipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find(UpperName);

	if (nullptr == FindPipeLine_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 랜더링 파이프라인입니다." + UpperName);
		return;
	}

	if (nullptr == PipeLine_)
	{
		PipeLine_ = FindPipeLine_->Clone();
	}
	else 
	{
		PipeLine_->Copy(FindPipeLine_);
	}

	ShaderHelper.ShaderResourcesCheck(PipeLine_->GetPixelShader());
	ShaderHelper.ShaderResourcesCheck(PipeLine_->GetVertexShader());

	if (true == ShaderHelper.IsConstantBuffer("TransformData"))
	{
		ShaderHelper.SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
	}

	if (true == ShaderHelper.IsConstantBuffer("LightsData"))
	{
		LightsData& Data = GetLevel()->GetMainCamera()->LightData_;
		ShaderHelper.SettingConstantBufferLink("LightsData", Data);
	}

	if (nullptr == PipeLine_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 파이프라인 입니다!");
	}

	SetRenderingPipeLineSettingNext();
}

void GameEngineRenderer::SetMesh(const std::string& _Value)
{
	SetMesh(_Value, _Value);
}

void GameEngineRenderer::SetMesh(const std::string& _Vtx, const std::string& _Idx)
{
	if (nullptr == PipeLine_)
	{
		GameEngineDebug::MsgBoxError("랜더링 파이프라인을 먼저 세팅해주세요." + _Vtx);
		return;
	}

	PipeLine_->SetInputAssembler1VertexBufferSetting(_Vtx);
	PipeLine_->SetInputAssembler2IndexBufferSetting(_Idx);
}

void GameEngineRenderer::SetMesh(GameEngineVertexBuffer* _Vtx, GameEngineIndexBuffer* _Idx)
{
	if (nullptr == PipeLine_)
	{
		GameEngineDebug::MsgBoxError("랜더링 파이프라인을 먼저 세팅해주세요." + _Vtx->GetName());
		return;
	}

	PipeLine_->SetInputAssembler1VertexBufferSetting(_Vtx);
	PipeLine_->SetInputAssembler2IndexBufferSetting(_Idx);
}

void GameEngineRenderer::Render(float _DeltaTime)
{
	ShaderHelper.Setting();
	PipeLine_->Rendering();
	ShaderHelper.ReSet();
	PipeLine_->Reset();
}
