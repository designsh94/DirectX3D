#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>

// Ό³Έν :
class GameEngineStructuredBuffer : public GameEngineObjectNameBase
{
private:
	ID3D11Buffer* Buffer;
	D3D11_BUFFER_DESC BufferData;
	D3D11_MAPPED_SUBRESOURCE ResData;
	D3D11_SHADER_BUFFER_DESC BufferDesc;
	ID3D11ShaderResourceView* SRV;
	UINT DataSize;
	UINT DataCount;
	std::vector<D3D11_SHADER_VARIABLE_DESC> VarInfos_;

public:
	GameEngineStructuredBuffer();
	~GameEngineStructuredBuffer();

protected:
	GameEngineStructuredBuffer(const GameEngineStructuredBuffer& _Other) = delete;
	GameEngineStructuredBuffer(GameEngineStructuredBuffer&& _Other) noexcept = delete;

private:
	GameEngineStructuredBuffer& operator=(const GameEngineStructuredBuffer& _Other) = delete;
	GameEngineStructuredBuffer& operator=(GameEngineStructuredBuffer&& _Other) noexcept = delete;

public:
	inline unsigned int GetBufferSize()
	{
		return BufferData.ByteWidth;
	}

	inline ID3D11Buffer*& GetBuffer()
	{
		return Buffer;
	}

	inline ID3D11ShaderResourceView** GetShaderResourcesView()
	{
		return &SRV;
	}

public:
	void Create(UINT _DataSize, UINT _DataCount, void* _StartData);
	void ChangeData(const void* _Data, size_t _Size);

public:
};
