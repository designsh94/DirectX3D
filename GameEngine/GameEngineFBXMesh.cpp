#include "PreCompile.h"
#include "GameEngineFBXMesh.h"
#include "GameEngineStructuredBuffer.h"

GameEngineFBXMesh::GameEngineFBXMesh() :
	IsAnimationLoadOnce(false),
	AnimationBuffer(nullptr)
{
}

GameEngineFBXMesh::~GameEngineFBXMesh() 
{
	if (nullptr != AnimationBuffer)
	{
		delete AnimationBuffer;
	}
}

void GameEngineFBXMesh::Load(const std::string& _Path) 
{
	// FBX File 로드 및 정보수집을 위한 초기화
	if (false == CreateFBXSystemInitialize(_Path))
	{
		GameEngineDebug::MsgBoxError("FBX 이니셜라이즈에 실패했습니다.");
		return;
	}

	// 축시스템 변환
	FBXConvertScene();

	// 정점데이터 수집
	MeshLoad();

	// 본정보 수집
	ImportBone();
}

fbxsdk::FbxNode* GameEngineFBXMesh::RecursiveFindParentLodGroup(fbxsdk::FbxNode* parentNode)
{
	if (nullptr == parentNode)
	{
		return nullptr;
	}

	fbxsdk::FbxNodeAttribute* attribute = parentNode->GetNodeAttribute();

	if (nullptr != attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eLODGroup)
	{
		return parentNode;
	}

	return RecursiveFindParentLodGroup(parentNode->GetParent());
}

void GameEngineFBXMesh::MeshNodeCheck()
{
	// 해당 Scene의 모든 정점의 수 Get
	int geometryCount = Scene->GetGeometryCount();
	for (int i = 0; i < geometryCount; i++)
	{
		// 노드중에서 기하구조를 가진것을 추출
		fbxsdk::FbxGeometry* geoMetry = Scene->GetGeometry(i);
		fbxsdk::FbxNode* geoMetryNode = geoMetry->GetNode();

		// 추출한 것의 타입이 메쉬라면
		if (geoMetry->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
		{
			MeshInfos.push_back(FbxExMeshInfo());
			FbxExMeshInfo& Info = MeshInfos[MeshInfos.size() - 1];

			if (geoMetry->GetName()[0] != '\0')
			{
				Info.Name = GameEngineString::AnsiToUTF8Return(geoMetry->GetName());
			}
			else 
			{
				Info.Name = nullptr != geoMetryNode ? GameEngineString::AnsiToUTF8Return(geoMetryNode->GetName()) : "None";
			}

			Info.Mesh = reinterpret_cast<fbxsdk::FbxMesh*>(geoMetry);
			Info.bTriangulated = Info.Mesh->IsTriangleMesh();
			Info.MaterialNum = geoMetryNode ? geoMetryNode->GetMaterialCount() : 0;
			Info.FaceNum = Info.Mesh->GetPolygonCount();
			Info.VertexNum = Info.Mesh->GetControlPointsCount();
			Info.UniqueId = Info.Mesh->GetUniqueID();

			// LOD Group Check
			Info.LODGroup = "";
			if (nullptr != geoMetryNode)
			{
				fbxsdk::FbxNode* ParentNode = RecursiveFindParentLodGroup(geoMetryNode->GetParent());
				if (ParentNode != nullptr && ParentNode->GetNodeAttribute() && ParentNode->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eLODGroup)
				{
					Info.LODGroup = GameEngineString::AnsiToUTF8Return(ParentNode->GetName());
					Info.bIsLodGroup = true;
				}
			}

			if (Info.Mesh->GetDeformerCount(FbxDeformer::eSkin) > 0)
			{
				Info.bIsSkelMesh = true;
				Info.MorphNum = Info.Mesh->GetShapeCount();
				fbxsdk::FbxSkin* Skin = (fbxsdk::FbxSkin*)Info.Mesh->GetDeformer(0, FbxDeformer::eSkin);
				int ClusterCount = Skin->GetClusterCount();
				fbxsdk::FbxNode* Link = NULL;
				for (int ClusterId = 0; ClusterId < ClusterCount; ++ClusterId)
				{
					// 클러스터 : 링크를 관리
					fbxsdk::FbxCluster* Cluster = Skin->GetCluster(ClusterId);
					Link = Cluster->GetLink();
					while (Link && Link->GetParent() && Link->GetParent()->GetSkeleton())
					{
						Link = Link->GetParent();
					}

					if (Link != NULL)
					{
						break;
					}
				}

				Info.SkeletonRoot = nullptr != Link ? GameEngineString::UTF8ToAnsiReturn(Link->GetName()) : "None";
				Info.SkeletonElemNum = nullptr != Link ? Link->GetChildCount(true) : 0;
			}
			else
			{
				Info.bIsSkelMesh = false;
				Info.SkeletonRoot = "";
			}
		}
	}

	// LOD는 정점의 갯수로 저장
	std::multimap<int, FbxExMeshInfo*> LodCheckMap;
	for (int i = 0; i < MeshInfos.size(); i++)
	{
		if (false == MeshInfos[i].bIsLodGroup)
		{
			continue;
		}

		LodCheckMap.insert(std::make_pair(MeshInfos[i].VertexNum, &MeshInfos[i]));
	}

	int Count = 0;
	for (auto& Data : LodCheckMap)
	{
		Data.second->VertexOrder = Count;
		++Count;
	}
}

void GameEngineFBXMesh::MeshLoad()
{
	// Mesh를 얻어올 수 있는 노드를 체크
	MeshNodeCheck();

	// 얻어온 메쉬들을 이용하여 실질적인 버텍스버퍼를 생성
	VertexBufferCheck();
}

fbxsdk::FbxAMatrix GameEngineFBXMesh::ComputeTotalMatrix(fbxsdk::FbxNode* Node)
{
	fbxsdk::FbxAMatrix Geometry;
	fbxsdk::FbxVector4 Translation, Rotation, Scaling;
	Translation = Node->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	Rotation = Node->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	Scaling = Node->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);
	Geometry.SetT(Translation);
	Geometry.SetR(Rotation);
	Geometry.SetS(Scaling);

	fbxsdk::FbxAMatrix& GlobalTransform = Scene->GetAnimationEvaluator()->GetNodeGlobalTransform(Node);
	return GlobalTransform * Geometry;
}

