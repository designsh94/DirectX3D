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
	// 512 ����Ʈ �����͸� �������ټ� �ִ� ���۸� ������ٰ� �ϴ���
// ��¥ 512���� ����Ʈ�� �����ϴ°��� �ƴҼ��� �����Ƿ�
// ������ ���� ���õ��� ���̵� ����Ʈ�� �������� �ʰ�����
// �����ϴϱ� ����� �ص־� �Ұ̴ϴ�.

// ���⿡�� ���� �����͸� �������ְ� �Ǵµ�.
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

	BufferData_.ByteWidth = DataSize * DataCount; // GPU �� ������ ����ȭ ���� �޸� ũ��(�ּҴ��� ??)
	BufferData_.Usage = D3D11_USAGE_DYNAMIC;
	BufferData_.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferData_.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	BufferData_.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	BufferData_.StructureByteStride = DataSize; // 48 ũ���� ����ü

	D3D11_SUBRESOURCE_DATA StartData;


	D3D11_SUBRESOURCE_DATA* StartDataPtr = nullptr;

	if (nullptr != _StartData)
	{
		StartData.pSysMem = _StartData;
	}


	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferData_, StartDataPtr, &Buffer_))
	{
		GameEngineDebug::MsgBoxError("��Ʈ��ó�� ���� ������ �����߽��ϴ�.");

	}

	// Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSRVDesc.BufferEx.FirstElement = 0;
	tSRVDesc.BufferEx.Flags = 0;
	tSRVDesc.BufferEx.NumElements = DataCount; // 10���� �츮���� m_pSRV�� �־�޶�.

	if (S_OK != GameEngineDevice::GetDevice()->CreateShaderResourceView(GetBuffer(), &tSRVDesc, &SRV))
	{
		GameEngineDebug::MsgBoxError("FAIL (S_OK != GameEngineDevice::GetDevice()->CreateShaderResourceView(GetBuffer(), &tSRVDesc, &m_pSRV))");
	}
}