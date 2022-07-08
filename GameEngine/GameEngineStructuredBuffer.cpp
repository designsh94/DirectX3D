#include "PreCompile.h"
#include "GameEngineStructuredBuffer.h"

GameEngineStructuredBuffer::GameEngineStructuredBuffer() :
	Buffer(nullptr),
	BufferData{},
	ResData{},
	BufferDesc{},
	SRV(nullptr),
	DataSize(0),
	DataCount(0)
{
}

GameEngineStructuredBuffer::~GameEngineStructuredBuffer() 
{
	if (nullptr != SRV)
	{
		SRV->Release();
		SRV = nullptr;
	}

	if (nullptr != Buffer)
	{
		Buffer->Release();
		Buffer = nullptr;
	}
}

void GameEngineStructuredBuffer::ChangeData(const void* _Data, size_t _Size)
{
#ifdef _DEBUG
	ResData.pData = nullptr;
#endif

	GameEngineDevice::GetContext()->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ResData);

#ifdef _DEBUG
	if (nullptr == ResData.pData)
	{
		GameEngineDebug::MsgBoxError("if (nullptr == m_Res.pData)");
	}
#endif 
	memcpy_s(ResData.pData, BufferData.ByteWidth, _Data, _Size);

	GameEngineDevice::GetContext()->Unmap(Buffer, 0);
}

void GameEngineStructuredBuffer::Create(UINT _DataSize, UINT _DataCount, void* _StartData) 
{
	DataSize = _DataSize;
	DataCount = _DataCount;

	BufferData.ByteWidth = DataSize * DataCount; // GPU 에 생성할 구조화 버퍼 메모리 크기(최소단위 ??)
	BufferData.Usage = D3D11_USAGE_DYNAMIC;
	BufferData.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferData.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	BufferData.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	BufferData.StructureByteStride = DataSize; // 48 크기의 구조체

	D3D11_SUBRESOURCE_DATA StartData = {};
	D3D11_SUBRESOURCE_DATA* StartDataPtr = nullptr;
	if (nullptr != _StartData)
	{
		StartDataPtr = &StartData;
		StartData.pSysMem = _StartData;
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferData, StartDataPtr, &Buffer))
	{
		GameEngineDebug::MsgBoxError("스트럭처드 버퍼 생성에 실패했습니다.");

	}

	// Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSRVDesc.BufferEx.FirstElement = 0;
	tSRVDesc.BufferEx.Flags = 0;
	tSRVDesc.BufferEx.NumElements = DataCount; // 10개로 우리어진 m_pSRV를 넣어달라.

	if (S_OK != GameEngineDevice::GetDevice()->CreateShaderResourceView(GetBuffer(), &tSRVDesc, &SRV))
	{
		GameEngineDebug::MsgBoxError("FAIL (S_OK != GameEngineDevice::GetDevice()->CreateShaderResourceView(GetBuffer(), &tSRVDesc, &m_pSRV))");
	}
}
