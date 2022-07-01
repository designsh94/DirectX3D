#include "PreCompile.h"
#include "GameEngineWindow.h"
#include "GameEngineResourcesManager.h"

GameEngineRenderingPipeLine::GameEngineRenderingPipeLine() :
	VertexBuffer_(nullptr),
	VertexShader_(nullptr),
	IndexBuffer_(nullptr),
	Topology_(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	LayOut_(nullptr),
	IsCloneLayOutCreate(false)
{
	SetOutputMergerBlend("AlphaBlend");
	SetRasterizer("EngineBaseRasterizerBack");
	SetOutputMergerDepthStencil("BaseDepthOn");
	SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

GameEngineRenderingPipeLine::~GameEngineRenderingPipeLine()
{
	if (IsClone() && true == IsCloneLayOutCreate)
	{
		if (nullptr != LayOut_)
		{
			LayOut_->Release();
			LayOut_ = nullptr;
		}
	}
	else if(false == IsClone())
	{
		if (nullptr != LayOut_)
		{
			LayOut_->Release();
			LayOut_ = nullptr;
		}
	}

	if (true == Rasterizer_->IsClone())
	{
		delete Rasterizer_;
		Rasterizer_ = nullptr;
	}
}

GameEngineRenderingPipeLine::GameEngineRenderingPipeLine(GameEngineRenderingPipeLine&& _other) noexcept :
	VertexBuffer_(_other.VertexBuffer_),
	VertexShader_(_other.VertexShader_),
	IndexBuffer_(_other.IndexBuffer_),
	Topology_(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	LayOut_(_other.LayOut_),
	IsCloneLayOutCreate(_other.IsCloneLayOutCreate)
{
}

void GameEngineRenderingPipeLine::SetInputAssembler1VertexBufferSetting(const std::string& _Name)
{
	VertexBuffer_ = GameEngineVertexBufferManager::GetInst().Find(_Name);
	if (nullptr == VertexBuffer_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 버텍스 버퍼를 세팅하려고 했습니다.");
		return;
	}

	if (nullptr != VertexShader_)
	{
		CreateLayOut();
	}
}

void GameEngineRenderingPipeLine::SetInputAssembler1VertexBufferSetting(GameEngineVertexBuffer* _Buffer)
{
	if (nullptr == _Buffer)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 버텍스 버퍼를 세팅하려고 했습니다.");
		return;
	}

	VertexBuffer_ = _Buffer;
}

void GameEngineRenderingPipeLine::SetInputAssembler2IndexBufferSetting(GameEngineIndexBuffer* _Buffer)
{
	if (nullptr == _Buffer)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 인덱스 버퍼를 세팅하려고 했습니다.");
		return;
	}

	IndexBuffer_ = _Buffer;
}

void GameEngineRenderingPipeLine::SetInputAssembler2IndexBufferSetting(const std::string& _Name) 
{
	IndexBuffer_ = GameEngineIndexBufferManager::GetInst().Find(_Name);

	if (nullptr == IndexBuffer_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 버텍스 버퍼를 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY _Topology) 
{
	Topology_ = _Topology;
}


void GameEngineRenderingPipeLine::SetVertexShader(const std::string& _Name)
{
	VertexShader_ = GameEngineVertexShaderManager::GetInst().Find(_Name);

	if (nullptr == VertexShader_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 버텍스 쉐이더를 세팅하려고 했습니다.");
		return;
	}

	if (nullptr != VertexBuffer_)
	{
		CreateLayOut();
	}
}

void GameEngineRenderingPipeLine::CreateLayOut()
{
	if (true == IsClone())
	{
		IsCloneLayOutCreate = true;
	}

	if (nullptr == VertexBuffer_->InputLayoutDesc_)
	{
		GameEngineDebug::MsgBoxError("인풋 레이아웃 정보가 없습니다.");
	}
	
	std::vector<D3D11_INPUT_ELEMENT_DESC>& InputLayoutDesc_ = *VertexBuffer_->InputLayoutDesc_;

	if (S_OK != GameEngineDevice::GetInst().GetDevice()->CreateInputLayout(&InputLayoutDesc_[0], static_cast<unsigned int>(InputLayoutDesc_.size()), VertexShader_->CodeBlob_->GetBufferPointer(), VertexShader_->CodeBlob_->GetBufferSize(), &LayOut_))
	{
		GameEngineDebug::MsgBoxError("인풋레이아웃 생성에 실패했습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetRasterizer(const std::string& _Name) 
{
	Rasterizer_ = GameEngineRasterizerManager::GetInst().Find(_Name);
	if (nullptr == Rasterizer_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 레이터라이저 세팅을 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetPixelShader(const std::string& _Name) 
{
	PixelShader_ = GameEnginePixelShaderManager::GetInst().Find(_Name);
	if (nullptr == PixelShader_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 픽셀 쉐이더를 세팅을 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetOutputMergerBlend(const std::string& _Name)
{
	Blend_ = GameEngineBlendManager::GetInst().Find(_Name);
	if (nullptr == Blend_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 블랜드를 세팅을 세팅하려고 했습니다.");
		return;
	}

}

void GameEngineRenderingPipeLine::SetOutputMergerDepthStencil(const std::string& _Name) 
{
	DepthStencil_ = GameEngineDepthStencilManager::GetInst().Find(_Name);
	if (nullptr == DepthStencil_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 깊이 세팅을 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::InputAssembler1() 
{
	VertexBuffer_->Setting();

	if (nullptr != LayOut_)
	{
		GameEngineDevice::GetInst().GetContext()->IASetInputLayout(LayOut_);
	}
}

void GameEngineRenderingPipeLine::InputAssembler2() 
{
	IndexBuffer_->Setting();
	GameEngineDevice::GetContext()->IASetPrimitiveTopology(Topology_);
}

void GameEngineRenderingPipeLine::VertexShader() 
{
	VertexShader_->Setting();
}

void GameEngineRenderingPipeLine::Rasterizer()
{
	Rasterizer_->Setting();
	Rasterizer_->SettingViewPort();
}

void GameEngineRenderingPipeLine::PixelShader()
{
	PixelShader_->Setting();
}

void GameEngineRenderingPipeLine::OutPutMerger() 
{
	Blend_->Setting();
	DepthStencil_->Setting();
}

void GameEngineRenderingPipeLine::RenderingPipeLineSetting() 
{
	InputAssembler1();
	InputAssembler2();
	VertexShader();
	Rasterizer();
	PixelShader();
	OutPutMerger();
}

void GameEngineRenderingPipeLine::Rendering() 
{
	RenderingPipeLineSetting();
	GameEngineDevice::GetContext()->DrawIndexed(IndexBuffer_->GetIndexCount(), 0, 0);
}

void GameEngineRenderingPipeLine::InstanceRendering() 
{
	RenderingPipeLineSetting();
}

void GameEngineRenderingPipeLine::Reset() 
{
	Blend_->Reset();
	DepthStencil_->Reset();
}

void GameEngineRenderingPipeLine::Copy(GameEngineRenderingPipeLine* _Value) 
{
	VertexBuffer_ = _Value->VertexBuffer_;
	VertexShader_ = _Value->VertexShader_;
	IndexBuffer_ = _Value->IndexBuffer_;
	Topology_ = _Value->Topology_;
	Rasterizer_ = _Value->Rasterizer_;
	PixelShader_ = _Value->PixelShader_;
	Blend_ = _Value->Blend_;
	RenderTarget_ = _Value->RenderTarget_;
	DepthStencil_ = _Value->DepthStencil_;
}

GameEngineRenderingPipeLine* GameEngineRenderingPipeLine::Clone()
{
	GameEngineRenderingPipeLine* NewClone = new GameEngineRenderingPipeLine();

	NewClone->VertexBuffer_			= VertexBuffer_;
	NewClone->VertexShader_			= VertexShader_;
	NewClone->IndexBuffer_				= IndexBuffer_;
	NewClone->LayOut_				= LayOut_;
	NewClone->Topology_				= Topology_;
	NewClone->Rasterizer_				= Rasterizer_;
	NewClone->PixelShader_				= PixelShader_;
	NewClone->Blend_					= Blend_;
	NewClone->RenderTarget_			= RenderTarget_;
	NewClone->DepthStencil_			= DepthStencil_;

	NewClone->CloneOn();

	return NewClone;
}

void GameEngineRenderingPipeLine::RasterizerClone()
{
	Rasterizer_ = Rasterizer_->Clone();
}
