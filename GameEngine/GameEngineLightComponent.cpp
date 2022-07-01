#include "PreCompile.h"
#include "GameEngineLightComponent.h"

GameEngineLightComponent::GameEngineLightComponent() 
{
	LightDataObject.DiffuseLightColor = float4::ONE;
	LightDataObject.AmbientLightColor = float4::ONE;
	LightDataObject.SpacularLightColor = float4::ONE;
	LightDataObject.AmbientLight = float4(0.1f, 0.1f, 0.1f, 0.1f);
	LightDataObject.SpacularLightPow.x = 50.0f;
	LightDataObject.LightPower = float4::ONE;
}

GameEngineLightComponent::~GameEngineLightComponent() 
{
}

void GameEngineLightComponent::Start() 
{
	GetLevel()->GetMainCamera()->PushLight(this);
}

void GameEngineLightComponent::Update(float _DeltaTime)
{
	LightDataObject.ViewLightDir = GetTransform()->GetWorldForwardVector();
	LightDataObject.ViewLightPosition = GetTransform()->GetWorldPosition();
}
