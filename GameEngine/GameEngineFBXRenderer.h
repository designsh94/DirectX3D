#pragma once
#include "GameEngineRendererBase.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

class RenderSet 
{
public:
	GameEngineShaderResHelper* ShaderHelper;
	GameEngineRenderingPipeLine* PipeLine_;
};

// 설명 : 
class FbxExAniData;
class GameEngineFBXRenderer;
class GameEngineFBXMesh;
class GameEngineFBXAnimation;
class FBXAnimation
{
public:
	GameEngineFBXMesh* Mesh;
	GameEngineFBXAnimation* Animation;
	GameEngineFBXRenderer* ParentRenderer;
	FbxExAniData* PixAniData;

public:
	float CurFrameTime;
	float FrameTime;
	UINT CurFrame;
	UINT End;
	UINT Start;

public:
	void Init();
	void Update(float _DeltaTime);
};

// 설명 : 하나의 랜더 단위를 표현
class CameraComponent;
class GameEngineLevel;
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineRenderingPipeLine;
class GameEngineFBXRenderer : public GameEngineRendererBase
{
	friend FBXAnimation;
	friend GameEngineLevel;
	friend CameraComponent;

private:
	GameEngineFBXMesh* FBXMesh;
	std::vector<RenderSet> RenderSets;

private: // 애니메이션관련
	FBXAnimation* CurAnimation;
	std::vector<float4x4> BoneData;
	std::map<std::string, FBXAnimation*> Animations;

public:
	GameEngineFBXRenderer();
	~GameEngineFBXRenderer();

protected:
	GameEngineFBXRenderer(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer(GameEngineFBXRenderer&& _Other) noexcept = delete;

private:
	GameEngineFBXRenderer& operator=(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer& operator=(GameEngineFBXRenderer&& _Other) noexcept = delete;

private:
	void Update(float _DeltaTime) override;

public:
	size_t GetRenderSetCount()
	{
		return RenderSets.size();
	}

	RenderSet& GetRenderSet(unsigned int _Index)
	{
		return RenderSets[_Index];
	}

public:
	void SetFBXMesh(const std::string& _Value, std::string _PipeLine);

protected:
	void Start() override;
	void Render(float _DeltaTime) override;

public: // Animation Function
	void CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName);
	void ChangeFBXAnimation(const std::string& _AnimationName);

public:
};

