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
// 설명 : 
class GameEngineFBX : public GameEngineObjectNameBase
{
protected:
	fbxsdk::FbxManager* Manager;
	fbxsdk::FbxIOSettings* IOSetting;
	fbxsdk::FbxImporter* Importer;
	fbxsdk::FbxScene* Scene;
	fbxsdk::FbxNode* RootNode;
	fbxsdk::FbxAMatrix ConvertMatrix;
	fbxsdk::FbxAMatrix JointConvertMatrix;
	fbxsdk::FbxVector4 AxisVector;

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

public:
};

