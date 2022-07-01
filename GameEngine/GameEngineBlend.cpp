#include "PreCompile.h"
#include "GameEngineBlend.h"

GameEngineBlend::GameEngineBlend() :
	State_(nullptr),
	Info_(),
	Mask_(0xfffffff)
{
}

GameEngineBlend::~GameEngineBlend()
{
	if (nullptr != State_)
	{
		State_->Release();
		State_ = nullptr;
	}
}

void GameEngineBlend::ReCreate(const D3D11_BLEND_DESC& _Info, float4 _Factor, unsigned int _Mask) 
{
	Create(_Info, _Factor, _Mask);
}

void GameEngineBlend::Create(const D3D11_BLEND_DESC& _Info, float4 _Factor, unsigned int _Mask)
{
	ID3D11BlendState* State = nullptr;

	if (S_OK != GameEngineDevice::GetDevice()->CreateBlendState(&_Info, &State))
	{
		GameEngineDebug::MsgBox("샘플러 생성에 실패했습니다.");
		return;
	}

	Factor_ = _Factor;
	Mask_ = _Mask;

	Info_ = _Info;
	State_ = State;
}

void GameEngineBlend::Setting()
{
	GameEngineDevice::GetContext()->OMSetBlendState(State_, Factor_.Arr1D, Mask_);
}

void GameEngineBlend::Reset()
{
	GameEngineDevice::GetContext()->OMSetBlendState(nullptr, Factor_.Arr1D, Mask_);
}
