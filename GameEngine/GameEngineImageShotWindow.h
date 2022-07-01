#pragma once
#include "GameEngineGUI.h"
#include "GameEngineRenderTarget.h"

class GameEngineImageShotWindow : public GameEngineGUIWindow
{
public:
	struct RenderTargetDrawData
	{
	public:
		std::string Name;
		GameEngineRenderTarget* Target;
		float4 Size_;
		bool ScaleCheck;
	};

private:
	ID3D11ShaderResourceView* View_;
	float4 Size_;
	std::vector<RenderTargetDrawData> DrawRenderTarget_;

public:
	GameEngineImageShotWindow();
	~GameEngineImageShotWindow();

public:
	GameEngineImageShotWindow(const GameEngineImageShotWindow& _Other) = delete;
	GameEngineImageShotWindow(GameEngineImageShotWindow&& _Other) noexcept = delete;
	GameEngineImageShotWindow& operator=(const GameEngineImageShotWindow& _Other) = delete;
	GameEngineImageShotWindow& operator=(GameEngineImageShotWindow&& _Other) noexcept = delete;

protected:
	void Start() override;
	void OnGUI() override;

public:
	void ClaerRenderTarget() 
	{
		DrawRenderTarget_.clear();
	}

	void PushRenderTarget(std::string _Name, GameEngineRenderTarget* _RenderTarget, float4 Size_)
	{
		DrawRenderTarget_.push_back({ _Name, _RenderTarget, Size_, false });
	}

	void SetView(ID3D11ShaderResourceView* _View, float4 _Size)
	{
		Size_ = _Size;
		View_ = _View;
	}
};

