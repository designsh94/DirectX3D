#pragma once
#include "GameEngineGUI.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineFBXMeshManager.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>

class GameEngineFBXWindow : public GameEngineGUIWindow
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

public:
	GameEngineDirectory FBXFolder;

public:
	int FBXFileSelect;
	int ActorSelect;

public:
	std::vector<std::string> ActorsNames;
	std::vector<std::string> OriNames;
	std::vector<std::string> Names;
	std::vector<GameEngineActor*> Actors;

public:
	GameEngineFBXWindow();
	~GameEngineFBXWindow();

public:
	GameEngineFBXWindow(const GameEngineFBXWindow& _Other) = delete;
	GameEngineFBXWindow(GameEngineFBXWindow&& _Other) noexcept = delete;
	GameEngineFBXWindow& operator=(const GameEngineFBXWindow& _Other) = delete;
	GameEngineFBXWindow& operator=(GameEngineFBXWindow&& _Other) noexcept = delete;

protected:
	void OnGUI() override;

private:
	void ActorControl();

};

