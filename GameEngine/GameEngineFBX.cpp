#include "PreCompile.h"
#include "GameEngineFBX.h"

GameEngineFBX::GameEngineFBX() :
	Manager(nullptr),
	IOSetting(nullptr),
	Importer(nullptr),
	Scene(nullptr),
	RootNode(nullptr),
	ConvertMatrix(),
	JointMatrix(),
	AxisVector()
{
}

GameEngineFBX::~GameEngineFBX()
{
	Reset();
}

void GameEngineFBX::RecursiveAllNode(std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack, std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack, int _ParentReturn)
{
	RecursiveAllNode(GetRootNode(), _InitCallBack, _EndCallBack, _ParentReturn);
}

void GameEngineFBX::Reset()
{
	if (nullptr != Scene)
	{
		Scene->Destroy();
		Scene = nullptr;
	}
	if (nullptr != Importer)
	{
		Importer->Destroy();
		Importer = nullptr;
	}
	if (nullptr != IOSetting)
	{
		IOSetting->Destroy();
		IOSetting = nullptr;
	}
	if (nullptr != Manager)
	{
		Manager->Destroy();
		Manager = nullptr;
	}
}

bool GameEngineFBX::CreateFBXSystemInitialize(const std::string& _Path)
{
	Manager = fbxsdk::FbxManager::Create();

	if (nullptr == Manager)
	{
		GameEngineDebug::MsgBoxError("FBX 매니저 생성에 실패했습니다.");
		return false;
	}

	// fbx파일을 읽는 방법을 정의한다.
	IOSetting = fbxsdk::FbxIOSettings::Create(Manager, IOSROOT);

	// 
	Importer = fbxsdk::FbxImporter::Create(Manager, "");
	if (false == Importer->Initialize(GameEngineString::AnsiToUTF8Return(_Path).c_str(), -1, IOSetting))
	{
		Importer->Destroy();
		IOSetting->Destroy();
		GameEngineDebug::MsgBoxError("FBX 로드 이니셜라이즈 실패.");
		return false;
	}

	Scene = fbxsdk::FbxScene::Create(Manager, "");

	if (nullptr == Scene)
	{
		GameEngineDebug::MsgBoxError("FBX 씬생성 실패.");
		return false;
	}

	if (false == Importer->Import(Scene))
	{
		GameEngineDebug::MsgBoxError("FBX 임포트 실패.");
		return false;
	}

	return true;
}

void GameEngineFBX::FBXConvertScene()
{
	AxisVector = { 0.0f, 0.0f, 0.0f, 1.0f };
	fbxsdk::FbxAxisSystem::EUpVector UpVector = fbxsdk::FbxAxisSystem::eYAxis;
	fbxsdk::FbxAxisSystem::EFrontVector FrontVector = (fbxsdk::FbxAxisSystem::EFrontVector)-fbxsdk::FbxAxisSystem::eParityOdd;
	fbxsdk::FbxAxisSystem::ECoordSystem CooreSystem = fbxsdk::FbxAxisSystem::ECoordSystem::eRightHanded;

	fbxsdk::FbxAxisSystem EngineAxisSystem(UpVector, FrontVector, CooreSystem);
	fbxsdk::FbxAxisSystem SourceAxisSystem = Scene->GetGlobalSettings().GetAxisSystem();

	// 
	if (SourceAxisSystem != EngineAxisSystem)
	{
		fbxsdk::FbxRootNodeUtility::RemoveAllFbxRoots(Scene);
		EngineAxisSystem.ConvertScene(Scene);
		fbxsdk::FbxAMatrix SourceMatrix;
		SourceAxisSystem.GetMatrix(SourceMatrix);
		fbxsdk::FbxAMatrix EngineMatrix;
		EngineAxisSystem.GetMatrix(EngineMatrix);
		ConvertMatrix = SourceMatrix.Inverse() * EngineMatrix;
	}

	int OriginUpSign = 0;
	int OriginFrontSign = 0;
	int EngineUpSign = 0;
	int EngineFrontSign = 0;

	fbxsdk::FbxAxisSystem::EFrontVector OriginFrontVector = SourceAxisSystem.GetFrontVector(OriginFrontSign);
	fbxsdk::FbxAxisSystem::EFrontVector EngineFrontVector = EngineAxisSystem.GetFrontVector(EngineFrontSign);

	fbxsdk::FbxAxisSystem::EUpVector OriginUpVector = SourceAxisSystem.GetUpVector(OriginUpSign);
	fbxsdk::FbxAxisSystem::EUpVector EngineUpVector = EngineAxisSystem.GetUpVector(EngineUpSign);

	fbxsdk::FbxAxisSystem::ECoordSystem  OriginCoordSystem = SourceAxisSystem.GetCoorSystem();
	fbxsdk::FbxAxisSystem::ECoordSystem  EngineCoordSystem = EngineAxisSystem.GetCoorSystem();

	if (OriginUpVector == EngineUpVector && OriginUpSign == EngineUpSign)
	{
		if (OriginFrontSign != EngineFrontSign && OriginCoordSystem == EngineCoordSystem)
		{
			AxisVector.mData[static_cast<int>(OriginUpVector) - 1] += 180;
		}
	}
	else if (OriginUpVector == EngineUpVector && OriginUpSign != EngineUpSign)
	{
		if (OriginUpVector == fbxsdk::FbxAxisSystem::EUpVector::eXAxis)
		{
			AxisVector.mData[1] += 180;
		}
		else
		{
			AxisVector.mData[0] += 180;
		}

		if (OriginFrontSign != EngineFrontSign && OriginCoordSystem == EngineCoordSystem)
		{
			AxisVector.mData[static_cast<int>(OriginUpVector) - 1] += 180;
		}
	}
	else if (OriginUpVector == fbxsdk::FbxAxisSystem::EUpVector::eXAxis)
	{
		AxisVector.mData[1] += OriginUpSign * 90;
	}
	else
	{
		AxisVector.mData[0] += OriginUpSign * 90;
		if (OriginUpSign != EngineFrontSign)
		{
			AxisVector.mData[static_cast<int>(OriginUpVector) - 1] += OriginUpSign * 180;
		}
	}

	JointMatrix.SetR(AxisVector);

	if (true == JointMatrix.IsIdentity())
	{
		JointMatrix = ConvertMatrix;
	}

	// 애니메이션도 행렬의 집합
	Scene->GetAnimationEvaluator()->Reset();

	RootNode = Scene->GetRootNode();

	if (nullptr == RootNode)
	{
		GameEngineDebug::MsgBoxError("루트노드생성에 실패했습니다.");
	}

	fbxsdk::FbxGeometryConverter Con(Manager);

	if (false == Con.Triangulate(Scene, true))
	{
		GameEngineDebug::MsgBoxError("삼각화에 실패했습니다.");
	}

	return;
}

