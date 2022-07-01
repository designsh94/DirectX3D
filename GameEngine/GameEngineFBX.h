#pragma once

// ���̺귯������ �������
#pragma warning(push)
#pragma warning(disable:26495)
#pragma warning(disable:26451)

#include <fbxsdk.h>

// FBX SDK(�ܺ�)
#pragma comment(lib, "zlib-md.lib")			// ��������
#pragma comment(lib, "libxml2-md.lib")		// ��������
#pragma comment(lib, "libfbxsdk.lib")		// Ư�� static �Լ� ��ũ�����߻�
//#pragma comment(lib, "libfbxsdk-md.lib")	// �޸𸮴����߻�(visual studio 2022(fbx sdk v2014)�� fbx sdk v2019�� �������̷� �޸𸮸��̹߻�)

#pragma warning(pop)

// �з� : 
// �뵵 : 
// ���� : 
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