bool GameEngineFBXMesh::IsOddNegativeScale(const fbxsdk::FbxAMatrix& TotalMatrix)
{
	fbxsdk::FbxVector4 Scale = TotalMatrix.GetS();
	int NegativeNum = 0;

	if (Scale[0] < 0)
	{
		NegativeNum++;
	}
	if (Scale[1] < 0)
	{
		NegativeNum++;
	}
	if (Scale[2] < 0)
	{
		NegativeNum++;
	}

	return NegativeNum == 1 || NegativeNum == 3;
}

void GameEngineFBXMesh::LoadBinormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index)
{
	int iCount = _Mesh->GetElementBinormalCount();
	if (0 == iCount)
	{
		return;
	}

	FbxGeometryElementBinormal* pElement = _Mesh->GetElementBinormal();
	int iDataIndex = VtxId;

	if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(VtxId);
		}
		else
		{
			iDataIndex = VtxId;
		}
	}

	else if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _Index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_Index);
		}
		else {
			GameEngineDebug::MsgBoxError("맵핑 방식중 처리할수 없는 방식입니다.");
		}
	}

	FbxVector4 BiNormal = pElement->GetDirectArray().GetAt(iDataIndex);

	fbxsdk::FbxAMatrix conversionMeshMatrix = _MeshMatrix.Transpose();
	BiNormal = conversionMeshMatrix.MultT(BiNormal);

	_ArrVtx[_Index].BINORMAL.x = (float)BiNormal.mData[0];
	_ArrVtx[_Index].BINORMAL.y = (float)BiNormal.mData[1];
	_ArrVtx[_Index].BINORMAL.z = -(float)BiNormal.mData[2];
	_ArrVtx[_Index].BINORMAL.w = (float)BiNormal.mData[3];
	_ArrVtx[_Index].BINORMAL.Normalize3D();
}

void GameEngineFBXMesh::LoadTangent(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index)
{
	int iCount = _Mesh->GetElementTangentCount();

	if (0 == iCount)
	{
		return;
	}

	FbxGeometryElementTangent* pElement = _Mesh->GetElementTangent();
	int iDataIndex = VtxId;

	if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(VtxId);
		}
		else
		{
			iDataIndex = VtxId;
		}
	}
	else if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _Index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_Index);
		}
		else {
			GameEngineDebug::MsgBoxError("맵핑 방식중 처리할수 없는 방식입니다.");
		}
	}

	FbxVector4 Tangent = pElement->GetDirectArray().GetAt(iDataIndex);

	fbxsdk::FbxAMatrix conversionMeshMatrix = _MeshMatrix.Transpose();
	Tangent = conversionMeshMatrix.MultT(Tangent);

	_ArrVtx[_Index].TANGENT.x = (float)Tangent.mData[0];
	_ArrVtx[_Index].TANGENT.y = (float)Tangent.mData[1];
	_ArrVtx[_Index].TANGENT.z = -(float)Tangent.mData[2];
	_ArrVtx[_Index].TANGENT.w = (float)Tangent.mData[3];
	_ArrVtx[_Index].TANGENT.Normalize3D();
}

void GameEngineFBXMesh::LoadNormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index)
{
	int iCount = _Mesh->GetElementNormalCount();
	if (0 == iCount)
	{
		GameEngineDebug::MsgBoxError("GetElementNormalCount가 여러개 입니다.");
	}

	FbxGeometryElementNormal* pElement = _Mesh->GetElementNormal();
	int iDataIndex = VtxId;
	if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(VtxId);
		}
		else
		{
			iDataIndex = VtxId;
		}
	}
	else if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _Index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_Index);
		}
		else {
			GameEngineDebug::MsgBoxError("맵핑 방식중 처리할수 없는 방식입니다.");
		}
	}

	FbxVector4 Normal = pElement->GetDirectArray().GetAt(iDataIndex);

	fbxsdk::FbxAMatrix conversionMeshMatrix = _MeshMatrix.Transpose();
	Normal = conversionMeshMatrix.MultT(Normal);

	_ArrVtx[_Index].NORMAL.x = (float)Normal.mData[0];
	_ArrVtx[_Index].NORMAL.y = (float)Normal.mData[1];
	_ArrVtx[_Index].NORMAL.z = -(float)Normal.mData[2];
	_ArrVtx[_Index].NORMAL.w = (float)Normal.mData[3];
	_ArrVtx[_Index].NORMAL.Normalize3D();
}

