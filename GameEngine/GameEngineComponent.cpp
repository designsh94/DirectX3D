#include "PreCompile.h"
#include "GameEngineComponent.h"
#include "GameEngineActor.h"

GameEngineComponent::GameEngineComponent() :
	Actor_(nullptr)
{
}

GameEngineComponent::~GameEngineComponent() 
{
}

void GameEngineComponent::InitComponent(GameEngineActor* _Actor)
{
	SetParent(_Actor);
	Actor_ = _Actor;
}

GameEngineLevel* GameEngineComponent::GetLevel()
{
	return Actor_->GetLevel();
}
