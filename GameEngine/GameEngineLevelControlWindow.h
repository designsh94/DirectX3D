#pragma once
#include "GameEngineGUI.h"
#include "GameEngineRenderTarget.h"

class GameEngineLevelControlWindow : public GameEngineGUIWindow
{
private:

public:
	GameEngineLevelControlWindow();
	~GameEngineLevelControlWindow();

public:
	GameEngineLevelControlWindow(const GameEngineLevelControlWindow& _Other) = delete;
	GameEngineLevelControlWindow(GameEngineLevelControlWindow&& _Other) noexcept = delete;
	GameEngineLevelControlWindow& operator=(const GameEngineLevelControlWindow& _Other) = delete;
	GameEngineLevelControlWindow& operator=(GameEngineLevelControlWindow&& _Other) noexcept = delete;

protected:
	void OnGUI() override;

public:
};

