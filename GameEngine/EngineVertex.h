#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngine/GameEngineDevice.h>
#include <GameEngine/GameEngineShader.h>

class GameEngineLayOut
{
public:
	unsigned int LayOutOffset_;
	std::vector<std::string> SemanticName_;
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc_;

public:
	void AddInputLayOut(const char* _SemanticName, unsigned int _Index, unsigned int _AlignedByteOffset, DXGI_FORMAT _Format, unsigned int _InputSlot, unsigned int _InstanceDataStepRate, D3D11_INPUT_CLASSIFICATION _inputClass);
	ID3D11InputLayout* CreateLayOut(GameEngineShader* _Shader);
};

struct GameEngineVertex 
{
	static GameEngineLayOut LayOut;

public:
	float4 POSITION;
	float4 TEXTURECOORD;
	float4 COLOR;
	float4 NORMAL;
	float4 TANGENT;
	float4 BINORMAL;
	float4 Weight;
	int Index[4];
};