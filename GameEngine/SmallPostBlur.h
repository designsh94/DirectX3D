#pragma once
#include "GameEnginePostProcessRender.h"

struct SmallBlurData 
{
	int FilterStartX; // -2
	int FilterStartY; // -2
	int FilterEndX; // 2
	int FilterEndY; // 2
	int FilterCount;
	float FilterSum;
	float ImagePixelUVX;
	float ImagePixelUVY;
	float FilterPixelX;
	float FilterPixelY;
	float Temp0;
	float Temp1;
};

// 분류 : 
// 용도 : 
// 설명 : 
class SmallPostBlur : public GameEnginePostProcessRender
{
private:
	SmallBlurData  Data;
	GameEngineTexture* Filter;

public:
	SmallPostBlur();
	~SmallPostBlur();

protected:
	SmallPostBlur(const SmallPostBlur& _Other) = delete;
	SmallPostBlur(SmallPostBlur&& _Other) noexcept = delete;

private:
	SmallPostBlur& operator=(const SmallPostBlur& _Other) = delete;
	SmallPostBlur& operator=(SmallPostBlur&& _Other) noexcept = delete;

protected:
	void Initialize() override;
	void Effect(float _DeltaTime) override;

public:
	void SetFilter(std::string _TextureName);

public:
};

