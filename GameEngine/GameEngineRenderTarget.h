#pragma once
#include "GameEngineDevice.h"
#include "GameEngineShaderResHelper.h"

class GameEngineTexture;
class GameEngineRenderTarget : public GameEngineObjectNameBase
{
private:
	static GameEngineRenderTarget* LastRenderTarget;
	static GameEngineDepthBuffer* LastDepthBuffer;

public:
	static GameEngineRenderTarget* GetLastRenderTarget()
	{
		return LastRenderTarget;
	}
	static GameEngineDepthBuffer* GetLastDepthBuffer()
	{
		return LastDepthBuffer;
	}

private:
	std::vector<GameEngineTexture*> ReleaseTextures_;
	std::vector<GameEngineTexture*> Textures_;
	std::vector<ID3D11RenderTargetView*> RenderTargetViews_;
	std::vector<ID3D11ShaderResourceView*> ShaderResourcesViews_;
	std::vector<float4> ClearColor_;

private:
	GameEngineDepthBuffer* DepthBuffer_;
	GameEngineRenderingPipeLine* Pipe_;
	GameEngineShaderResHelper Res_;

public:
	inline ID3D11ShaderResourceView* GetShaderResourcesView(size_t _Index)
	{
		return ShaderResourcesViews_[_Index];
	}

	inline float4 GetTextureSize(size_t _Index)
	{
		return Textures_[_Index]->GetTextureSize();
	}

	inline GameEngineTexture* GetTexture(size_t _Index)
	{
		return Textures_[_Index];
	}

public:
	GameEngineRenderTarget();
	~GameEngineRenderTarget();

protected:
	GameEngineRenderTarget(const GameEngineRenderTarget& _other) = delete;
	GameEngineRenderTarget(GameEngineRenderTarget&& _other) noexcept;

private:
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget& _other) = delete;
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget&& _other) = delete;

public:
	void Clear(bool _Death = true);
	void Setting(int _Index = -1);

public:
	void Create(const std::string _TextureName, float4 _ClearColor);
	void Create(float4 _Scale, float4 _ClearColor);
	void Create(GameEngineTexture* _Texture, float4 _ClearColor);
	void CreateDepthBuffer(float4 _Scale);

public:
	void Merge(GameEngineRenderTarget* _Other, int _Index = 0);
	void Copy(GameEngineRenderTarget* _Other);
};

