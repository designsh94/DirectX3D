#pragma once
#include "GameEngineImageRenderer.h"

// 분류 : UI Renderer
// 용도 : 깊이버퍼를 사용하지않는 렌더러
// 설명 : 하나의 랜더 단위를 표현
class CameraComponent;
class GameEngineLevel;
class GameEngineRenderingPipeLine;
class GameEngineUIRenderer : public GameEngineImageRenderer
{
private:
	friend GameEngineLevel;
	friend CameraComponent;

protected:
	static int UIRendererCount;
	static GameEngineRenderTarget* FontTarget_;

protected:
	bool IsText_;
	std::string FontName_;
	std::string PrintText_;
	float FontSize_;
	float4 FontPivot_;
	float4 Color_;
	unsigned int Flags_;
	int MaxLen_;

private:
	float4 CuttingUV_;

public:
	GameEngineUIRenderer();
	~GameEngineUIRenderer();

protected:
	GameEngineUIRenderer(const GameEngineUIRenderer& _Other) = delete;
	GameEngineUIRenderer(GameEngineUIRenderer&& _Other) noexcept = delete;

private:
	GameEngineUIRenderer& operator=(const GameEngineUIRenderer& _Other) = delete;
	GameEngineUIRenderer& operator=(GameEngineUIRenderer&& _Other) noexcept = delete;

public:
	std::string GetPrintText() const
	{
		return PrintText_;
	}

	int GetPrintTextLen() const
	{
		return static_cast<int>(PrintText_.length());
	}

private:
	void Start() override;
	void Render(float _DeltaTime) override;

public:
	void SetRenderGroup(int _Order) override;

public:
	void TextSetting(std::string _FontName, std::string _PrintText, float _FontSize, unsigned int _Flags = 0, float4 _Color = float4::WHITE, const float4& _FontPivot = float4::ZERO, int _MaxLen = 15);
	bool AddText(std::string _PrintText);
	bool DelText();
	void AllDelText();
	void SetPrintText(const std::string& _PrintText);
	void SetTextColor(const float4& _Color);
};

