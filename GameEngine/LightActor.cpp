#include "PreCompile.h"
#include "LightActor.h"

LightActor::LightActor() :
	Light_(nullptr)
{
}

LightActor::~LightActor() 
{
}

void LightActor::Start() 
{
	Light_ = CreateTransformComponent<GameEngineLightComponent>(GetTransform());
}

void LightActor::Update(float _DeltaTime) 
{
}
