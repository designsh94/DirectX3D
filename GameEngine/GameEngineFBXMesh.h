#pragma once
#include "GameEngineFBX.h"
#include "EngineVertex.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineVertexBuffer.h"

class FbxExIW
{
public:
	int Index;			// Bone Index
	double Weight;		// 비율
};

class FbxExRenderingPipeLineSettingData
{
public:
	std::string Name;				// 

public:	// 픽셀쉐이더에서 최종색상에 곱해주면 되는 색상
	float4 DifColor;				// 분산색 : 난반사로 인해 조명에 상관없이 관측자에게 동일하게 보여지는 색
	float4 AmbColor;				// 주변색 : 빛이 없을때 마치 주변으로부터의 반사된 빛이 나타내는것처럼 보여질 색
	float4 SpcColor;				// 반사색 : 주광이 반사되어 관측자에게 향할때 보여지는 색
	float4 EmvColor;				// 발광색 : 스스로 빛을 발산하는 색
	float SpecularPower;			// 반사의 정도
	float TransparencyFactor;		// 
	float Shininess;				// 빛의 정도

public: // 각 텍스쳐 Path & Name
	std::string DifTexturePath;		// Diffuse Texture Path
	std::string NorTexturePath;		// NormalMap Texture Path
	std::string SpcTexturePath;		// Specular Texture Path
	std::string DifTextureName;		// Diffuse Texutre Name
	std::string NorTextureName;		// NormalMap Texture Name
	std::string SpcTextureName;		// Specular Texture Name

public:
	FbxExRenderingPipeLineSettingData() :
		Name(),													// std::string
		DifColor(), AmbColor(), SpcColor(), EmvColor(),			// float4
		SpecularPower(), TransparencyFactor(), Shininess(),		// float
		DifTexturePath(), NorTexturePath(), SpcTexturePath(),	// std::string
		DifTextureName(), NorTextureName(), SpcTextureName()	// std::string
	{
	}

	~FbxExRenderingPipeLineSettingData() 
	{
	}
};


struct FbxMeshSet 
{
public:
	int IsLodLv;
	bool IsLod;

public:
	std::map<FbxMesh*, std::vector<GameEngineVertex>*> FbxVertexMap;
	std::map<FbxMesh*, std::map<int, std::vector<FbxExIW>>> MapWI;

	std::vector<std::vector<GameEngineVertex>> Vertexs;
	std::vector<std::vector<std::vector<UINT>>> Indexs;

	std::vector<GameEngineVertexBuffer*> GameEngineVertexBuffers;
	std::vector<std::vector<GameEngineIndexBuffer*>> GameEngineIndexBuffers;
	std::vector<std::vector<FbxExRenderingPipeLineSettingData>> MatialData;

	FbxMeshSet() :
		IsLod(false),
		IsLodLv(-1)
	{
	}

	~FbxMeshSet()
	{
		for (size_t i = 0; i < GameEngineVertexBuffers.size(); i++)
		{
			if (nullptr == GameEngineVertexBuffers[i])
			{
				continue;
			}
			delete GameEngineVertexBuffers[i];
			GameEngineVertexBuffers[i] = nullptr;
		}

		for (size_t i = 0; i < GameEngineIndexBuffers.size(); i++)
		{
			for (size_t j = 0; j < GameEngineIndexBuffers[i].size(); j++)
			{
				if (nullptr == GameEngineIndexBuffers[i][j])
				{
					continue;
				}
				delete GameEngineIndexBuffers[i][j];
				GameEngineIndexBuffers[i][j] = nullptr;
			}
		}
	}
};

struct JointPos
{
	float4 Scale;					// 
	float4 Rotation;				// 
	float4 Translation;				// 

	float4 GlobalScale;				// 
	float4 GlobalRotation;			// 
	float4 GlobalTranslation;		// 
	
	float4x4 Offset;				// 
	float4x4 SotredOffset;			// 
	float4x4 Local;					// 
	float4x4 Global;				// 

