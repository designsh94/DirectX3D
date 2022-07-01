#include "PreCompile.h"
#include "UserGame.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameEngine/GameEngineRenderWindow.h>

#include "FBXLevel.h"

std::atomic<int> UserGame::LoadingFolder = 0;

UserGame::UserGame()
{
}

UserGame::~UserGame()
{
}

UserGame::UserGame(UserGame&& _other) noexcept
{
}

void UserGame::Initialize()
{
	GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineLevelControlWindow>("LevelControlWindow");
	GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineRenderWindow>("RenderWindow");

	LevelCreate<FBXLevel>("FBXLevel");
	LevelChange("FBXLevel");

	return;
}

void UserGame::Release()
{
}
