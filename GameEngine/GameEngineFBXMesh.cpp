#include "PreCompile.h"
#include "GameEngineFBXMesh.h"

GameEngineFBXMesh::GameEngineFBXMesh() 
{
}

GameEngineFBXMesh::~GameEngineFBXMesh() 
{
}

float4x4 GameEngineFBXMesh::FbxMatTofloat4x4(const fbxsdk::FbxAMatrix& _BaseTrans)
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

fbxsdk::FbxAMatrix GameEngineFBXMesh::float4x4ToFbxAMatrix(const float4x4& _MATRIX)
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

float4 GameEngineFBXMesh::FbxVecTofloat4(const fbxsdk::FbxVector4& _BaseVector)
{
	float4 Vec;
	Vec.Arr1D[0] = (float)_BaseVector.mData[0];
	Vec.Arr1D[1] = (float)_BaseVector.mData[1];
	Vec.Arr1D[2] = (float)_BaseVector.mData[2];
	Vec.Arr1D[3] = (float)_BaseVector.mData[3];

	return Vec;
}

float4 GameEngineFBXMesh::FbxVecToTransform(const fbxsdk::FbxVector4& _BaseVector)
{
	float4 Vec;
	Vec.Arr1D[0] = (float)_BaseVector.mData[0];
	Vec.Arr1D[1] = (float)_BaseVector.mData[1];
	Vec.Arr1D[2] = -(float)_BaseVector.mData[2];
	Vec.Arr1D[3] = (float)_BaseVector.mData[3];

	return Vec;
}

float4 GameEngineFBXMesh::FbxQuaternionTofloat4(const fbxsdk::FbxQuaternion& _BaseQ)
{
	float4 Vec;
	Vec.Arr1D[0] = (float)_BaseQ.mData[0];
	Vec.Arr1D[1] = (float)_BaseQ.mData[1];
	Vec.Arr1D[2] = -(float)_BaseQ.mData[2];
	Vec.Arr1D[3] = -(float)_BaseQ.mData[3];

	return Vec;
}

void GameEngineFBXMesh::Load(const std::string& _Path) 
{
	if (false == CreateFBXSystemInitialize(_Path))
	{
		GameEngineDebug::MsgBoxError("FBX 이니셜라이즈에 실패했습니다.");
		return;
	}

	FBXConvertScene();
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
	int geometryCount = Scene->GetGeometryCount();
	for (int i = 0; i < geometryCount; i++)
	{
		// 노드중에서 기하구조를 가진것을 추출
		fbxsdk::FbxGeometry* geoMetry = Scene->GetGeometry(i);
		fbxsdk::FbxNode* geoMetryNode = geoMetry->GetNode();

		// 추출한 것의 타입이 메쉬이라면
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
			meshMatrix = JointConvertMatrix * meshMatrix;
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
