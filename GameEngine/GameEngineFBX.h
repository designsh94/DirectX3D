#pragma once

// 라이브러리내부 경고제거
#pragma warning(push)
#pragma warning(disable:26495)
#pragma warning(disable:26451)

#include <fbxsdk.h>

// FBX SDK(외부)
#pragma comment(lib, "zlib-md.lib")			// 문제없음
#pragma comment(lib, "libxml2-md.lib")		// 문제없음
#pragma comment(lib, "libfbxsdk.lib")		// 특정 static 함수 링크에러발생
//#pragma comment(lib, "libfbxsdk-md.lib")	// 메모리누수발생(visual studio 2022(fbx sdk v2014)과 fbx sdk v2019의 버젼차이로 메모리릭이발생)

#pragma warning(pop)

// 분류 : 
// 용도 : 
// 설명 : FBX File 로드 및 정보 수집(FBX File은 트리노드형식으로 구성)
//		  1) FBX Load를 위해 fbxsdk::FbxManager를 생성(객체화)
//		  2) fbxsdk::FbxManager를 이용하여 Scene을 생성(보통의 경우 FBX File한개당 Scene을 한개 요구)
//		  3) fbxsdk::FbxManager를 이용하여 FBX File Load 형식을 정의하는 fbxsdk::FbxIOSettings 생성(IOSROOT)
//		  4) fbxsdk::FbxManager를 이용하여 fbxsdk::FbxImporter 생성
//		  5) fbxsdk::FbxImporter를 초기화 -> FBX File이 있는 경로+파일명을 전달
//		  6) FBX File Load를 위한 준비완료 -> 파일 로드가 완료되었으므로 fbxsdk::FbxImporter를 소멸시키게되면 메모리 소모를 줄일수있음
//		  7) 
class GameEngineFBX : public GameEngineObjectNameBase
{
protected: // FBX File Load 준비를 위한 객체 생성 및 Scene Get
	fbxsdk::FbxManager* Manager;				// FBX File 로드 및 관리 인터페이스제공 - 싱글톤형식
	fbxsdk::FbxIOSettings* IOSetting;			// FBX File을 읽는 방법을 정의 - 싱글톤형식
	fbxsdk::FbxImporter* Importer;				// 씬(Scene)의 요소를 파일에서 가져오거나 파일로 내보낼지 여부를 지정
	fbxsdk::FbxScene* Scene;					// FBX File에서 씬(Scene)을 로드하기 위해 씬 컨테이너용 객체

protected:
	fbxsdk::FbxNode* RootNode;					// 해당 FBX File의 Scene을 구성하는 최상위노드(루트노드)
	fbxsdk::FbxAMatrix ConvertMatrix;			// 축 변환 행렬
	fbxsdk::FbxAMatrix JointMatrix;				// 쿼터니언
	fbxsdk::FbxVector4 AxisVector;				// 축정보

public:
	GameEngineFBX();
	~GameEngineFBX();

protected:
	GameEngineFBX(const GameEngineFBX& _Other) = delete;
	GameEngineFBX(GameEngineFBX&& _Other) noexcept = delete;

private:
	GameEngineFBX& operator=(const GameEngineFBX& _Other) = delete;
	GameEngineFBX& operator=(GameEngineFBX&& _Other) noexcept = delete;

public:
	inline fbxsdk::FbxNode* GetRootNode()
	{
		return RootNode;
	}

public:
	void RecursiveAllNode(std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack, std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack, int _ParentReturn);
	void Reset();

protected:
	bool CreateFBXSystemInitialize(const std::string& _Path);
	void FBXConvertScene();
	void RecursiveAllNode(fbxsdk::FbxNode* _Node, std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack, std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack, int _ParentReturn);

protected:
	float4x4 FbxMatTofloat4x4(const fbxsdk::FbxAMatrix& _BaseTrans);
	fbxsdk::FbxAMatrix float4x4ToFbxAMatrix(const float4x4& _MATRIX);
	float4 FbxVecTofloat4(const fbxsdk::FbxVector4& _BaseVector);
	float4 FbxVecToTransform(const fbxsdk::FbxVector4& _BaseVector);
	float4 FbxQuaternionTofloat4(const fbxsdk::FbxQuaternion& _BaseQ);

public:
};

