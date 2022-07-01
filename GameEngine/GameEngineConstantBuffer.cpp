#include "PreCompile.h"
#include "GameEngineConstantBuffer.h"

GameEngineConstantBuffer::GameEngineConstantBuffer() :
	Buffer_(nullptr),
	BufferData_(),
	ResData_(),
	BufferDesc_()
{
}

GameEngineConstantBuffer::~GameEngineConstantBuffer() 
{
	if (nullptr != Buffer_)
	{
		Buffer_->Release();
		Buffer_ = nullptr;
	}
}

void GameEngineConstantBuffer::Create(const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo)
{
	BufferDesc_ = _BufferDesc;

	BufferData_.ByteWidth = BufferDesc_.Size;
	BufferData_.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	BufferData_.CPUAccessFlags = 0;
	if (BufferData_.Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		BufferData_.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	}

	BufferData_.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferData_.StructureByteStride = 0;
	BufferData_.MiscFlags = 0;

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferData_, nullptr, &Buffer_))
	{
		GameEngineDebug::MsgBoxError("상수버퍼 생성에 오류가 있습니다." + GetName());
		return;
	}

	for (unsigned int i = 0; i < BufferDesc_.Variables; i++)
	{
		ID3D11ShaderReflectionVariable* Var = _VarInfo->GetVariableByIndex(i);
		D3D11_SHADER_VARIABLE_DESC VarDesc;
		Var->GetDesc(&VarDesc);
		VarInfos_.push_back(VarDesc);
	}
}

void GameEngineConstantBuffer::ChangeData(const void* _Data, size_t _Size) 
{
	if (BufferData_.ByteWidth != _Size)
	{
		GameEngineDebug::MsgBoxError("상수버퍼 세팅의 크기가 다릅니다.");
	}

	memset(&ResData_, 0, sizeof(ResData_));

	GameEngineDevice::GetContext()->Map(Buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &ResData_);
	memcpy_s(ResData_.pData, BufferData_.ByteWidth, _Data, BufferData_.ByteWidth);
	GameEngineDevice::GetContext()->Unmap(Buffer_, 0);
}