void GameEngineFBXMesh::VertexBufferCheck()
{
	// 얻어온 정점 정보를 이용하여 버텍스버퍼 생성
	size_t meshInfoSize = MeshInfos.size();
	for (size_t meshInfoIndex = 0; meshInfoIndex < meshInfoSize; ++meshInfoIndex)
	{
		FbxExMeshInfo& meshInfo = MeshInfos.at(meshInfoIndex);
		fbxsdk::FbxNode* pMeshNode = meshInfo.Mesh->GetNode();
		fbxsdk::FbxMesh* pMesh = meshInfo.Mesh;

		if (AllMeshMap.end() == AllMeshMap.find(meshInfo.VertexOrder))
		{
			AllMeshMap.insert(std::make_pair(meshInfo.VertexOrder, FbxMeshSet()));
		}

		FbxMeshSet& DrawMesh = AllMeshMap[meshInfo.VertexOrder];

		if (DrawMesh.MapWI.end() == DrawMesh.MapWI.find(pMesh))
		{
			DrawMesh.MapWI.insert(std::make_pair(pMesh, std::map<int, std::vector<FbxExIW>>()));
		}

		DrawMesh.IsLod = meshInfo.bIsLodGroup;
		DrawMesh.IsLodLv = meshInfo.VertexOrder;
		DrawMesh.Vertexs.push_back(std::vector<GameEngineVertex>());
		DrawMesh.Indexs.push_back(std::vector<std::vector<UINT>>());
		std::vector<GameEngineVertex>& VtxData = DrawMesh.Vertexs[DrawMesh.Vertexs.size() - 1];
		std::vector<std::vector<UINT>>& IdxData = DrawMesh.Indexs[DrawMesh.Indexs.size() - 1];

		int controlPointsCount = pMesh->GetControlPointsCount();

		// FBX 파일내에 담겨있는 포지션 데이터
		fbxsdk::FbxVector4* pControlPoints = pMesh->GetControlPoints();
		VtxData.resize(controlPointsCount);

		fbxsdk::FbxAMatrix meshMatrix = ConvertMatrix;
		meshMatrix = ComputeTotalMatrix(pMeshNode);

		if (false == meshInfo.bIsSkelMesh)
		{
			meshMatrix = JointMatrix * meshMatrix;
		}

		// 크기가 -인게 있는지 확인
		bool isOddNegativeScale = IsOddNegativeScale(meshMatrix);

		// 포지션을 얻어온다.
		for (int controlPointIndex = 0; controlPointIndex < controlPointsCount; ++controlPointIndex)
		{
			fbxsdk::FbxVector4 controlPoint = pControlPoints[controlPointIndex];
			fbxsdk::FbxVector4 calculateControlPoint = meshMatrix.MultT(controlPoint);
			VtxData[controlPointIndex].POSITION = FbxVecToTransform(calculateControlPoint);
			VtxData[controlPointIndex].POSITION.w = 1.0f;
		}

		// 
		FbxMeshSetMaterialSetting(pMeshNode, &DrawMesh);

		fbxsdk::FbxStringList UVSetNameList;
		pMesh->GetUVSetNames(UVSetNameList);
		int uvSetCount = UVSetNameList.GetCount();
		for (int uvSetIndex = 0; uvSetIndex < uvSetCount; ++uvSetIndex)
		{
			const char* uvSetName = UVSetNameList.GetStringAt(uvSetIndex);
			const fbxsdk::FbxGeometryElementUV* pUVElement = pMesh->GetElementUV(uvSetName);
			if (nullptr == pUVElement)
			{
				continue;
			}

			fbxsdk::FbxGeometryElement::EMappingMode eMappingMode = pUVElement->GetMappingMode();
			fbxsdk::FbxGeometryElement::EReferenceMode eReferenceMode = pUVElement->GetReferenceMode();
			bool useIndex = fbxsdk::FbxGeometryElement::EReferenceMode::eDirect != eReferenceMode;
			int IndexCount = true == useIndex ? pUVElement->GetIndexArray().GetCount() : 0;

			int nPolygonCount = pMesh->GetPolygonCount();
			for (int PolygonIndex = 0; PolygonIndex < nPolygonCount; ++PolygonIndex)
			{
				const int PolygonSize = pMesh->GetPolygonSize(PolygonIndex);
				if (3 != PolygonSize)
				{
					GameEngineDebug::MsgBoxError("삼각형이 아닌 면이 발견됬습니다.");
				}

				for (int PositionInPolygon = 0; PositionInPolygon < PolygonSize; ++PositionInPolygon)
				{
					int convertUvIndex = isOddNegativeScale ? 2 - PositionInPolygon : PositionInPolygon;
					int ControlPointIndex = pMesh->GetPolygonVertex(PolygonIndex, PositionInPolygon);
					int UVMapIndex = (fbxsdk::FbxGeometryElement::EMappingMode::eByControlPoint == eMappingMode) ?
						ControlPointIndex : PolygonIndex * 3 + PositionInPolygon;

					int UvIndex = useIndex ? pUVElement->GetIndexArray().GetAt(UVMapIndex) : UVMapIndex;
					fbxsdk::FbxVector2 uvValue = pUVElement->GetDirectArray().GetAt(UvIndex);

					VtxData[ControlPointIndex].TEXTURECOORD.x = static_cast<float>(uvValue.mData[0]);
					VtxData[ControlPointIndex].TEXTURECOORD.y = 1.f - static_cast<float>(uvValue.mData[1]);
				}
			}
		}

		pMesh->GetElementMaterialCount();
		fbxsdk::FbxGeometryElementMaterial* pGeometryElementMaterial = pMesh->GetElementMaterial();
		fbxsdk::FbxGeometryElementNormal* pGeometryElementNormal = pMesh->GetElementNormal();

		int materialCount = pMeshNode->GetMaterialCount();
		IdxData.resize(materialCount);

		UINT VtxId = 0;

		int nPolygonCount = pMesh->GetPolygonCount();
		for (int PolygonIndex = 0; PolygonIndex < nPolygonCount; ++PolygonIndex)
		{
			int PolygonSize = pMesh->GetPolygonSize(PolygonIndex);
			if (3 != PolygonSize)
			{
				GameEngineDebug::MsgBoxError("삼각형이 아닌 면이 발견됬습니다.");
			}

			int IndexArray[3] = { 0, };
			for (int PositionInPolygon = 0; PositionInPolygon < PolygonSize; ++PositionInPolygon)
			{
				int ControlPointIndex = pMesh->GetPolygonVertex(PolygonIndex, PositionInPolygon);
				IndexArray[PositionInPolygon] = ControlPointIndex;

				LoadNormal(pMesh, meshMatrix, VtxData, VtxId, ControlPointIndex);
				LoadTangent(pMesh, meshMatrix, VtxData, VtxId, ControlPointIndex);
				LoadBinormal(pMesh, meshMatrix, VtxData, VtxId, ControlPointIndex);

				++VtxId;
			}

			int materialId = pGeometryElementMaterial->GetIndexArray().GetAt(PolygonIndex);
			IdxData[materialId].push_back(IndexArray[0]);
			IdxData[materialId].push_back(IndexArray[2]);
			IdxData[materialId].push_back(IndexArray[1]);
		}

		DrawMesh.FbxVertexMap.insert(std::make_pair(pMesh, &VtxData));
	}
}

void GameEngineFBXMesh::CreateRenderingBuffer()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
}

void GameEngineFBXMesh::CreateVertexBuffer()
{
	for (auto& Data : AllMeshMap)
	{
		for (size_t i = 0; i < Data.second.Vertexs.size(); i++)
		{
			GameEngineVertexBuffer* NewRes = new GameEngineVertexBuffer();
			NewRes->Create(Data.second.Vertexs[i], D3D11_USAGE::D3D11_USAGE_DEFAULT);
			Data.second.GameEngineVertexBuffers.push_back(NewRes);
		}
	}
}

