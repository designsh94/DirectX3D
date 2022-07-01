#pragma once
#include <GameEngineBase\GameEngineObjectNameBase.h>
#include "GameEngineRenderer.h"

struct CorrectResultColor 
{
	float4 vMulColor;
	float4 vPlusColor;
};

// 분류 : Image Renderer
// 용도 : 
// 설명 : 
class GameEngineTexture;
class GameEngineFolderTexture;
class GameEngineImageRenderer : public GameEngineRenderer
{
private:
	struct Animation2D : public GameEngineObjectNameBase
	{
	public:
		GameEngineFolderTexture* FolderTextures_;
		GameEngineTexture* AnimationTexture_;
		GameEngineImageRenderer* Renderer;

	public:
		float InterTime_;
		float CurTime_;

	public:
		bool IsEnd;
		bool Loop_;
		int CurFrame_;
		int StartFrame_;
		int EndFrame_;

	public:
		std::map<int, std::vector<std::function<void()>>> FrameCallBack_;
		std::vector<std::function<void()>> EndCallBack_;
		std::vector<std::function<void()>> StartCallBack_;

	public:
		void SetCurrentIndex(int _Index) 
		{
			CurFrame_ = _Index;
		}

	public:
		void Reset();
		void CallStart();
		void CallEnd();
		void CallFrame();
		void Update(float _DeltaTime);

	public:
		void FrameUpdate();
		void ReverseFrameUpdate();
	};

private:
	GameEngineTexture* CurTexture;
	Animation2D* CurAnimation_;

private:
	CorrectResultColor CorrectResultColor_;

protected:
	float4 CutData_;
	float4 ResultColor_;

private:
	bool IsPlay_;

private:
	std::map<std::string, Animation2D*> AllAnimations_;

public:
	GameEngineImageRenderer();
	~GameEngineImageRenderer();

protected:
	GameEngineImageRenderer(const GameEngineImageRenderer& _Other) = delete;
	GameEngineImageRenderer(GameEngineImageRenderer&& _Other) noexcept = delete;

private:
	GameEngineImageRenderer& operator=(const GameEngineImageRenderer& _Other) = delete;
	GameEngineImageRenderer& operator=(GameEngineImageRenderer&& _Other) noexcept = delete;

public:
	inline GameEngineTexture* GetCurrentTexture()
	{
		return CurTexture;
	}

	inline std::string GetCurrentAnimationName()
	{
		return CurAnimation_->GetName();
	}

	inline void IsCurrentAnimationIndex(const int _Index)
	{
		CurAnimation_->SetCurrentIndex(_Index);
	}

	inline bool IsCurrentAnimationString(const std::string& _Name)
	{
		return CurAnimation_->GetName() == _Name;
	}

	inline bool IsCurrentAnimationPtr(const char* _Name)
	{
		return CurAnimation_->GetName() == _Name;
	}

	inline void SetAlpha(float _Value)
	{
		CorrectResultColor_.vMulColor.a = _Value;
	}

	inline void SetPlusColor(float4 _Color)
	{
		CorrectResultColor_.vPlusColor = _Color;
	}

	inline void AnimationStop()
	{
		IsPlay_ = false;
	}

	inline void AnimationPlay()
	{
		IsPlay_ = true;
	}

public:
	void SetImage(const std::string& _ImageName, const std::string& _Sampler = "");
	void SetIndex(const int Index);

public:
	void CreateAnimation(const std::string& _TextureName, const std::string& _Name, int _StartFrame, int _EndFrame, float _InterTime, bool _Loop = true);
	void CreateAnimationFolder(const std::string& _FolderTexName, const std::string& _Name, float _InterTime, bool _Loop = true);

public:
	void SetChangeAnimation(const std::string& _Name, bool _IsForce = false);

public:
	void SetStartCallBack(const std::string& _Name, std::function<void()> _CallBack);
	void SetEndCallBack(const std::string& _Name, std::function<void()> _CallBack);
	void SetFrameCallBack(const std::string& _Name, int _Index, std::function<void()> _CallBack);

protected:
	void SetRenderingPipeLineSettingNext() override;
	void Update(float _DeltaTime) override;

private:
	void Start() override;
};

