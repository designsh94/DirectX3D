#include "PreCompile.h"
#include <GameEngineBase\GameEngineTime.h>

#include "GameEngineCore.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"

#include "GameEngineLevelControlWindow.h"

GameEngineLevelControlWindow::GameEngineLevelControlWindow() 
{
}

GameEngineLevelControlWindow::~GameEngineLevelControlWindow() 
{
}

void GameEngineLevelControlWindow::OnGUI() 
{
	static float Acc = 1.0f;
	static std::string FrameText = "Frame : " + std::to_string(1.0f / GameEngineTime::GetInst().GetDeltaTime());

	Acc -= GameEngineTime::GetInst().GetDeltaTime();
	if (0 >= Acc)
	{
		FrameText = "Frame : " + std::to_string(1.0f / GameEngineTime::GetInst().GetDeltaTime());
		Acc = 1.0f;
	}
	ImGui::Text(FrameText.c_str());

	int Count = static_cast<int>(GameEngineCore::AllLevel_.size());

	for (auto& Level : GameEngineCore::AllLevel_)
	{
		if (true == ImGui::Button(Level.first.c_str()))
		{
			GameEngineCore::LevelChange(Level.first);
		}

		--Count;

		if (Count != 0)
		{
			ImGui::SameLine();
		}
	}
}