void GameEngineFBXMesh::CreateIndexBuffer()
{
	for (auto& Data : AllMeshMap)
	{
		for (size_t i = 0; i < Data.second.Indexs.size(); i++)
		{
			Data.second.GameEngineIndexBuffers.push_back(std::vector<GameEngineIndexBuffer*>());
			for (size_t j = 0; j < Data.second.Indexs[i].size(); j++)
			{
				GameEngineIndexBuffer* NewRes = new GameEngineIndexBuffer();
				NewRes->Create(Data.second.Indexs[i][j], D3D11_USAGE::D3D11_USAGE_DEFAULT);
				Data.second.GameEngineIndexBuffers[i].push_back(NewRes);
			}
		}
	}
}

void GameEngineFBXMesh::ImportCluster()
{
	size_t meshCount = MeshInfos.size();
	if (0 == meshCount)
	{
		return;
	}

	for (size_t n = 0; n < meshCount; ++n)
	{
		FbxExMeshInfo& meshInfo = MeshInfos.at(n);
		fbxsdk::FbxNode* pNode = meshInfo.Mesh->GetNode();
		fbxsdk::FbxMesh* FbxMesh = meshInfo.Mesh;
		const int SkinDeformerCount = FbxMesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
		if (0 == SkinDeformerCount)
		{
			continue;
		}
		std::vector<FbxClusterData>& vecClusterData = ClusterData[meshInfo.VertexOrder];

		if (ClusterData.end() == ClusterData.find(meshInfo.VertexOrder))
		{
			ClusterData.insert(std::make_pair(meshInfo.VertexOrder, std::vector<FbxClusterData>()));
		}

		for (int DeformerIndex = 0; DeformerIndex < SkinDeformerCount; DeformerIndex++)
		{
			fbxsdk::FbxSkin* Skin = (fbxsdk::FbxSkin*)FbxMesh->GetDeformer(DeformerIndex, fbxsdk::FbxDeformer::eSkin);
			for (int ClusterIndex = 0; ClusterIndex < Skin->GetClusterCount(); ClusterIndex++)
			{
				vecClusterData.push_back(FbxClusterData());
				FbxClusterData& clusterData = vecClusterData[vecClusterData.size() - 1];
				clusterData.Cluster = Skin->GetCluster(ClusterIndex);
				clusterData.Mesh = FbxMesh;
				clusterData.LinkName = clusterData.Cluster->GetLink()->GetName();
			}
		}
	}
}

void GameEngineFBXMesh::LoadSkinAndCluster()
{
	ImportCluster();

	std::map<int, FbxMeshSet>::iterator Start = AllMeshMap.begin();
	std::map<int, FbxMeshSet>::iterator End = AllMeshMap.end();
	for (; Start != End; ++Start)
	{
		FbxMeshSet& MeshInfo = (*Start).second;
		std::vector<FbxClusterData>& ClusterInfo = ClusterData[(*Start).first];

		// 클러스터는 가중치 정보와 인덱스 정보를 가지고 있는데
		// 클러스터를 통해서 정보를 가져오고
		LoadAnimationVertexData(&MeshInfo, ClusterInfo);

		// 진짜 가중치를 계산한다.
		CalAnimationVertexData((*Start).second);
	}
}

void GameEngineFBXMesh::LoadAnimationVertexData(FbxMeshSet* _DrawData, const std::vector<FbxClusterData>& vecClusterData)
{
	for (auto& clusterData : vecClusterData)
	{
		if (nullptr == clusterData.Cluster->GetLink())
		{
			continue;
		}

		std::string StrBoneName = clusterData.LinkName;
		Bone* pBone = FindBone(StrBoneName);
		if (nullptr == pBone)
		{
			continue;
		}

		DrawSetWeightAndIndexSetting(_DrawData, clusterData.Mesh, clusterData.Cluster, pBone->Index);
	}
}

void GameEngineFBXMesh::DrawSetWeightAndIndexSetting(FbxMeshSet* _DrawSet, fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxCluster* _Cluster, int _BoneIndex)
{
	int iIndexCount = _Cluster->GetControlPointIndicesCount();

	for (size_t i = 0; i < iIndexCount; i++)
	{
		FbxExIW IW;
		IW.Index = _BoneIndex;

		IW.Weight = _Cluster->GetControlPointWeights()[i];
		int ControlPointIndices = _Cluster->GetControlPointIndices()[i];

		_DrawSet->MapWI[_Mesh][ControlPointIndices].push_back(IW);
	}
}

void GameEngineFBXMesh::FbxMeshSetMaterialSetting(fbxsdk::FbxNode* _Node, FbxMeshSet* _RenderData)
{
	int MtrlCount = _Node->GetMaterialCount();
	if (MtrlCount > 0)
	{
		_RenderData->MatialData.push_back(std::vector<FbxExRenderingPipeLineSettingData>());

		std::vector<FbxExRenderingPipeLineSettingData>& MatrialSet = _RenderData->MatialData[_RenderData->MatialData.size() - 1];

		for (int i = 0; i < MtrlCount; i++)
		{
			fbxsdk::FbxSurfaceMaterial* pMtrl = _Node->GetMaterial(i);

			if (nullptr == pMtrl)
			{
				GameEngineDebug::MsgBoxError("if (nullptr == pMtrl) 메테리얼 정보가 존재하지 않습니다");
				continue;
			}

			MatrialSet.push_back(FbxExRenderingPipeLineSettingData());
			FbxExRenderingPipeLineSettingData& MatData = MatrialSet[MatrialSet.size() - 1];
			MatData.Name = pMtrl->GetName();
			// fbxsdk::FbxSurfaceMaterial::sDiffuse = 0x00007ff61122bf40 "DiffuseColor"
			// fbxsdk::FbxSurfaceMaterial::sDiffuseFactor = 0x00007ff61122bf50 "DiffuseFactor"
			MatData.DifColor = MaterialColor(pMtrl, "DiffuseColor", "DiffuseFactor");
			MatData.AmbColor = MaterialColor(pMtrl, "AmbientColor", "AmbientFactor");
			MatData.SpcColor = MaterialColor(pMtrl, "SpecularColor", "SpecularFactor");
			MatData.EmvColor = MaterialColor(pMtrl, "EmissiveColor", "EmissiveFactor");
			MatData.SpecularPower = MaterialFactor(pMtrl, "SpecularFactor");
			// fbxsdk::FbxSurfaceMaterial::sShininess = 0x00007ff61122bf80 "ShininessExponent"
			// fbxsdk::FbxSurfaceMaterial::sTransparencyFactor = 0x00007ff61122bfd8 "TransparencyFactor"
			MatData.Shininess = MaterialFactor(pMtrl, "ShininessExponent");
			MatData.TransparencyFactor = MaterialFactor(pMtrl, "TransparencyFactor");

			MatData.DifTexturePath = MaterialTex(pMtrl, "DiffuseColor");
			// fbxsdk::FbxSurfaceMaterial::sNormalMap = 0x00007ff68291bfa0 "NormalMap"
			MatData.NorTexturePath = MaterialTex(pMtrl, "NormalMap");
			MatData.SpcTexturePath = MaterialTex(pMtrl, "SpecularColor");

			//if (MatData.DifTexturePath != L"")
			//{
			//	if (true == GameEnginePath::IsExist(MatData.DifTexturePath))
			//	{
			//		DirectTexture2D::Load(MatData.DifTexturePath);
			//		MatData.DifTextureName = GameEnginePath::GetFileName(MatData.DifTexturePath);
			//	}
			//}

			//if (MatData.BmpTexturePath != L"")
			//{
			//	if (true == GameEnginePath::IsExist(MatData.BmpTexturePath))
			//	{
			//		DirectTexture2D::Load(MatData.BmpTexturePath);
			//		MatData.BmpTextureName = GameEnginePath::GetFileName(MatData.BmpTexturePath);
			//	}
			//}

			//if (MatData.SpcTexturePath != L"")
			//{
			//	if (true == GameEnginePath::IsExist(MatData.SpcTexturePath))
			//	{
			//		DirectTexture2D::Load(MatData.SpcTexturePath);
			//		MatData.SpcTextureName = GameEnginePath::GetFileName(MatData.SpcTexturePath);
			//	}
			//}

		}

	}
	else {
		GameEngineDebug::MsgBoxError("매쉬는 존재하지만 재질은 존재하지 않습니다.");
	}
}

