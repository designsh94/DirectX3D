#include "PreCompile.h"
#include "GameEngineDepthStencil.h"

GameEngineDepthStencil::GameEngineDepthStencil() 
{
}

GameEngineDepthStencil::~GameEngineDepthStencil() 
{
	if (nullptr != State_)
	{
		State_->Release();
		State_ = nullptr;
	}
}

void GameEngineDepthStencil::Create(const D3D11_DEPTH_STENCIL_DESC& _Info)
{
	ID3D11DepthStencilState* State = nullptr;

	if (S_OK != GameEngineDevice::GetDevice()->CreateDepthStencilState(&_Info, &State))
	{
		GameEngineDebug::MsgBoxError("DepthStencil state 생성에 실패했습니다.");
		return;
	}

	Info_ = _Info;
	State_ = State;

}

void GameEngineDepthStencil::ReCreate(const D3D11_DEPTH_STENCIL_DESC& _Info)
{
	if (nullptr != State_)
	{
		State_->Release();
		State_ = nullptr;
	}

	Create(_Info);
}

void GameEngineDepthStencil::Setting() 
{
	GameEngineDevice::GetContext()->OMSetDepthStencilState(State_, 0);
}

void GameEngineDepthStencil::Reset()
{
	GameEngineDevice::GetContext()->OMSetDepthStencilState(nullptr, 0);
}