void GameEngineFBX::RecursiveAllNode(fbxsdk::FbxNode* _Node, std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack, std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack, int _ParentReturn)
{
	fbxsdk::FbxNodeAttribute* Info = _Node->GetNodeAttribute();
	if (nullptr != Info)
	{
		fbxsdk::FbxNodeAttribute::EType Type = Info->GetAttributeType();

		switch (Type)
		{
			case fbxsdk::FbxNodeAttribute::eUnknown:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eNull:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eMarker:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eSkeleton:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eMesh:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eNurbs:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::ePatch:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eCamera:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eCameraStereo:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eLight:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eOpticalReference:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eOpticalMarker:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eNurbsCurve:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eBoundary:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eNurbsSurface:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eShape:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eLODGroup:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eSubDiv:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eCachedEffect:
			{
				break;
			}
			case fbxsdk::FbxNodeAttribute::eLine:
			{
				break;
			}
			default:
			{
				break;
			}
		}
	}

	int StartReturn = 0;

	if (nullptr != _InitCallBack)
	{
		if (nullptr != Info)
		{
			fbxsdk::FbxNodeAttribute::EType Type = Info->GetAttributeType();
			StartReturn = _InitCallBack(Type, _Node, _ParentReturn);
		}
		else 
		{
			StartReturn = _InitCallBack(fbxsdk::FbxNodeAttribute::EType::eUnknown, _Node, _ParentReturn);
		}
	}


	int ChildCount = _Node->GetChildCount();

	for (int i = 0; i < ChildCount; i++)
	{
		fbxsdk::FbxNode* ChildNode = _Node->GetChild(i);

		FbxStringList List = ChildNode->GetTypeFlags();

		RecursiveAllNode(ChildNode, _InitCallBack, _EndCallBack, StartReturn);
	}

	if (nullptr != _EndCallBack)
	{
		if (nullptr != Info)
		{
			fbxsdk::FbxNodeAttribute::EType Type = Info->GetAttributeType();
			_EndCallBack(Type, _Node, StartReturn);
		}
		else
		{
			_EndCallBack(fbxsdk::FbxNodeAttribute::EType::eUnknown, _Node, StartReturn);
		}
	}
}

float4x4 GameEngineFBX::FbxMatTofloat4x4(const fbxsdk::FbxAMatrix& _BaseTrans)
{
	float4x4 Mat;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			Mat.Arr2D[y][x] = (float)_BaseTrans.Get(y, x);
		}
	}

	return Mat;
}

fbxsdk::FbxAMatrix GameEngineFBX::float4x4ToFbxAMatrix(const float4x4& _MATRIX)
{
	fbxsdk::FbxAMatrix mat;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			mat.mData[y].mData[x] = _MATRIX.Arr2D[y][x];
		}
	}

	return mat;
}

float4 GameEngineFBX::FbxVecTofloat4(const fbxsdk::FbxVector4& _BaseVector)
{
	float4 Vec;
	Vec.Arr1D[0] = (float)_BaseVector.mData[0];
	Vec.Arr1D[1] = (float)_BaseVector.mData[1];
	Vec.Arr1D[2] = (float)_BaseVector.mData[2];
	Vec.Arr1D[3] = (float)_BaseVector.mData[3];

	return Vec;
}

float4 GameEngineFBX::FbxVecToTransform(const fbxsdk::FbxVector4& _BaseVector)
{
	float4 Vec;
	Vec.Arr1D[0] = (float)_BaseVector.mData[0];
	Vec.Arr1D[1] = (float)_BaseVector.mData[1];
	Vec.Arr1D[2] = -(float)_BaseVector.mData[2];
	Vec.Arr1D[3] = (float)_BaseVector.mData[3];
	return Vec;
}

float4 GameEngineFBX::FbxQuaternionTofloat4(const fbxsdk::FbxQuaternion& _BaseQ)
{
	float4 Vec;
	Vec.Arr1D[0] = (float)_BaseQ.mData[0];
	Vec.Arr1D[1] = (float)_BaseQ.mData[1];
	Vec.Arr1D[2] = -(float)_BaseQ.mData[2];
	Vec.Arr1D[3] = -(float)_BaseQ.mData[3];
	return Vec;
}
