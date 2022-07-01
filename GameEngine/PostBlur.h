#pragma once
#include "GameEnginePostProcessRender.h"

struct BlurData 
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
class PostBlur : public GameEnginePostProcessRender
{
private:
	BlurData Data;
	GameEngineTexture* Filter;

public:
	PostBlur();
	~PostBlur();

protected:
	PostBlur(const PostBlur& _Other) = delete;
	PostBlur(PostBlur&& _Other) noexcept = delete;

private:
	PostBlur& operator=(const PostBlur& _Other) = delete;
	PostBlur& operator=(PostBlur&& _Other) noexcept = delete;

public:
	void SetFilter(std::string _TextureName);

protected:
	void Effect(float _DeltaTime) override;
	void Initialize() override;

public:
};