	float Length;					// 
	float XSize;					// 
	float YSize;					// 
	float ZSize;					// 

	void BuildMatrix()
	{
		Global = float4x4::Transformation(GlobalScale, GlobalRotation, GlobalTranslation);
		Local = float4x4::Transformation(Scale, Rotation, Translation);
		Offset = Global.InverseReturn();
	}

	void SetScale(fbxsdk::FbxVector4& scale)
	{
		Scale.x = (float)scale.mData[0];
		Scale.y = (float)scale.mData[1];
		Scale.z = (float)scale.mData[2];
		Scale.w = 0.0f;
	}

	void SetRotation(fbxsdk::FbxQuaternion& quaternion)
	{
		Rotation.x = (float)quaternion.mData[0];
		Rotation.y = (float)quaternion.mData[1];
		Rotation.z = -(float)quaternion.mData[2];
		Rotation.w = -(float)quaternion.mData[3];
	}

	void SetTranslation(fbxsdk::FbxVector4& translation)
	{
		Translation.x = (float)translation.mData[0];
		Translation.y = (float)translation.mData[1];
		Translation.z = -(float)translation.mData[2];
		Translation.w = 0.0f;
	}

	void SetGlobalScale(fbxsdk::FbxVector4& scale)
	{
		GlobalScale.x = (float)scale.mData[0];
		GlobalScale.y = (float)scale.mData[1];
		GlobalScale.z = (float)scale.mData[2];
		GlobalScale.w = 0.0f;
	}

	void SetGlobalRotation(fbxsdk::FbxQuaternion& quaternion)
	{
		GlobalRotation.x = (float)quaternion.mData[0];
		GlobalRotation.y = (float)quaternion.mData[1];
		GlobalRotation.z = -(float)quaternion.mData[2];
		GlobalRotation.w = -(float)quaternion.mData[3];
	}

	void SetGlobalTranslation(fbxsdk::FbxVector4& translation)
	{
		GlobalTranslation.x = (float)translation.mData[0];
		GlobalTranslation.y = (float)translation.mData[1];
		GlobalTranslation.z = -(float)translation.mData[2];
		GlobalTranslation.w = 0.0f;
	}

	void SetFromMatrix(const float4x4& InMatrix)
	{
		float4x4 M = InMatrix;

		// Get the 3D scale from the matrix
		Scale = M.ExtractScaling();
		Scale.w = 0.0;

		// If there is negative scaling going on, we handle that here
		if (InMatrix.Determinant() < 0.f)
		{
			// Assume it is along X and modify transform accordingly. 
			// It doesn't actually matter which axis we choose, the 'appearance' will be the same
			float4 FloatMinus1_111(-1.f, 1.f, 1.f, 1.f);
			Scale *= FloatMinus1_111;// VectorMultiply(Scale, FloatMinus1_111);
			float4 XAxis = -float4(M.Arr2D[0][0], M.Arr2D[0][1], M.Arr2D[0][2], 0.0f);
			M.Arr2D[0][0] = XAxis.x;
			M.Arr2D[0][1] = XAxis.w;
			M.Arr2D[0][2] = XAxis.z;
		}

		Rotation = float4::MatrixToQuaternion(M);
		Translation = float4(InMatrix.Arr2D[3][0], InMatrix.Arr2D[3][1], InMatrix.Arr2D[3][2], 0.0f);

		// Normalize rotation
		__m128 Temp1 = _mm_mul_ps(Rotation.DirectVector, Rotation.DirectVector);
		__m128 Temp2 = _mm_shuffle_ps(Temp1, Temp1, _MM_SHUFFLE(1, 0, 3, 2));	// (Z,W,X,Y).
		Temp1 = _mm_add_ps(Temp1, Temp2);								// (X*X + Z*Z, Y*Y + W*W, Z*Z + X*X, W*W + Y*Y)
		Temp2 = _mm_shuffle_ps(Temp1, Temp1, _MM_SHUFFLE(0, 3, 2, 1));
		__m128 SquareSum = _mm_add_ps(Temp1, Temp2);
		const __m128 NonZeroMask = _mm_cmpge_ps(SquareSum, _mm_setr_ps(1.e-8f, 1.e-8f, 1.e-8f, 1.e-8f));
		const __m128 OneHalf = _mm_setr_ps(0.5f, 0.5f, 0.5f, 0.5f);
		const __m128 VecDivBy2 = _mm_mul_ps(SquareSum, OneHalf);

		// Initial estimate
		const __m128 x0 = _mm_rsqrt_ps(SquareSum);

		// First iteration
		__m128 x1 = _mm_mul_ps(x0, x0);
		x1 = _mm_sub_ps(OneHalf, _mm_mul_ps(VecDivBy2, x1));
		x1 = _mm_add_ps(_mm_mul_ps(x0, x1), x0);

		// Second iteration
		__m128 x2 = _mm_mul_ps(x1, x1);
		x2 = _mm_sub_ps(OneHalf, _mm_mul_ps(VecDivBy2, x2));
		const __m128 InvLength = _mm_add_ps(_mm_mul_ps(x1, x2), x1);

		const __m128 NormalizedVector = _mm_mul_ps(InvLength, Rotation.DirectVector);
		__m128 DefaultValue = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
		Rotation.DirectVector = _mm_xor_ps(DefaultValue, _mm_and_ps(NonZeroMask, _mm_xor_ps(NormalizedVector, DefaultValue)));
	}

