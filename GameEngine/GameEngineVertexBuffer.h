#pragma once
#include "GameEngineDevice.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineVertexBuffer : public GameEngineObjectNameBase
{
private:
	ID3D11Buffer* Buffer_;
	UINT Size_;
	UINT Count_;
	UINT Offset_;
	D3D11_BUFFER_DESC BufferData_;
	D3D11_SUBRESOURCE_DATA ResData_;

public:
	std::vector<D3D11_INPUT_ELEMENT_DESC>* InputLayoutDesc_;

public:
	GameEngineVertexBuffer();
	~GameEngineVertexBuffer();

protected:
	GameEngineVertexBuffer(const GameEngineVertexBuffer& _other) = delete;
	GameEngineVertexBuffer(GameEngineVertexBuffer&& _other) noexcept;

private:
	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer& _other) = delete;
	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer&& _other) = delete;

private:
	void Create(const void* _Data, size_t _Datas, size_t _Count, D3D11_USAGE _Usage);

public:
	template<typename VertexType>
	void Create(const std::vector<VertexType>& _Datas, D3D11_USAGE _Usage)
	{
		InputLayoutDesc_ = &(VertexType::LayOut.InputLayoutDesc_);

		Create(reinterpret_cast<const void*>(&_Datas[0]), sizeof(VertexType), _Datas.size(), _Usage);
	}

public:
	void Setting();

public:
};



