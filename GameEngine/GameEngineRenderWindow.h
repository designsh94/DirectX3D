#pragma once
#include "GameEngineGUI.h"
#include "GameEngineRenderTarget.h"

class GameEngineRenderWindow : public GameEngineGUIWindow
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
	std::vector<RenderTargetDrawData> DrawRenderTarget_;
	std::list<class GameEngineImageShotWindow*> ImageShot_;

public:
	GameEngineRenderWindow();
	~GameEngineRenderWindow();

public:
	GameEngineRenderWindow(const GameEngineRenderWindow& _Other) = delete;
	GameEngineRenderWindow(GameEngineRenderWindow&& _Other) noexcept = delete;
	GameEngineRenderWindow& operator=(const GameEngineRenderWindow& _Other) = delete;
	GameEngineRenderWindow& operator=(GameEngineRenderWindow&& _Other) noexcept = delete;

protected:
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
};