	void ToMatrixInternal(__m128& OutDiagonals, __m128& OutAdds, __m128& OutSubtracts) const
	{
		const __m128 RotationX2Y2Z2 = _mm_add_ps(Rotation.DirectVector, Rotation.DirectVector);	// x2, y2, z2
		const __m128 RotationXX2YY2ZZ2 = _mm_mul_ps(RotationX2Y2Z2, Rotation.DirectVector);	// xx2, yy2, zz2		

		// The diagonal terms of the rotation matrix are:
		//   (1 - (yy2 + zz2)) * scale
		//   (1 - (xx2 + zz2)) * scale
		//   (1 - (xx2 + yy2)) * scale
		const __m128 yy2_xx2_xx2 = _mm_shuffle_ps(RotationXX2YY2ZZ2, RotationXX2YY2ZZ2, _MM_SHUFFLE(0, 0, 0, 1));
		const __m128 zz2_zz2_yy2 = _mm_shuffle_ps(RotationXX2YY2ZZ2, RotationXX2YY2ZZ2, _MM_SHUFFLE(0, 1, 2, 2));
		const __m128 DiagonalSum = _mm_add_ps(yy2_xx2_xx2, zz2_zz2_yy2);
		const __m128 Diagonals = _mm_sub_ps(float4::ONE.DirectVector, DiagonalSum);
		OutDiagonals = _mm_mul_ps(Diagonals, Scale.DirectVector);

		// Grouping the non-diagonal elements in the rotation block by operations:
		//    ((x*y2,y*z2,x*z2) + (w*z2,w*x2,w*y2)) * scale.xyz and
		//    ((x*y2,y*z2,x*z2) - (w*z2,w*x2,w*y2)) * scale.yxz
		// Rearranging so the LHS and RHS are in the same order as for +
		//    ((x*y2,y*z2,x*z2) - (w*z2,w*x2,w*y2)) * scale.yxz

		// RotBase = x*y2, y*z2, x*z2
		// RotOffset = w*z2, w*x2, w*y2
		const __m128 x_y_x = _mm_shuffle_ps(Rotation.DirectVector, Rotation.DirectVector, _MM_SHUFFLE(0, 0, 1, 0)); // 1, 2, 2, 0
		const __m128 y2_z2_z2 = _mm_shuffle_ps(RotationX2Y2Z2, RotationX2Y2Z2, _MM_SHUFFLE(0, 2, 2, 1));
		const __m128 RotBase = _mm_mul_ps(x_y_x, y2_z2_z2);

		const __m128 w_w_w = _mm_shuffle_ps(Rotation.DirectVector, Rotation.DirectVector, _MM_SHUFFLE(3, 3, 3, 3));
		const __m128 z2_x2_y2 = _mm_shuffle_ps(RotationX2Y2Z2, RotationX2Y2Z2, _MM_SHUFFLE(0, 1, 0, 2));
		const __m128 RotOffset = _mm_mul_ps(w_w_w, z2_x2_y2);

		// Adds = (RotBase + RotOffset)*Scale3D :  (x*y2 + w*z2) * Scale3D.X , (y*z2 + w*x2) * Scale3D.Y, (x*z2 + w*y2) * Scale3D.Z
		// Subtracts = (RotBase - RotOffset)*Scale3DYZX :  (x*y2 - w*z2) * Scale3D.Y , (y*z2 - w*x2) * Scale3D.Z, (x*z2 - w*y2) * Scale3D.X
		const __m128 Adds = _mm_add_ps(RotBase, RotOffset);
		OutAdds = _mm_mul_ps(Adds, Scale.DirectVector);
		const __m128 Scale3DYZXW = _mm_shuffle_ps(Scale.DirectVector, Scale.DirectVector, _MM_SHUFFLE(3, 0, 2, 1));
		const __m128 Subtracts = _mm_sub_ps(RotBase, RotOffset);
		OutSubtracts = _mm_mul_ps(Subtracts, Scale3DYZXW);
	}

