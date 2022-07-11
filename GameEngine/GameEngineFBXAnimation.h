#pragma once
#include "GameEngineFBX.h"
#include "EngineVertex.h"
#include "GameEngineFBXMesh.h"

class FbxExBoneFrameData
{
public:
	float4 S; // 크기
	float4 Q; // 자전
	float4 T; // 이동
	double Time;
	float4x4 FrameMat;

	fbxsdk::FbxAMatrix GlobalAnimation;
	fbxsdk::FbxAMatrix LocalAnimation;
};

// 본의 프레임정보
// 해당 본의 인덱스, 본이 가지는 애니메이션 프레임데이터
class FbxExBoneFrame
{
public:
	int BoneIndex;
	int BoneParentIndex;

	// 120프레임이야.
	std::vector<FbxExBoneFrameData> BoneMatData;
};

// 애니메이션 정보
class FbxExAniData
{
public:
	std::string				AniName;			// 
	fbxsdk::FbxTime			StartTime;			// 설정된 애니메이션 시간
	fbxsdk::FbxTime			EndTime;			// 
	fbxsdk::FbxLongLong		TimeStartCount;		// 
	fbxsdk::FbxLongLong		TimeEndCount;		// 
	fbxsdk::FbxLongLong		FrameCount;			// 
	fbxsdk::FbxTime::EMode	TimeMode;			// 
	__int64					FbxModeCount;		// 
	double					FbxModeRate;		// 

	std::vector<FbxExBoneFrame> AniFrameData;	// Ex) 애니메이션에 100개의 본의 애니메이션 데이터가 있다.

public:
	float FrameTime(int _Frame)
	{
		fbxsdk::FbxTime tTime = {};
		tTime.SetFrame(_Frame, TimeMode);

		return (float)tTime.GetSecondDouble();
	}

public:
	__int64 FrameModeRateInt()
	{
		return static_cast<__int64>(fbxsdk::FbxGetFrameRate(TimeMode));
	}
	double FrameModeRateDouble()
	{
		return fbxsdk::FbxGetFrameRate(TimeMode);
	}

	float FrameModeRateFloat()
	{
		return static_cast<float>(fbxsdk::FbxGetFrameRate(TimeMode));
	}


public:
	FbxExAniData() :
		AniName(""), 
		StartTime(0), 
		EndTime(0), 
		TimeStartCount(0), 
		TimeEndCount(0),
		FrameCount(0),
		FbxModeCount(0), 
		FbxModeRate(0.0),
		TimeMode()
	{
	}

	~FbxExAniData() 
	{
	}
};

// 
class FBXAnimation;
class GameEngineFBXAnimation : public GameEngineFBX
{
	friend FBXAnimation;

private:
	std::vector<FbxExAniData> AnimationDatas;

public:
	GameEngineFBXAnimation();
	~GameEngineFBXAnimation();

protected:
	GameEngineFBXAnimation(const GameEngineFBXAnimation& _Other) = delete;
	GameEngineFBXAnimation(GameEngineFBXAnimation&& _Other) noexcept = delete;

private:
	GameEngineFBXAnimation& operator=(const GameEngineFBXAnimation& _Other) = delete;
	GameEngineFBXAnimation& operator=(GameEngineFBXAnimation&& _Other) noexcept = delete;

private:
	bool CheckAnimation();

public:
	inline size_t GetAnimationCount()
	{
		return AnimationDatas.size();
	}

	inline fbxsdk::FbxAMatrix GetGeometryTransformation(fbxsdk::FbxNode* _pNode)
	{
		// 노드에 존재하는 정보들을 종합해서 행렬로 리턴
		fbxsdk::FbxVector4 translation = _pNode->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
		fbxsdk::FbxVector4 rotation = _pNode->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
		fbxsdk::FbxVector4 scale = _pNode->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);

		return fbxsdk::FbxAMatrix(translation, rotation, scale);
	}

public:
	bool LoadAnimation();
	void Load(const std::string& _Path);

public:
	bool AnimationLoad(GameEngineFBXMesh* _Mesh, fbxsdk::FbxNode* _Node);
	
public:
	void CalFbxExBoneFrameTransMatrix(GameEngineFBXMesh* _Mesh);
	void ProcessAnimationLoad(GameEngineFBXMesh* _Mesh, fbxsdk::FbxNode* _pNode);	// 애니메이션 로드
	void ProcessAnimationCheckState(GameEngineFBXMesh* _Fbx);						// 애니메이션을 로드했으나 FbxExBoneFrame의 BoneMatData가 비어있는 경우 기본정보로 셋팅

public:
};

