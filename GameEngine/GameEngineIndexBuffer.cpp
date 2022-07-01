#include "PreCompile.h"
#include "GameEngineIndexBuffer.h"

GameEngineIndexBuffer::GameEngineIndexBuffer() :
	Buffer_(nullptr),
	BufferData_(),
	ResData_(),
	Format_(DXGI_FORMAT::DXGI_FORMAT_R32_UINT),
	Offset_(0),
	IndexCount_(0)
{
	memset(&BufferData_, 0, sizeof(BufferData_));
	memset(&ResData_, 0, sizeof(ResData_));
}

GameEngineIndexBuffer::~GameEngineIndexBuffer()
{
	if (nullptr != Buffer_)
	{
		Buffer_->Release();
		Buffer_ = nullptr;
	}
}

void GameEngineIndexBuffer::Create(const std::vector<UINT>& _Datas, D3D11_USAGE _Usage)
{
	Create(reinterpret_cast<const void*>(&_Datas[0]), sizeof(UINT), _Datas.size(), _Usage);
}

void GameEngineIndexBuffer::Create(const void* _Data, size_t _Size, size_t _Count, D3D11_USAGE _Usage) 
{
	ResData_.pSysMem = _Data;
	IndexCount_ = static_cast<UINT>(_Count);

	BufferData_.ByteWidth = static_cast<UINT>(_Size) * static_cast<UINT>(_Count);
	BufferData_.Usage = _Usage;
	BufferData_.CPUAccessFlags = 0;
	if (BufferData_.Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		BufferData_.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	}

	BufferData_.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferData_.StructureByteStride = 0;
	BufferData_.MiscFlags = 0;

	Offset_ = 0;
	Format_ = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferData_, &ResData_, &Buffer_))
	{
		GameEngineDebug::MsgBoxError("버텍스 버퍼 생성 에러");
		return;
	}
}

void GameEngineIndexBuffer::Setting()
{
	GameEngineDevice::GetContext()->IASetIndexBuffer(Buffer_, Format_, Offset_);
}