	float4x4 ToMatrixWithScale()
	{
		float4x4 OutMatrix;
		__m128 DiagonalsXYZ;
		__m128 Adds;
		__m128 Subtracts;

		ToMatrixInternal(DiagonalsXYZ, Adds, Subtracts);
		union { __m128 v; __m128i i; } tmp;
		tmp.i = _mm_setr_epi32((unsigned int)0xffffffff, (unsigned int)0xffffffff, (unsigned int)0xffffffff, (unsigned int)0x00000000);
		const __m128 DiagonalsXYZ_W0 = _mm_and_ps(DiagonalsXYZ, tmp.v);

		// OutMatrix.M[0][0] = (1.0f - (yy2 + zz2)) * Scale.X;    // Diagonal.X
		// OutMatrix.M[0][1] = (xy2 + wz2) * Scale.X;             // Adds.X
		// OutMatrix.M[0][2] = (xz2 - wy2) * Scale.X;             // Subtracts.Z
		// OutMatrix.M[0][3] = 0.0f;                              // DiagonalsXYZ_W0.W
		const __m128 AddX_DC_DiagX_DC = _mm_shuffle_ps(Adds, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 0, 0, 0));
		const __m128 SubZ_DC_DiagW_DC = _mm_shuffle_ps(Subtracts, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 3, 0, 2));
		const __m128 Row0 = _mm_shuffle_ps(AddX_DC_DiagX_DC, SubZ_DC_DiagW_DC, _MM_SHUFFLE(2, 0, 0, 2));

		// OutMatrix.M[1][0] = (xy2 - wz2) * Scale.Y;             // Subtracts.X
		// OutMatrix.M[1][1] = (1.0f - (xx2 + zz2)) * Scale.Y;    // Diagonal.Y
		// OutMatrix.M[1][2] = (yz2 + wx2) * Scale.Y;             // Adds.Y
		// OutMatrix.M[1][3] = 0.0f;                            // DiagonalsXYZ_W0.W
		const __m128 SubX_DC_DiagY_DC = _mm_shuffle_ps(Subtracts, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 1, 0, 0));
		const __m128 AddY_DC_DiagW_DC = _mm_shuffle_ps(Adds, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 3, 0, 1));
		const __m128 Row1 = _mm_shuffle_ps(SubX_DC_DiagY_DC, AddY_DC_DiagW_DC, _MM_SHUFFLE(2, 0, 2, 0));

		// OutMatrix.M[2][0] = (xz2 + wy2) * Scale.Z;             // Adds.Z
		// OutMatrix.M[2][1] = (yz2 - wx2) * Scale.Z;             // Subtracts.Y
		// OutMatrix.M[2][2] = (1.0f - (xx2 + yy2)) * Scale.Z;    // Diagonals.Z
		// OutMatrix.M[2][3] = 0.0f;                              // DiagonalsXYZ_W0.W
		const __m128 AddZ_DC_SubY_DC = _mm_shuffle_ps(Adds, Subtracts, _MM_SHUFFLE(0, 1, 0, 2));
		const __m128 Row2 = _mm_shuffle_ps(AddZ_DC_SubY_DC, DiagonalsXYZ_W0, _MM_SHUFFLE(3, 2, 2, 0));

		_mm_store_ps(&(OutMatrix.Arr2D[0][0]), Row0);
		_mm_store_ps(&(OutMatrix.Arr2D[1][0]), Row1);
		_mm_store_ps(&(OutMatrix.Arr2D[2][0]), Row2);

		// OutMatrix.M[3][0] = Translation.X;
		// OutMatrix.M[3][1] = Translation.Y;
		// OutMatrix.M[3][2] = Translation.Z;
		// OutMatrix.M[3][3] = 1.0f;
		__m128 Temp1 = _mm_movehl_ps(float4::ONE.DirectVector, Translation.DirectVector);

		const __m128 Row3 = _mm_shuffle_ps(Translation.DirectVector, Temp1, _MM_SHUFFLE(3, 0, 1, 0));
		_mm_store_ps(&(OutMatrix.Arr2D[3][0]), Row3);

		return OutMatrix;
	}

	void Reset()
	{
		Scale = float4::ONE;
		Rotation = float4::ZERO;
		Translation = float4::ZERO;

		Offset.Identity();
	}

	JointPos()
	{
		Scale = float4::ONE;
		Rotation = float4::ZERO;
		Translation = float4::ZERO;

		Offset.Identity();
	}
};

