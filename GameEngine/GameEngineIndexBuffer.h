#pragma once
#include "GameEngineDevice.h"

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineIndexBuffer : public GameEngineObjectNameBase
{
public:
	ID3D11Buffer* Buffer_;
	D3D11_BUFFER_DESC BufferData_;
	D3D11_SUBRESOURCE_DATA ResData_;
	DXGI_FORMAT Format_;
	UINT Offset_;
	int IndexCount_;

public:
	GameEngineIndexBuffer();
	~GameEngineIndexBuffer();

protected:
	GameEngineIndexBuffer(const GameEngineIndexBuffer& _other) = delete;
	GameEngineIndexBuffer(GameEngineIndexBuffer&& _other) noexcept = delete;

private:
	GameEngineIndexBuffer& operator=(const GameEngineIndexBuffer& _other) = delete;
	GameEngineIndexBuffer& operator=(const GameEngineIndexBuffer&& _other) = delete;

public:
	int GetIndexCount()
	{
		return IndexCount_;
	}

public:
	void Create(const std::vector<UINT>& _Datas, D3D11_USAGE _Usage);
	void Create(const void* _Data, size_t _Size, size_t _Count, D3D11_USAGE _Usage);

public:
	void Setting();

public:
};

