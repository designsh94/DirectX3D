#pragma once
#include "GameEngineActor.h"
#include "GameEngineUIRenderer.h"

// 분류 : 
// 용도 : 
// 설명 : 
class MouseActor : public GameEngineActor
{
private:
	GameEngineUIRenderer* UIRenderer;

public:
	MouseActor();
	~MouseActor();

protected:
	MouseActor(const MouseActor& _Other) = delete;
	MouseActor(MouseActor&& _Other) noexcept = delete;

private:
	MouseActor& operator=(const MouseActor& _Other) = delete;
	MouseActor& operator=(MouseActor&& _Other) noexcept = delete;

public:
	GameEngineUIRenderer* GetUIRenderer()
	{
		return UIRenderer;
	}

public:
	void SetCurSor(std::string _Name);
	void SetCurSor(std::string _Name, int _Index);

public:
	void WindowCursorOn();
	void WindowCursorOff();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
};