struct Bone
{
	std::string Name;

	unsigned int Flags;
	int			 NumChildren;
	int			 Index;
	int			 ParentIndex;
	JointPos	 BonePos;

	Bone()
	{
		Name = "";
		Flags = 0;
		NumChildren = 0;
		Index = -1;
		ParentIndex = 0;
		BonePos.Reset();
	}
};

struct FbxExMeshInfo
{
	std::string Name;
	fbxsdk::FbxMesh* Mesh;
	bool bTriangulated;

	unsigned __int64 UniqueId;
	int FaceNum;
	int VertexNum;
	int MaterialNum;

	bool bIsSkelMesh;
	std::string SkeletonRoot;
	int SkeletonElemNum;

	bool bIsLodGroup;
	std::string LODGroup;
	int VertexOrder;
	int MorphNum;

	FbxExMeshInfo()
	{
		Name = "";
		UniqueId = 0;
		FaceNum = 0;
		VertexNum = 0;
		bTriangulated = false;
		MaterialNum = 0;
		bIsSkelMesh = false;
		SkeletonRoot = "";
		SkeletonElemNum = 0;
		bIsLodGroup = false;
		LODGroup = "";
		VertexOrder = 0;
		MorphNum = 0;
	}
};

class FbxClusterData
{
public:
	fbxsdk::FbxCluster* Cluster;
	fbxsdk::FbxMesh* Mesh;
	std::string LinkName;
};

