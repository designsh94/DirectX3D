#pragma once
#include "GameEngineFBX.h"
#include "EngineVertex.h"

class FbxExBoneFrameData
{
public:
	float4 S; // ũ��
	float4 Q; // ����
	float4 T; // �̵�
	double Time;
	float4x4 FrameMat;

	fbxsdk::FbxAMatrix GlobalAnimation;
	fbxsdk::FbxAMatrix LocalAnimation;
};

// ���� ����������
// �ش� ���� �ε���, ���� ������ �ִϸ��̼� �����ӵ�����
class FbxExBoneFrame
{
public:
	int BoneIndex;
	int BoneParentIndex;

	// 120�������̾�.
	std::vector<FbxExBoneFrameData> BoneMatData;
};

// �ִϸ��̼� ����
class FbxExAniData
{
public:
	std::string				AniName;
	fbxsdk::FbxTime			StartTime; // ������ �ִϸ��̼� �ð�
	fbxsdk::FbxTime			EndTime; // 
	fbxsdk::FbxLongLong		TimeStartCount;
	fbxsdk::FbxLongLong		TimeEndCount;
	fbxsdk::FbxLongLong		FrameCount;
	fbxsdk::FbxTime::EMode	TimeMode;
	__int64					FbxModeCount;
	double					FbxModeRate;

	std::vector<FbxExBoneFrame> m_AniFrameData;	// Ex) �ִϸ��̼ǿ� 100���� ���� �ִϸ��̼� �����Ͱ� �ִ�.

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
	FbxExAniData() : AniName(""), StartTime(0), EndTime(0), TimeStartCount(0), TimeEndCount(0), FrameCount(0), FbxModeCount(0), FbxModeRate(0.0)
	{
	}

	~FbxExAniData() 
	{
	}
};

// 
class GameEngineFBXAnimation : public GameEngineFBX
{
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
	bool LoadAnimation();
	void Load(const std::string& _Path);

public:
};

