#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include "Map.h"

Map::Map()
{
}

Map::~Map()
{
}

void Map::Start()
{
	GetTransform()->SetWorldPosition({ 0.0f, -50.0f, 0.0f });

	GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	Renderer->SetRenderingPipeLine("Color");
	Renderer->SetMesh("Box");

	Renderer->GetTransform()->SetLocalScaling({ 1000.0f, 10.0f, 1000.0f });
	Renderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	Renderer->ShaderHelper.SettingConstantBufferSet("ResultColor", float4(0.0f, 0.2f, 0.0f));
}
