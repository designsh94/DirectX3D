#pragma once
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngineDevice.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineConstantBuffer : public GameEngineObjectNameBase
{
private:
	ID3D11Buffer* Buffer_;
	D3D11_BUFFER_DESC BufferData_;
	D3D11_MAPPED_SUBRESOURCE ResData_;
	D3D11_SHADER_BUFFER_DESC BufferDesc_;

	std::vector<D3D11_SHADER_VARIABLE_DESC> VarInfos_;

public:
	GameEngineConstantBuffer();
	~GameEngineConstantBuffer();

protected:
	GameEngineConstantBuffer(const GameEngineConstantBuffer& _Other) = delete;
	GameEngineConstantBuffer(GameEngineConstantBuffer&& _Other) noexcept = delete;

private:
	GameEngineConstantBuffer& operator=(const GameEngineConstantBuffer& _Other) = delete;
	GameEngineConstantBuffer& operator=(GameEngineConstantBuffer&& _Other) noexcept = delete;

public:
	inline unsigned int GetBufferSize() 
	{
		return BufferData_.ByteWidth;
	}

	inline ID3D11Buffer*& GetBuffer()
	{
		return Buffer_;
	}

public:
	void Create(const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo);
	void ChangeData(const void* _Data, size_t _Size);
};

