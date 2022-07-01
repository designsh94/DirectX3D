#include "PreCompile.h"
#include "SKySphereActor.h"

#include "GameEngineRenderer.h"
#include "GameEngineLevel.h"
#include "CameraActor.h" 

SKySphereActor::SKySphereActor() :
	Renderer(nullptr),
	Data{}
{
}

SKySphereActor::~SKySphereActor() 
{
}

void SKySphereActor::SetRadius(float _Radius)
{
	Renderer->GetTransform()->SetLocalScaling({ _Radius * 2.0f, _Radius * 2.0f, _Radius * 2.0f });
}

void SKySphereActor::Start() 
{
	Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	Renderer->SetRenderingPipeLine("Texture");
	Renderer->SetMesh("Sphere");
	Renderer->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerFront");
	Renderer->ShaderHelper.SettingConstantBufferSet("TextureResultColor", Data);
	Renderer->ShaderHelper.SettingTexture("Tex", "EngineSky.jpg");

	Renderer->GetTransform()->SetLocalScaling({ 100.0f, 100.0f, 100.0f });
	Renderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
}

void SKySphereActor::Update(float _DeltaTime)
{
	float Far = GetLevel()->GetMainCamera()->GetFar();
	Far *= 0.98f;
	SetRadius(Far);

	float4 WorldPos = GetLevel()->GetMainCameraActor()->GetTransform()->GetWorldPosition();
	GetTransform()->SetWorldPosition(WorldPos);
}