float4 GameEngineFBXMesh::MaterialColor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _ColorName, const char* _FactorName)
{
	FbxDouble3 vResult(0, 0, 0);
	double dFactor = 0;
	FbxProperty ColorPro = pMtrl->FindProperty(_ColorName);
	FbxProperty FactorPro = pMtrl->FindProperty(_FactorName);

	if (true == ColorPro.IsValid() && true == FactorPro.IsValid())
	{
		vResult = ColorPro.Get<FbxDouble3>();
		dFactor = FactorPro.Get<FbxDouble>();

		if (dFactor != 1)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}
	}

	return float4((float)vResult[0], (float)vResult[1], (float)vResult[2]);
}

float GameEngineFBXMesh::MaterialFactor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName)
{
	double dFactor = 0;
	FbxProperty FactorPro = pMtrl->FindProperty(_FactorName);

	if (true == FactorPro.IsValid())
	{
		dFactor = FactorPro.Get<FbxDouble>();
	}

	return (float)dFactor;
}

std::string GameEngineFBXMesh::MaterialTex(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName)
{
	return "";

	fbxsdk::FbxProperty TexturePro = pMtrl->FindProperty(_FactorName);
	std::string Str;
	if (true == TexturePro.IsValid())
	{
		fbxsdk::FbxObject* pFileTex = TexturePro.GetFbxObject();

		int a = 0;

		//TexturePro.
		// int iTexCount = TexturePro.GetSrcObjectCount<FbxFileTexture>();

		//if (iTexCount > 0)
		//{
		//	FbxFileTexture* pFileTex = TexturePro.GetSrcObject<FbxFileTexture>(0);

		//	// TexturePro.

		//	if (nullptr != pFileTex)
		//	{
		//		Str = pFileTex->GetFileName();
		//	}
		//}
		//else
		//{
		//	return "";
		//}
	}
	else
	{
		return "";
	}

	return Str;
}

void GameEngineFBXMesh::CalAnimationVertexData(FbxMeshSet& _DrawSet)
{
	for (auto& _WISet : _DrawSet.MapWI)
	{
		std::vector<GameEngineVertex>* Ptr = _DrawSet.FbxVertexMap[_WISet.first];

		if (nullptr == Ptr)
		{
			GameEngineDebug::MsgBoxError("버텍스 데이터와 수집한 가중치 데이터가 매칭되지 않습니다.");
		}

		std::vector<GameEngineVertex>& VertexData = *Ptr;

		for (auto& _WI : _WISet.second)
		{
			double dWeight = 0.0;
			for (int n = 0; n < _WI.second.size(); ++n)
			{
				dWeight += _WI.second[n].Weight;
			}

			for (int n = 0; n < _WI.second.size(); ++n)
			{
				_WI.second[n].Weight /= dWeight;
			}

			if (_WI.second.size() > 4)
			{
				std::sort(_WI.second.begin(), _WI.second.end(), [](const FbxExIW& _Left, const FbxExIW& _Right) { return _Left.Weight > _Right.Weight; });

				double dInterPolate = 0.0;

				std::vector<FbxExIW>::iterator IterErase = _WI.second.begin() + 4;
				for (; IterErase != _WI.second.end(); ++IterErase)
				{
					dInterPolate += IterErase->Weight;
				}
				IterErase = _WI.second.begin() + 4;

				_WI.second.erase(IterErase, _WI.second.end());
				_WI.second[0].Weight += dInterPolate;
			};

			float Weight[4] = { 0.0f };
			int Index[4] = { 0 };

			for (size_t i = 0; i < _WI.second.size(); i++)
			{
				Weight[i] = (float)_WI.second[i].Weight;
				Index[i] = _WI.second[i].Index;
			}

			memcpy_s(VertexData[_WI.first].WEIGHT.Arr1D, sizeof(float4), Weight, sizeof(float4));
			memcpy_s(VertexData[_WI.first].BLENDINDICES, sizeof(float4), Index, sizeof(float4));
		}
	}
}

bool GameEngineFBXMesh::IsBone(fbxsdk::FbxNode* Link)
{
	fbxsdk::FbxNodeAttribute* Attr = Link->GetNodeAttribute();
	if (Attr)
	{
		fbxsdk::FbxNodeAttribute::EType AttrType = Attr->GetAttributeType();
		if (AttrType == fbxsdk::FbxNodeAttribute::eSkeleton ||
			AttrType == fbxsdk::FbxNodeAttribute::eMesh ||
			AttrType == fbxsdk::FbxNodeAttribute::eNull)
		{
			return true;
		}
	}

	return false;
}

