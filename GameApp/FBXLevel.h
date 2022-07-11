#pragma once
#include <GameEngine/GameEngineLevel.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineFBXWindow;
class MouseActor;
class FBXLevel : public GameEngineLevel
{
private:	// member Var
	GameEngineFBXWindow* ImageWindow;
	MouseActor* Mouse_;

public:
	FBXLevel();
	~FBXLevel();

protected:		// delete constructer
	FBXLevel(const FBXLevel& _other) = delete;
	FBXLevel(FBXLevel&& _other) noexcept = delete;

private:		//delete operator
	FBXLevel& operator=(const FBXLevel& _other) = delete;
	FBXLevel& operator=(const FBXLevel&& _other) = delete;

private:
	void CreateActorLevel();

private:
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

public:
};