// 분류 : 
// 용도 : 
// 설명 : 
class FBXAnimation;
class GameEngineFBXAnimation;
class GameEngineStructuredBuffer;
class GameEngineFBXMesh : public GameEngineFBX
{
	friend FBXAnimation;
	friend GameEngineFBXAnimation;

private:
	bool IsAnimationLoadOnce;
	std::vector<FbxExMeshInfo> MeshInfos;
	std::vector<Bone> AllBones;
	std::map<int, FbxMeshSet> AllMeshMap;
	std::map<std::string, Bone*> AllFindMap;				// Bone의 이름이 겹치는 경우가 생기므로 이름으로 관리
	std::map<int, std::vector<FbxClusterData>> ClusterData;	// 
	GameEngineStructuredBuffer* AnimationBuffer;			// 구조화버퍼 : 애니메이션 

public:
	GameEngineFBXMesh();
	~GameEngineFBXMesh();

protected:
	GameEngineFBXMesh(const GameEngineFBXMesh& _Other) = delete;
	GameEngineFBXMesh(GameEngineFBXMesh&& _Other) noexcept = delete;

private:
	GameEngineFBXMesh& operator=(const GameEngineFBXMesh& _Other) = delete;
	GameEngineFBXMesh& operator=(GameEngineFBXMesh&& _Other) noexcept = delete;

public:
	std::map<int, FbxMeshSet>& GetMeshSet()
	{
		return AllMeshMap;
	}

	size_t GetBoneCount()
	{
		return AllBones.size();
	}

	GameEngineStructuredBuffer* GetAnimationBuffer()
	{
		return AnimationBuffer;
	}

public:
	void Load(const std::string& _Path);
	void MeshLoad();
	void CreateRenderingBuffer();

private:
	fbxsdk::FbxNode* RecursiveFindParentLodGroup(fbxsdk::FbxNode* parentNode);

	void MeshNodeCheck();
	void VertexBufferCheck();

	fbxsdk::FbxAMatrix ComputeTotalMatrix(fbxsdk::FbxNode* Node);
	bool IsOddNegativeScale(const fbxsdk::FbxAMatrix& TotalMatrix);

	// 탄젠트 바이노말 계산
	void LoadBinormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);
	void LoadTangent(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);
	void LoadNormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);

	// 
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	// 클러스터 데이터를 처리
	void ImportCluster();
	void LoadSkinAndCluster();
	void LoadAnimationVertexData(FbxMeshSet* _DrawData, const std::vector<FbxClusterData>& vecClusterData);
	void DrawSetWeightAndIndexSetting(FbxMeshSet* _DrawSet, fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxCluster* _Cluster, int _BoneIndex);
	void CalAnimationVertexData(FbxMeshSet& _DrawSet);

	// 재질관련(텍스처)
	void FbxMeshSetMaterialSetting(fbxsdk::FbxNode* _Node, FbxMeshSet* _RenderData);
	float4 MaterialColor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _ColorName, const char* _FactorName);
	float MaterialFactor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName);
	std::string MaterialTex(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName);

public: // 애니메이션관련
	// Check
	bool IsBone(fbxsdk::FbxNode* Link);
	bool IsNull(fbxsdk::FbxNode* Link);
	Bone* FindBone(int _Index);
	Bone* FindBone(std::string _Name);

	// 재귀
	void RecursiveBuildSkeleton(fbxsdk::FbxNode* Link, std::vector<fbxsdk::FbxNode*>& OutSortedLinks);

	// 추출??
	bool RetrievePoseFromBindPose(fbxsdk::FbxScene* pScene, const std::vector<fbxsdk::FbxNode*>& NodeArray, fbxsdk::FbxArray<fbxsdk::FbxPose*>& PoseArray);
	void BuildSkeletonSystem(fbxsdk::FbxScene* pScene, std::vector<fbxsdk::FbxCluster*>& ClusterArray, std::vector<fbxsdk::FbxNode*>& OutSortedLinks);
	fbxsdk::FbxNode* GetRootSkeleton(fbxsdk::FbxScene* pScene, fbxsdk::FbxNode* Link);

	// 본(Bone)정보 수집
	bool ImportBone();

public:
};

