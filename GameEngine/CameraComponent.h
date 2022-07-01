#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineDebugRenderData.h"
#include "GameEngineLightComponent.h"
#include "Enums.h"

enum class ProjectionMode
{
	Perspective,
	Orthographic
};

// 분류 : 카메라 컴포넌트
// 용도 : 
// 설명 : 
class GameEngineRendererBase;
class CameraComponent : public GameEngineTransformComponent
{
	friend class CameraActor;
	friend class GameEngineLevel;
	friend class GameEngineRenderer;

private:
	ProjectionMode				ProjectionMode_;			// 
	float						FovAngleY_;					// 
	float4						CamSize_;					// 
	float						NearZ_;						// 
	float						FarZ_;						// 
	float						ZoomValue;

private:
	LightsData LightData_;
	std::list<GameEngineLightComponent*> Lights_;
	std::map<int, std::list<GameEngineRendererBase*>> RendererList_;

private:
	int DebugRenderCount_;
	float ZoomValue_;
	std::vector<GameEngineDebugRenderData> DebugVector_;
	GameEngineRenderTarget* CameraBufferTarget_;

public:
	CameraComponent();
	~CameraComponent();

protected:
	CameraComponent(const CameraComponent& _other) = delete;
	CameraComponent(CameraComponent&& _other) noexcept = delete;

private:
	CameraComponent& operator=(const CameraComponent& _other) = delete;
	CameraComponent& operator=(const CameraComponent&& _other) = delete;

private:
	void ClearCameraTarget();
	void CameraTransformUpdate();
	void Render(float _DeltaTime);
	void DebugRender();
	void ReleaseRenderer();

private:
	void NextLevelMoveRenderer(CameraComponent* _NextCamera, GameEngineActor* _Actor);

public:
	inline const LightsData& GetLightData() const
	{
		return LightData_;
	}

	inline GameEngineRenderTarget* GetCameraRenderTarget()
	{
		return CameraBufferTarget_;
	}

	inline float GetZoomValue()
	{
		return ZoomValue_;
	}

	inline float GetFar() 
	{
		return FarZ_;
	}

	inline const LightsData& LightData() const
	{
		return LightData_;
	}

public:
	void CameraZoomReset();
	void CameraZoomSetting(float _Value);

public:
	void SetProjectionMode(ProjectionMode _ProjectionMode);
	void PushRenderer(int _Order, GameEngineRendererBase* _Renderer);
	void PushLight(GameEngineLightComponent* _Light);

public:
	void PushDebugRender(GameEngineTransform* _Trans, CollisionType _Type);
	void ChangeRendererGroup(int _Group, GameEngineRendererBase* _Renderer);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
};

