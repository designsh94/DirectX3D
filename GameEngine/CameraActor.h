#pragma once
#include "GameEngineActor.h"
#include "CameraComponent.h"

// 분류 : 
// 용도 : 
// 설명 : 
class CameraComponent;
class CameraActor : public GameEngineActor
{
private:
	CameraComponent* Camera_;
	GameEngineTransform OriginTransform;
	ProjectionMode PrevCamMode_;
	float FreeCameraSpeed_;
	float RotSpeed_;
	bool IsFreeCameraMode_;

public:
	CameraActor();
	~CameraActor();

protected:
	CameraActor(const CameraActor& _Other) = delete;
	CameraActor(CameraActor&& _Other) noexcept = delete;

private:
	CameraActor& operator=(const CameraActor& _Other) = delete;
	CameraActor& operator=(CameraActor&& _Other) noexcept = delete;

public:
	CameraComponent* GetCamera() 
	{
		return Camera_;
	}

public:
	float4x4 GetViewMatrix();
	float4x4 GetProjectionMatrix();
	bool IsFreeCameraMode();

public:
	void FreeCameraModeSwitch();
	void FreeCameraModeOn();
	void FreeCameraModeOff();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
};

