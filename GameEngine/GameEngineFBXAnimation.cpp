#include "PreCompile.h"
#include "GameEngineFBXAnimation.h"

GameEngineFBXAnimation::GameEngineFBXAnimation()
{
}

GameEngineFBXAnimation::~GameEngineFBXAnimation()
{
}

void GameEngineFBXAnimation::Load(const std::string& _Path) 
{
	if (false == CreateFBXSystemInitialize(_Path))
	{
		GameEngineDebug::MsgBoxError("FBX �̴ϼȶ���� �����߽��ϴ�.");
		return;
	}

	FBXConvertScene();
}

bool GameEngineFBXAnimation::LoadAnimation()
{
	if (false == CheckAnimation())
	{
		return false;
	}

	return true;
}

bool GameEngineFBXAnimation::CheckAnimation()
{
	fbxsdk::FbxArray<FbxString*> AniNameArray;
	Scene->FillAnimStackNameArray(AniNameArray);

	if (0 == AniNameArray.Size())
	{
		return false;
	}

	AnimationDatas.resize(AniNameArray.Size());
	for (unsigned int i = 0; i < AnimationDatas.size(); i++)
	{
		fbxsdk::FbxAnimStack* pAniStatic = Scene->FindMember<fbxsdk::FbxAnimStack>(AniNameArray[i]->Buffer());

		if (nullptr == pAniStatic)
		{
			GameEngineDebug::MsgBoxError("if (nullptr == pAniStatic)");
			continue;
		}

		FbxTakeInfo* TakeInfo = Scene->GetTakeInfo(pAniStatic->GetName());

		AnimationDatas[i].StartTime = TakeInfo->mLocalTimeSpan.GetStart();
		AnimationDatas[i].EndTime = TakeInfo->mLocalTimeSpan.GetStop();

		// �ִϸ��̼��� ������ �ƴ��� �ʷ� �ƴ��� ���뼼����� �ƴ��� ������ ������� ����ƴ��� ���
		AnimationDatas[i].TimeMode = Scene->GetGlobalSettings().GetTimeMode();
		AnimationDatas[i].TimeStartCount = AnimationDatas[i].StartTime.GetFrameCount(AnimationDatas[i].TimeMode);

		if (0 >= AnimationDatas[i].TimeStartCount)
		{
			AnimationDatas[i].TimeStartCount *= (FbxLongLong)-1;
		}

		AnimationDatas[i].TimeEndCount = AnimationDatas[i].EndTime.GetFrameCount(AnimationDatas[i].TimeMode);
		AnimationDatas[i].FrameCount = AnimationDatas[i].TimeEndCount - AnimationDatas[i].TimeStartCount;

		AnimationDatas[i].FbxModeCount = (long long)fbxsdk::FbxTime::GetFrameRate(AnimationDatas[i].TimeMode);
		AnimationDatas[i].FbxModeRate = (double)fbxsdk::FbxTime::GetFrameRate(AnimationDatas[i].TimeMode);
	}

	return true;
}
