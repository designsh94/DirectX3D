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
	// FBX Manager 생성
	Manager = fbxsdk::FbxManager::Create();
	if (nullptr == Manager)
	{
		GameEngineDebug::MsgBoxError("FBX 매니저 생성에 실패했습니다.");
		return false;
	}

	// FBX Class로 장면
	Scene = fbxsdk::FbxScene::Create(Manager, "");
	if (nullptr == Scene)
	{
		GameEngineDebug::MsgBoxError("FBX 씬생성 실패.");
		return false;
	}

	// FBX File을 읽는 방법을 정의를 위한 IOSetting 생성
	IOSetting = fbxsdk::FbxIOSettings::Create(Manager, IOSROOT);

	// FBX File을 SDK 개체로 가져오기위한 클래스 생성
	Importer = fbxsdk::FbxImporter::Create(Manager, "");
	if (false == Importer->Initialize(GameEngineString::AnsiToUTF8Return(_Path).c_str(), -1, IOSetting))
	{
		// FBX File Load를 위한 객체(Importer) 초기화 실패로 로드에 필요한 객체 모두 소멸
		// 실패요인
		// 1) 경로이상인 경우 초기화 실패사유가 될 수 있다.
		Importer->Destroy();
		IOSetting->Destroy();
		Scene->Destroy();

		// 
		GameEngineDebug::MsgBoxError("FBX 로드 이니셜라이즈 실패.");
		return false;
	}

	// 해당 FBX File의 내용을 Scene 컨테이너에 채우기
	if (false == Importer->Import(Scene))
	{
		GameEngineDebug::MsgBoxError("FBX 임포트 실패.");
		return false;
	}

	// FBX File Load 준비완료!!!!!
	// -> 파일 내용을 씬으로 채운 후에는 메모리 사용을 줄이기 위해 importer를 소멸시키는 것이 안전
	//Importer->Destroy();
	//Importer = nullptr;

	return true;
}

void GameEngineFBX::FBXConvertScene()
{
	// 현재 Engine의 축정보 생성
	AxisVector = { 0.0f, 0.0f, 0.0f, 1.0f };
	fbxsdk::FbxAxisSystem::EUpVector UpVector = fbxsdk::FbxAxisSystem::eYAxis;
	fbxsdk::FbxAxisSystem::EFrontVector FrontVector = static_cast<fbxsdk::FbxAxisSystem::EFrontVector>(-fbxsdk::FbxAxisSystem::eParityOdd);
	fbxsdk::FbxAxisSystem::ECoordSystem CooreSystem = fbxsdk::FbxAxisSystem::ECoordSystem::eRightHanded;
	fbxsdk::FbxAxisSystem EngineAxisSystem(UpVector, FrontVector, CooreSystem);

	// 씬의 축 시스템, 주변 조명 및 시간 설정은 해당 FbxGlobalSettings 객체에 정의
	// 그러므로 FbxGlobalSettings을 통해 좌표축을 Get
	fbxsdk::FbxAxisSystem SourceAxisSystem = Scene->GetGlobalSettings().GetAxisSystem();

	// 로드한 파일의 축정보와 현재 엔진의 축정보가 일치하지않다면
	if (SourceAxisSystem != EngineAxisSystem)
	{
		// 모든 Fbx_Root 노드의 장면을 제거하고,
		fbxsdk::FbxRootNodeUtility::RemoveAllFbxRoots(Scene);

		// 현재 씬의 축정보를 엔진의 축시스템으로 변경하며
		EngineAxisSystem.ConvertScene(Scene);

		// 파일의 축행렬 정보 Get
		fbxsdk::FbxAMatrix SourceMatrix;
		SourceAxisSystem.GetMatrix(SourceMatrix);

		// 엔진의 축행렬 정보 Get
		fbxsdk::FbxAMatrix EngineMatrix;
		EngineAxisSystem.GetMatrix(EngineMatrix);

		// 축변환행렬 저장
		ConvertMatrix = SourceMatrix.Inverse() * EngineMatrix;
	}

	// 각 축정보 Setting(X,Y,Z)
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

	// 쿼터니언 정보 저장
	JointMatrix.SetR(AxisVector);
	if (true == JointMatrix.IsIdentity())
	{
		JointMatrix = ConvertMatrix;
	}

	// 애니메이션도 행렬의 집합
	Scene->GetAnimationEvaluator()->Reset();

	// FBX File의 루트노드를 Get
	RootNode = Scene->GetRootNode();
	if (nullptr == RootNode)
	{
		GameEngineDebug::MsgBoxError("루트노드생성에 실패했습니다.");
	}

	// 씬(Scene) 내에서 삼각형화(Triangulate) 할 수 있는 모든 노드를 삼각형화
	// -> FBX는 각 정점을 제어점(Control Point)으로 관리하는
	// 반면, DirectX는 정점을 이용하여 면을 구성할때 삼각형(Triangle)을 기준으로 면을 구성한다.
	// 그러므로 FBX File을 로드했을때 각 제어정점을 DirectX에서 면을 구성하는 방식으로 변환한다.
	// 즉, 4개의 제어점이 존재할때 DirctX에서의 면구성방식을 위해 삼각화하며 4개이던 제어점은 6개의 점으로 특정정점을 중복바인딩한다.
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
