#include "PreCompile.h"
#include "FBXLevel.h"

#include <GameEngine/GameEngineFBXWindow.h>
#include <GameEngine/GameEngineRenderWindow.h>

#include <GameEngine/LightActor.h>
#include <GameEngine/MouseActor.h>
#include <GameEngine/SKySphereActor.h>

#include "UserGame.h"
#include "Map.h"

FBXLevel::FBXLevel() :
	Mouse_(nullptr)
{
}

FBXLevel::~FBXLevel()
{
}

void FBXLevel::CreateActorLevel()
{
	static bool Check = false;

	if (true == Check)
	{
		return;
	}

	// 마우스
	Mouse_ = CreateActor<MouseActor>();
	Mouse_->GetUIRenderer()->SetRenderGroup(1000);

	// 조명
	LightActor* Light1 = CreateActor<LightActor>();
	Light1->GetLight()->SetDiffusePower(0.3f);
	Light1->GetLight()->SetSpacularLightPow(50.0f);

	LightActor* Light2 = CreateActor<LightActor>();
	Light2->GetLight()->SetDiffusePower(0.3f);
	Light2->GetLight()->SetSpacularLightPow(50.0f);
	Light2->GetTransform()->SetLocalRotationDegree({ 45.0f, 0.0f, 0.0f });

	// 바닥
	Map* Floor = CreateActor<Map>();

	// 배경
	SKySphereActor* BackGround = CreateActor<SKySphereActor>();
}

void FBXLevel::LevelStart()
{
	// Create FBX Window And FBX Files Path Set
	GameEngineFBXWindow* ImageWindow = GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineFBXWindow>("FBXWindow");
	ImageWindow->FBXFolder.MoveParent("DirectX3D");
	ImageWindow->FBXFolder.MoveChild("EngineResources");
	ImageWindow->FBXFolder.MoveChild("FBX");

	// MainCam Mode And Position Set
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -300.0f));

	// FreeCam Key Set
	if (false == GameEngineInput::GetInst().IsKey("FreeCameraOn"))
	{
		GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'O');
	}
}

void FBXLevel::LevelUpdate(float _DeltaTime)
{
	static bool CreateActorCheck = false;

	if (0 >= UserGame::LoadingFolder && false == CreateActorCheck)
	{
		CreateActorLevel();
		CreateActorCheck = true;
	}

	static bool Check = false;
	if (false == Check && nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("UI 카메라 타겟", GetUICamera()->GetCameraRenderTarget(), Size * 3);
		Check = true;
	}

	if (true == GameEngineInput::GetInst().Down("FreeCameraOn"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}

	//if (true == GetMainCameraActor()->IsFreeCameraMode())
	//{
	//	return;
	//}
}

void FBXLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void FBXLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
}
