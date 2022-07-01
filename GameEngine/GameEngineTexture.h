#pragma once
#include "GameEngineDevice.h"
#include "ThirdParty/Include/DirectXTex/DirectXTex.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineTexture : public GameEngineObjectNameBase
{
private:
	D3D11_TEXTURE2D_DESC TextureDesc_;
	ID3D11Texture2D* Texture2D_;
	ID3D11RenderTargetView* RenderTargetView_;
	ID3D11ShaderResourceView* ShaderResourceView_;
	ID3D11DepthStencilView* DepthStencilView_;
	DirectX::ScratchImage Image_;

private:
	std::vector<float4> CutList_;

public:
	GameEngineTexture();
	~GameEngineTexture();

protected:
	GameEngineTexture(const GameEngineTexture& _other) = delete;
	GameEngineTexture(GameEngineTexture&& _other) noexcept = delete;

private:
	GameEngineTexture& operator=(const GameEngineTexture& _other) = delete;
	GameEngineTexture& operator=(const GameEngineTexture&& _other) = delete;

public:
	inline ID3D11DepthStencilView* GetDepthStencilView()
	{
		return DepthStencilView_;
	}

	inline ID3D11RenderTargetView* GetRenderTargetView()
	{
		return RenderTargetView_;
	}

	inline ID3D11ShaderResourceView** GetShaderResourcesView()
	{
		return &ShaderResourceView_;
	}

	inline float4 GetTextureSize()
	{
		return float4(static_cast<float>(TextureDesc_.Width), static_cast<float>(TextureDesc_.Height));
	}

public:
	bool IsCut();
	void Cut(int _x, int _y);
	void PushCutIndex(const float4& _Size, const float4& _Pos);
	float4 GetCutData(int _Index);
	float4 GetPixel(int _X, int _y);

public:
	ID3D11RenderTargetView* CreateRenderTargetView();
	ID3D11ShaderResourceView* CreateShaderResourceView();
	ID3D11DepthStencilView* CreateDepthStencilView();

public:
	void Create(ID3D11Texture2D* _Texture2D);
	void Create(float4 _TextureSize, DXGI_FORMAT _Format, D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT, unsigned int _BindFlag = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
	void Create(D3D11_TEXTURE2D_DESC _Desc);
	void Load(const std::string& _Path);

public:
};

