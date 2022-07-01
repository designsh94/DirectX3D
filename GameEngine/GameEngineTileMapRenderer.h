#pragma once
#include "GameEngineImageRenderer.h"

// 분류 : 타일맵 전용 렌더러
// 용도 : 
// 설명 : 
class GameEngineTexture;
class GameEngineFolderTexture;
class CameraComponent;
class GameEngineLevel;
class GameEngineRenderingPipeLine;
class GameEngineTileMapRenderer : public GameEngineImageRenderer
{
private:	// member Var

public:
	GameEngineTileMapRenderer();
	~GameEngineTileMapRenderer();

protected:		// delete constructer
	GameEngineTileMapRenderer(const GameEngineTileMapRenderer& _other) = delete;
	GameEngineTileMapRenderer(GameEngineTileMapRenderer&& _other) noexcept = delete;

private:		//delete operator
	GameEngineTileMapRenderer& operator=(const GameEngineTileMapRenderer& _other) = delete;
	GameEngineTileMapRenderer& operator=(const GameEngineTileMapRenderer&& _other) = delete;

private:
	void Start() override;
	void Render(float _DeltaTime) override;

public:
	void SetRenderGroup(int _Order) override;

};

