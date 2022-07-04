#include "PreCompile.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineFBXMeshManager.h"

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
		GameEngineDebug::MsgBoxError("FBX 이니셜라이즈에 실패했습니다.");
		return;
	}

	FBXConvertScene();
	CheckAnimation();
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
		AnimationDatas[i].AniName = GameEngineString::UTF8ToAnsiReturn(AniNameArray[i]->Buffer());

		FbxTakeInfo* TakeInfo = Scene->GetTakeInfo(AnimationDatas[i].AniName.c_str());

		AnimationDatas[i].StartTime = TakeInfo->mLocalTimeSpan.GetStart();
		AnimationDatas[i].EndTime = TakeInfo->mLocalTimeSpan.GetStop();

		// 애니메이션이 분으로 됐는지 초로 됐는지 나노세컨드로 됐는지 프레임 기반으로 저장됐는지 등등
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

	// fbxsdk::FbxArray<FbxString*>의 사용한 메모리를 직접 제거해야한다.
	for (int i = 0; i < AniNameArray.GetCount(); i++)
	{
		delete AniNameArray[i];
	}
	AniNameArray.Clear();

	return true;
}

bool GameEngineFBXAnimation::AnimationLoad(GameEngineFBXMesh* _Mesh, fbxsdk::FbxNode* Node)
{
	//std::vector<FbxExBoneFrame> vecAniFrameData;
	//vecAniFrameData.resize(_Fbx->GetBoneCount());

	//fbxsdk::FbxMesh* pCurrMesh = pNode->GetMesh();
	//int deformerCount = pCurrMesh->GetDeformerCount();
	//size_t userAniDataSize = m_UserAniData.size();
	//fbxsdk::FbxAMatrix geometryTransform = GetGeometryTransformation(pNode);

	//fbxsdk::FbxAnimStack* currAnimStack = m_Scene->GetSrcObject<fbxsdk::FbxAnimStack>(0);
	//fbxsdk::FbxString animStackName = currAnimStack->GetName();
	//fbxsdk::FbxTakeInfo* takeInfo = m_Scene->GetTakeInfo(animStackName);
	//fbxsdk::FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	//fbxsdk::FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
	//fbxsdk::FbxTime::EMode timeMode = m_Scene->GetGlobalSettings().GetTimeMode();

	//fbxsdk::FbxAMatrix currentTransformOffset;
	//fbxsdk::FbxAMatrix localTransform;
	//fbxsdk::FbxAMatrix globalTransform;

	//fbxsdk::FbxCluster* pCurrCluster;
	//std::string currJointName;
	//Bone* pBone;

	//fbxsdk::FbxAMatrix transformMatrix;
	//fbxsdk::FbxAMatrix transformLinkMatrix;
	//fbxsdk::FbxAMatrix globalBindposeInverseMatrix;

	//fbxsdk::FbxLongLong fixIndex;

	//GameEngineString linkName;

	//fbxsdk::FbxLongLong endTime;
	//fbxsdk::FbxLongLong startTime;

	//fbxsdk::FbxTime currTime;


	//for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
	//{
	//	fbxsdk::FbxSkin* pCurrSkin = reinterpret_cast<fbxsdk::FbxSkin*>(pCurrMesh->GetDeformer(deformerIndex, fbxsdk::FbxDeformer::eSkin));
	//	if (nullptr == pCurrSkin)
	//	{
	//		continue;
	//	}

	//	for (size_t userAniDataIndex = 0; userAniDataIndex < userAniDataSize; ++userAniDataIndex)
	//	{
	//		unsigned int numOfClusters = pCurrSkin->GetClusterCount();

	//		endTime = end.GetFrameCount(timeMode);
	//		startTime = start.GetFrameCount(timeMode);

	//		FbxExAniData& CurAniData = m_UserAniData[userAniDataIndex];
	//		CurAniData.EndTime = endTime;
	//		CurAniData.StartTime = startTime;
	//		CurAniData.TimeMode = timeMode;

	//		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
	//		{
	//			pCurrCluster = pCurrSkin->GetCluster(clusterIndex);
	//			currJointName = GameEngineString::ConvertMultiByteString(pCurrCluster->GetLink()->GetName());
	//			pBone = _Fbx->FindBone(currJointName);

	//			pCurrCluster->GetTransformMatrix(transformMatrix);
	//			pCurrCluster->GetTransformLinkMatrix(transformLinkMatrix);
	//			globalBindposeInverseMatrix = transformLinkMatrix.Inverse();

	//			linkName = GameEngineString::ConvertMultiByteString(pCurrCluster->GetLink()->GetName());
	//			fbxsdk::FbxNode* pLinkNode = m_Scene->FindNodeByName(linkName.GetConvertMultiByteString().c_str());

	//			FbxExBoneFrame& Frame = CurAniData.m_AniFrameData[pBone->Index];
	//			Frame.m_Data.resize(endTime - startTime + 1);
	//			Frame.BoneIndex = pBone->Index;
	//			Frame.BoneParentIndex = pBone->ParentIndex;

	//			for (fbxsdk::FbxLongLong i = startTime; i <= endTime; ++i)
	//			{
	//				fixIndex = i - startTime;

	//				FbxExBoneFrameData& FrameData = Frame.m_Data[fixIndex];

	//				currTime.SetFrame(fixIndex, timeMode);
	//				currentTransformOffset = pNode->EvaluateGlobalTransform(currTime) * m_JointPostConversionMatrix * geometryTransform;
	//				globalTransform = currentTransformOffset.Inverse() * pLinkNode->EvaluateGlobalTransform(currTime);

	//				localTransform.SetS(pLinkNode->EvaluateLocalScaling(currTime));
	//				localTransform.SetR(pLinkNode->EvaluateLocalRotation(currTime));
	//				localTransform.SetT(pLinkNode->EvaluateLocalTranslation(currTime));

	//				FrameData.Time = currTime.GetSecondDouble();
	//				FrameData.LocalAnimation = localTransform;
	//				FrameData.GlobalAnimation = globalTransform;
	//				FrameData.FrameMat = FbxMatTofloat4x4(FrameData.GlobalAnimation);
	//				FrameData.S = FbxVecTofloat4(FrameData.GlobalAnimation.GetS());
	//				FrameData.Q = FbxQuaternionTofloat4(FrameData.GlobalAnimation.GetQ());
	//				FrameData.T = FbxVecToTransform(FrameData.GlobalAnimation.GetT());

	//				if (_AniProgress != nullptr)
	//				{
	//					_AniProgress((float)++CurMatrixCount / (float)AllAniMatirixCount * 100.0f);
	//				}
	//			}
	//		}
	//	}
	//}

	return true;
}

void GameEngineFBXAnimation::CalFbxExBoneFrameTransMatrix(GameEngineFBXMesh* _Mesh)
{
	if (0 == AnimationDatas.size())
	{
		GameEngineDebug::MsgBoxError("애니메이션 데이터가 존재하지 않는 매쉬로 애니메이션을 만들려고 했습니다.");
		return;
	}

	for (size_t i = 0; i < AnimationDatas.size(); i++)
	{
		AnimationDatas[i].AniFrameData.resize(_Mesh->AllBones.size());
	}

	// 
}

