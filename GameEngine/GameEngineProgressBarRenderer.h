#pragma once
#include "GameEngineImageRenderer.h"
#include "Enums.h"

struct ProgressBarCBuffer
{
	float Percent;
	int PregressDirection;
	float Empty1;
	float Empty2;

	ProgressBarCBuffer() : Percent(1.f), PregressDirection(0), Empty1(0.f), Empty2(0.f)
	{
	}
};

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineTexture;
class GameEngineFolderTexture;
class CameraComponent;
class GameEngineLevel;
class GameEngineRenderingPipeLine;
class GameEngineProgressBarRenderer : public GameEngineImageRenderer
{
private:	// member Var
	ProgressBarCBuffer ProgressBarData_;

public:
	GameEngineProgressBarRenderer();
	~GameEngineProgressBarRenderer();

protected:		// delete constructer
	GameEngineProgressBarRenderer(const GameEngineProgressBarRenderer& _other) = delete;
	GameEngineProgressBarRenderer(GameEngineProgressBarRenderer&& _other) noexcept = delete;

private:		//delete operator
	GameEngineProgressBarRenderer& operator=(const GameEngineProgressBarRenderer& _other) = delete;
	GameEngineProgressBarRenderer& operator=(const GameEngineProgressBarRenderer&& _other) = delete;

private:
	void Start() override;
	void Render(float _DeltaTime) override;

public:
	void SetRenderGroup(int _Order) override;

public:
	void SetProgressBarDirect(int _ProgressBarDir);
	void SetPercent(float _Percent);
};