bool GameEngineFBXMesh::IsNull(fbxsdk::FbxNode* Link)
{
	fbxsdk::FbxNodeAttribute* Attr = Link->GetNodeAttribute();
	if (Attr)
	{
		fbxsdk::FbxNodeAttribute::EType AttrType = Attr->GetAttributeType();
		if (AttrType == fbxsdk::FbxNodeAttribute::eNull)
		{
			return true;
		}
	}
	return false;
}

Bone* GameEngineFBXMesh::FindBone(int _Index)
{
	if (AllBones.size() <= _Index)
	{
		GameEngineDebug::MsgBoxError("본이 존재하지 않습니다.");
		return nullptr;
	}

	return &AllBones[_Index];
}

Bone* GameEngineFBXMesh::FindBone(std::string _Name)
{
	if (0 == AllBones.size())
	{
		ImportBone();
	}

	if (0 == AllBones.size())
	{
		return nullptr;
	}

	if (0 == AllFindMap.size())
	{
		GameEngineDebug::MsgBoxError("본을 찾는 작업을 하지 않은 매쉬입니다");
	}

	if (AllFindMap.end() == AllFindMap.find(_Name))
	{
		return nullptr;
	}

	return AllFindMap[_Name];
}

void GameEngineFBXMesh::RecursiveBuildSkeleton(fbxsdk::FbxNode* Link, std::vector<fbxsdk::FbxNode*>& OutSortedLinks)
{
	if (IsBone(Link))
	{
		if (false == IsNull(Link))
		{
			OutSortedLinks.push_back(Link);
		}
		int ChildIndex;
		for (ChildIndex = 0; ChildIndex < Link->GetChildCount(); ChildIndex++)
		{
			RecursiveBuildSkeleton(Link->GetChild(ChildIndex), OutSortedLinks);
		}
	}
}

bool GameEngineFBXMesh::RetrievePoseFromBindPose(fbxsdk::FbxScene* pScene, const std::vector<fbxsdk::FbxNode*>& NodeArray, fbxsdk::FbxArray<fbxsdk::FbxPose*>& PoseArray)
{
	const int PoseCount = pScene->GetPoseCount();
	for (int PoseIndex = 0; PoseIndex < PoseCount; PoseIndex++)
	{
		fbxsdk::FbxPose* CurrentPose = pScene->GetPose(PoseIndex);

		if (CurrentPose && CurrentPose->IsBindPose())
		{
			std::string PoseName = CurrentPose->GetName();
			fbxsdk::FbxStatus Status;

			for (auto Current : NodeArray)
			{
				std::string CurrentName = Current->GetName();
				fbxsdk::NodeList pMissingAncestors, pMissingDeformers, pMissingDeformersAncestors, pWrongMatrices;

				if (CurrentPose->IsValidBindPoseVerbose(Current, pMissingAncestors, pMissingDeformers, pMissingDeformersAncestors, pWrongMatrices, 0.0001, &Status))
				{
					PoseArray.Add(CurrentPose);
					break;
				}
				else
				{
					for (int i = 0; i < pMissingAncestors.GetCount(); i++)
					{
						fbxsdk::FbxAMatrix mat = pMissingAncestors.GetAt(i)->EvaluateGlobalTransform(FBXSDK_TIME_ZERO);
						CurrentPose->Add(pMissingAncestors.GetAt(i), mat);
					}

					pMissingAncestors.Clear();
					pMissingDeformers.Clear();
					pMissingDeformersAncestors.Clear();
					pWrongMatrices.Clear();

					if (CurrentPose->IsValidBindPose(Current))
					{
						PoseArray.Add(CurrentPose);
						break;
					}
					else
					{
						fbxsdk::FbxNode* ParentNode = Current->GetParent();
						
						while (ParentNode)
						{
							fbxsdk::FbxNodeAttribute* Attr = ParentNode->GetNodeAttribute();
							if (Attr && Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNull)
							{
								break;
							}

							ParentNode = ParentNode->GetParent();
						}

						if (ParentNode && CurrentPose->IsValidBindPose(ParentNode))
						{
							PoseArray.Add(CurrentPose);
							break;
						}
						else
						{
							// 포즈(Pose)에 관련없는 Mesh인경우
							std::string ErrorString = Status.GetErrorString();
							std::string CurrentName = Current->GetName();

							GameEngineDebug::MsgBoxError(ErrorString + "_" + CurrentName);
						}
					}
				}
			}
		}
	}

	return (PoseArray.Size() > 0);
}

void GameEngineFBXMesh::BuildSkeletonSystem(fbxsdk::FbxScene* pScene, std::vector<fbxsdk::FbxCluster*>& ClusterArray, std::vector<fbxsdk::FbxNode*>& OutSortedLinks)
{
	fbxsdk::FbxNode* Link;
	std::vector<fbxsdk::FbxNode*> RootLinks;
	size_t ClusterIndex;
	for (ClusterIndex = 0; ClusterIndex < ClusterArray.size(); ClusterIndex++)
	{
		Link = ClusterArray[ClusterIndex]->GetLink();
		if (Link)
		{
			Link = GetRootSkeleton(pScene, Link);
			size_t LinkIndex;
			for (LinkIndex = 0; LinkIndex < RootLinks.size(); LinkIndex++)
			{
				if (Link == RootLinks[static_cast<int>(LinkIndex)])
				{
					break;
				}
			}

			if (LinkIndex == RootLinks.size())
			{
				RootLinks.push_back(Link);
			}
		}
	}

	for (size_t LinkIndex = 0; LinkIndex < RootLinks.size(); LinkIndex++)
	{
		RecursiveBuildSkeleton(RootLinks[LinkIndex], OutSortedLinks);
	}
}

fbxsdk::FbxNode* GameEngineFBXMesh::GetRootSkeleton(fbxsdk::FbxScene* pScene, fbxsdk::FbxNode* Link)
{
	fbxsdk::FbxNode* RootBone = Link;

	while (RootBone && RootBone->GetParent())
	{
		bool bIsBlenderArmatureBone = false;

		fbxsdk::FbxNodeAttribute* Attr = RootBone->GetParent()->GetNodeAttribute();
		if (Attr &&
			(Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh ||
				(Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNull && !bIsBlenderArmatureBone) ||
				Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton) &&
			RootBone->GetParent() != pScene->GetRootNode())
		{
			if (Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
			{
				fbxsdk::FbxMesh* Mesh = (fbxsdk::FbxMesh*)Attr;
				if (Mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin) > 0)
				{
					break;
				}
			}

			RootBone = RootBone->GetParent();
		}
		else
		{
			break;
		}
	}

	return RootBone;
}

