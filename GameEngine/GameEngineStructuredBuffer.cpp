#include "PreCompile.h"
#include "GameEngineStructuredBuffer.h"

GameEngineStructuredBuffer::GameEngineStructuredBuffer() 
{
}

GameEngineStructuredBuffer::~GameEngineStructuredBuffer() 
{
	if (nullptr != SRV)
	{
		SRV->Release();
		SRV = nullptr;
	}

	if (nullptr != Buffer_)
	{
		Buffer_->Release();
		Buffer_ = nullptr;
	}
}

void GameEngineStructuredBuffer::ChangeData(const void* _Data, size_t _Size)
{
	// 512 라이트 데이터를 세팅해줄수 있는 버퍼를 만들었다고 하더라도
// 진짜 512개의 라이트를 세팅하는것은 아닐수가 있으므로
// 기존에 만든 세팅들이 사이드 이펙트가 생기지는 않겠지만
// 위험하니까 고려는 해둬야 할겁니다.

// 여기에서 실제 데이터를 세팅해주게 되는데.
#ifdef _DEBUG
	//if (m_BufferInfo.ByteWidth != _Size)
	//{
	//	GameEngineDebug::AssertMsg("if (m_BufferInfo.ByteWidth != _Size)");
	//}
	ResData_.pData = nullptr;
#endif
	GameEngineDevice::GetContext()->Map(Buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &ResData_);

#ifdef _DEBUG
	if (nullptr == ResData_.pData)
	{
		GameEngineDebug::MsgBoxError("if (nullptr == m_Res.pData)");
	}
#endif 

	memcpy_s(ResData_.pData, BufferData_.ByteWidth, _Data, _Size);

	GameEngineDevice::GetContext()->Unmap(Buffer_, 0);
}


void GameEngineStructuredBuffer::Create(UINT _DataSize, UINT _DataCount, void* _StartData) 
{
	DataSize = _DataSize;
	DataCount = _DataCount;

	BufferData_.ByteWidth = DataSize * DataCount; // GPU 에 생성할 구조화 버퍼 메모리 크기(최소단위 ??)
	BufferData_.Usage = D3D11_USAGE_DYNAMIC;
	BufferData_.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferData_.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	BufferData_.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	BufferData_.StructureByteStride = DataSize; // 48 크기의 구조체

	D3D11_SUBRESOURCE_DATA StartData;


	D3D11_SUBRESOURCE_DATA* StartDataPtr = nullptr;

	if (nullptr != _StartData)
	{
		StartData.pSysMem = _StartData;
	}


	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferData_, StartDataPtr, &Buffer_))
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