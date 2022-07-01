#pragma once
#include "GameEnginePostProcessRender.h"

enum FadeOption
{
	LIGHT,
	DARK,
};

struct FadeData
{
	float CurTime;// 1
	float Ratio; // 
	int Clip; // 1 투명도가 0인 애들은 안한다.
			  // 0 투명도가 0인 애들은 한다.

	float Time;

	FadeData() :
		Clip(1),
		Ratio(0.0f),
		CurTime(0.0f),
		Time(0.0f)
	{
	}
};

// 분류 : 
// 용도 : 
// 설명 : 
class PostFade : public GameEnginePostProcessRender
{
private:
	FadeData Data;
	FadeOption Option;

public:
	PostFade();
	~PostFade();

protected:
	PostFade(const PostFade& _Other) = delete;
	PostFade(PostFade&& _Other) noexcept = delete;

private:
	PostFade& operator=(const PostFade& _Other) = delete;
	PostFade& operator=(PostFade&& _Other) noexcept = delete;

protected:
	void Initialize() override;
	void Effect(float _DeltaTime) override;

public:
	void SetData(float _Time, FadeOption _Option);

public:
};

