#include "PreCompile.h"
#include "GameEngineTileMapRenderer.h"
#include "GameEngineLevel.h"
#include "CameraComponent.h"
#include "GameEngineTransform.h"
#include "GameEngineTextureManager.h"
#include "GameEngineFolderTextureManager.h"
#include "GameEngineFolderTexture.h"
#include "GameEngineWindow.h"

GameEngineTileMapRenderer::GameEngineTileMapRenderer()
{
}

GameEngineTileMapRenderer::~GameEngineTileMapRenderer()
{
}

void GameEngineTileMapRenderer::Start()
{
	GetLevel()->GetMainCamera()->PushRenderer(GetOrder(), this);

	SetRenderingPipeLine("TileMap");
}

void GameEngineTileMapRenderer::Render(float _DeltaTime)
{
	GameEngineRenderer::Render(_DeltaTime);
}

void GameEngineTileMapRenderer::SetRenderGroup(int _Order)
{
	GetLevel()->GetUICamera()->ChangeRendererGroup(_Order, this);
}
