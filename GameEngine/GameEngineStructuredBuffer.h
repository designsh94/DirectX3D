#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>

// 설명 :
class GameEngineStructuredBuffer : public GameEngineObjectNameBase
{
public:
	// constrcuter destructer
	GameEngineStructuredBuffer();
	~GameEngineStructuredBuffer();

	// delete Function
	GameEngineStructuredBuffer(const GameEngineStructuredBuffer& _Other) = delete;
	GameEngineStructuredBuffer(GameEngineStructuredBuffer&& _Other) noexcept = delete;
	GameEngineStructuredBuffer& operator=(const GameEngineStructuredBuffer& _Other) = delete;
	GameEngineStructuredBuffer& operator=(GameEngineStructuredBuffer&& _Other) noexcept = delete;


	inline unsigned int GetBufferSize()
	{
		return BufferData_.ByteWidth;
	}

	inline ID3D11Buffer*& GetBuffer()
	{
		return Buffer_;
	}

	void Create(UINT _DataSize, UINT _DataCount, void* _StartData);
	void ChangeData(const void* _Data, size_t _Size);


protected:

private:
	ID3D11Buffer* Buffer_;
	D3D11_BUFFER_DESC BufferData_;
	D3D11_MAPPED_SUBRESOURCE ResData_;
	D3D11_SHADER_BUFFER_DESC BufferDesc_;

	std::vector<D3D11_SHADER_VARIABLE_DESC> VarInfos_;

	ID3D11ShaderResourceView* SRV; // 그냥 쉐이더에 세팅할때죠?

	UINT DataSize;
	UINT DataCount;

};