bool GameEngineFBXMesh::ImportBone()
{
	// 이미 Bone정보를 로드하여 가지고있다면 중복 처리하지않는다.
	if (0 != AllBones.size())
	{
		return false;
	}

	size_t meshCount = MeshInfos.size();
	if (0 == meshCount)
	{
		return false;
	}

	// 스키닝 정보를 가지고있는 것
	std::vector<fbxsdk::FbxNode*> NodeArray;
	std::vector<fbxsdk::FbxNode*> SortedLinks;

	// 모든 스키닝 관련 정보를 모으는것(클러스터)
	std::vector<fbxsdk::FbxCluster*> ClusterArray;
	fbxsdk::FbxNode* Link = nullptr;
	int SkelType = 0;
	fbxsdk::FbxNode* SkeletalMeshNode = nullptr;

	for (size_t n = 0; n < meshCount; ++n)
	{
		FbxExMeshInfo& meshInfo = MeshInfos.at(n);

		// Lod가 적용되어 있으면 
		if (0 != meshInfo.VertexOrder)
		{
			continue;
		}

		fbxsdk::FbxNode* pNode = meshInfo.Mesh->GetNode();
		fbxsdk::FbxMesh* FbxMesh = meshInfo.Mesh;

		NodeArray.push_back(pNode);
		const int SkinDeformerCount = FbxMesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
		for (int DeformerIndex = 0; DeformerIndex < SkinDeformerCount; DeformerIndex++)
		{
			fbxsdk::FbxSkin* Skin = (fbxsdk::FbxSkin*)FbxMesh->GetDeformer(DeformerIndex, fbxsdk::FbxDeformer::eSkin);
			for (int ClusterIndex = 0; ClusterIndex < Skin->GetClusterCount(); ClusterIndex++)
			{
				ClusterArray.push_back(Skin->GetCluster(ClusterIndex));
			}
		}
	}

	if (0 == ClusterArray.size())
	{
		return false;
	}

	SkeletalMeshNode = NodeArray[0];

	fbxsdk::PoseList PoseArray;

	// 
	if (RetrievePoseFromBindPose(Scene, NodeArray, PoseArray) == false)
	{
		const int PoseCount = Scene->GetPoseCount();
		for (int PoseIndex = PoseCount - 1; PoseIndex >= 0; --PoseIndex)
		{
			fbxsdk::FbxPose* CurrentPose = Scene->GetPose(PoseIndex);

			if (CurrentPose && CurrentPose->IsBindPose())
			{
				Scene->RemovePose(PoseIndex);
				CurrentPose->Destroy();
			}
		}

		Manager->CreateMissingBindPoses(Scene);
		if (RetrievePoseFromBindPose(Scene, NodeArray, PoseArray) == false)
		{
			GameEngineDebug::MsgBoxError("Recreating bind pose failed.");
		}
		else
		{
		}
	}

	// 
	BuildSkeletonSystem(Scene, ClusterArray, SortedLinks);

	if (SortedLinks.size() == 0)
	{
		GameEngineDebug::MsgBoxError("연결된 링크가 없다.");
	}

	std::map<fbxsdk::FbxString, int> m_NameBoneCheck;
	size_t LinkIndex;

	for (LinkIndex = 0; LinkIndex < SortedLinks.size(); ++LinkIndex)
	{
		Link = SortedLinks[LinkIndex];
		m_NameBoneCheck.insert(std::make_pair(Link->GetName(), 0));

		for (size_t AltLinkIndex = LinkIndex + 1; AltLinkIndex < SortedLinks.size(); ++AltLinkIndex)
		{
			fbxsdk::FbxNode* AltLink = SortedLinks[AltLinkIndex];

			fbxsdk::FbxString tempLinkName = Link->GetName();
			fbxsdk::FbxString tempAltLinkName = AltLink->GetName();

			std::map<fbxsdk::FbxString, int>::iterator FindBoneNameIter = m_NameBoneCheck.find(tempAltLinkName);

			// 같은 이름의 본이 있어서
			// 기존 본의 겹치는 이름을 변경해준다.
			if (FindBoneNameIter != m_NameBoneCheck.end())
			{
				fbxsdk::FbxString newName = FindBoneNameIter->first;
				newName += "_";
				newName += (++FindBoneNameIter->second);

				// 만약에 바꿨는데도 또 있어
				std::map<fbxsdk::FbxString, int>::iterator RevertIter = m_NameBoneCheck.find(newName);

				while (RevertIter != m_NameBoneCheck.find(newName))
				{
					newName = FindBoneNameIter->first;
					newName += "_";
					newName += (++FindBoneNameIter->second);
					RevertIter = m_NameBoneCheck.find(newName);
				}

				// 겹치는 이름의 링크는 이 새이름을 바꾸면
				// 앞으로 펼쳐질 본과 관련된 모든곳에서
				// 이 이름으로 계산될것이므로 걱정할 필요가 없어진다.
				AltLink->SetName(newName.Buffer());

				// GameEngineDebug::OutPutMsg(tempLinkName + L"  " + tempAltLinkName);
				// GameEngineDebug::AssertMsg(L"같은 링크");
			}
		}
	}

	fbxsdk::FbxArray<fbxsdk::FbxAMatrix> GlobalsPerLink;
	GlobalsPerLink.Grow(static_cast<int>(SortedLinks.size()));
	GlobalsPerLink[0] = ConvertMatrix;

	bool GlobalLinkFoundFlag;
	fbxsdk::FbxVector4 LocalLinkT;
	fbxsdk::FbxQuaternion LocalLinkQ;
	fbxsdk::FbxVector4	LocalLinkS;
	fbxsdk::FbxVector4 GlobalLinkT;
	fbxsdk::FbxQuaternion GlobalLinkQ;
	fbxsdk::FbxVector4	GlobalLinkS;

	bool bAnyLinksNotInBindPose = false;
	std::string LinksWithoutBindPoses;
	int NumberOfRoot = 0;

	int RootIdx = -1;

	for (LinkIndex = 0; LinkIndex < SortedLinks.size(); LinkIndex++)
	{
		Bone& tempBoneData = AllBones.emplace_back();
		tempBoneData.Index = static_cast<int>(AllBones.size() - 1);

		Link = SortedLinks[LinkIndex];

		int ParentIndex = -1;
		fbxsdk::FbxNode* LinkParent = Link->GetParent();
		if (LinkIndex)
		{
			for (int ll = 0; ll < LinkIndex; ++ll)
			{
				fbxsdk::FbxNode* Otherlink = SortedLinks[ll];
				if (Otherlink == LinkParent)
				{
					ParentIndex = ll;
					break;
				}
			}
		}

		if (ParentIndex == -1)
		{
			++NumberOfRoot;
			RootIdx = static_cast<int>(LinkIndex);
		}

		GlobalLinkFoundFlag = false;
		if (!SkelType)
		{
			if (PoseArray.GetCount())
			{
				for (int PoseIndex = 0; PoseIndex < PoseArray.GetCount(); PoseIndex++)
				{
					int PoseLinkIndex = PoseArray[PoseIndex]->Find(Link);
					if (PoseLinkIndex >= 0)
					{
						fbxsdk::FbxMatrix NoneAffineMatrix = PoseArray[PoseIndex]->GetMatrix(PoseLinkIndex);
						fbxsdk::FbxAMatrix Matrix = *(fbxsdk::FbxAMatrix*)(double*)&NoneAffineMatrix;
						GlobalsPerLink[static_cast<int>(LinkIndex)] = Matrix;
						GlobalLinkFoundFlag = true;
						break;
					}
				}
			}

			if (!GlobalLinkFoundFlag)
			{
				for (int ClusterIndex = 0; ClusterIndex < ClusterArray.size(); ClusterIndex++)
				{
					fbxsdk::FbxCluster* Cluster = ClusterArray[ClusterIndex];
					if (Link == Cluster->GetLink())
					{
						Cluster->GetTransformLinkMatrix(GlobalsPerLink[static_cast<int>(LinkIndex)]);
						GlobalLinkFoundFlag = true;
						break;
					}
				}
			}
		}

		if (!GlobalLinkFoundFlag)
		{
			GlobalsPerLink[static_cast<int>(LinkIndex)] = Link->EvaluateGlobalTransform();
		}

		GlobalsPerLink[static_cast<int>(LinkIndex)] = GlobalsPerLink[static_cast<int>(LinkIndex)];
		if (LinkIndex &&
			-1 != ParentIndex)
		{
			fbxsdk::FbxAMatrix	Matrix;
			Matrix = GlobalsPerLink[static_cast<int>(ParentIndex)].Inverse() * GlobalsPerLink[static_cast<int>(LinkIndex)];
			LocalLinkT = Matrix.GetT();
			LocalLinkQ = Matrix.GetQ();
			LocalLinkS = Matrix.GetS();
			GlobalLinkT = GlobalsPerLink[static_cast<int>(LinkIndex)].GetT();
			GlobalLinkQ = GlobalsPerLink[static_cast<int>(LinkIndex)].GetQ();
			GlobalLinkS = GlobalsPerLink[static_cast<int>(LinkIndex)].GetS();
		}
		else
		{
			GlobalLinkT = LocalLinkT = GlobalsPerLink[static_cast<int>(LinkIndex)].GetT();
			GlobalLinkQ = LocalLinkQ = GlobalsPerLink[static_cast<int>(LinkIndex)].GetQ();
			GlobalLinkS = LocalLinkS = GlobalsPerLink[static_cast<int>(LinkIndex)].GetS();
		}

		Bone& Bone = AllBones[static_cast<int>(LinkIndex)];

		Bone.Name = Link->GetName();

		JointPos& BonePosData = Bone.BonePos;
		fbxsdk::FbxSkeleton* Skeleton = Link->GetSkeleton();
		if (Skeleton)
		{
			BonePosData.Length = (float)Skeleton->LimbLength.Get();
			BonePosData.XSize = (float)Skeleton->Size.Get();
			BonePosData.YSize = (float)Skeleton->Size.Get();
			BonePosData.ZSize = (float)Skeleton->Size.Get();
		}
		else
		{
			BonePosData.Length = 1.;
			BonePosData.XSize = 100.;
			BonePosData.YSize = 100.;
			BonePosData.ZSize = 100.;
		}

		Bone.ParentIndex = ParentIndex;
		Bone.NumChildren = 0;
		for (int ChildIndex = 0; ChildIndex < Link->GetChildCount(); ChildIndex++)
		{
			fbxsdk::FbxNode* Child = Link->GetChild(ChildIndex);
			if (IsBone(Child))
			{
				Bone.NumChildren++;
			}
		}

		BonePosData.SetTranslation(LocalLinkT);
		BonePosData.SetRotation(LocalLinkQ);
		BonePosData.SetScale(LocalLinkS);
		BonePosData.SetGlobalTranslation(GlobalLinkT);
		BonePosData.SetGlobalRotation(GlobalLinkQ);
		BonePosData.SetGlobalScale(GlobalLinkS);
		BonePosData.BuildMatrix();
	}

	for (size_t i = 0; i < AllBones.size(); i++)
	{
		if (AllFindMap.end() == AllFindMap.find(AllBones[i].Name))
		{
			AllFindMap.insert(std::make_pair(AllBones[i].Name, &AllBones[i]));
			continue;
		}

		std::multimap<std::string, Bone*>::iterator it, itlow, itup;

		itlow = AllFindMap.lower_bound(AllBones[i].Name);  // itlow points to b
		itup = AllFindMap.upper_bound(AllBones[i].Name);   // itup points to e (not d)

		int Count = 0;
		for (it = itlow; it != itup; ++it)
		{
			++Count;
		}

		std::string Name = AllBones[i].Name + std::to_string(Count);
		AllFindMap.insert(std::make_pair(Name, &AllBones[i]));
	}

	// 
	AnimationBuffer = new GameEngineStructuredBuffer();
	AnimationBuffer->Create(sizeof(float4x4), static_cast<UINT>(AllBones.size()), nullptr);

	LoadSkinAndCluster();

	return true;
}
