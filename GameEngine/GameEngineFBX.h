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
// ���� : FBX File �ε� �� ���� ����(FBX File�� Ʈ������������� ����)
//		  1) FBX Load�� ���� fbxsdk::FbxManager�� ����(��üȭ)
//		  2) fbxsdk::FbxManager�� �̿��Ͽ� Scene�� ����(������ ��� FBX File�Ѱ��� Scene�� �Ѱ� �䱸)
//		  3) fbxsdk::FbxManager�� �̿��Ͽ� FBX File Load ������ �����ϴ� fbxsdk::FbxIOSettings ����(IOSROOT)
//		  4) fbxsdk::FbxManager�� �̿��Ͽ� fbxsdk::FbxImporter ����
//		  5) fbxsdk::FbxImporter�� �ʱ�ȭ -> FBX File�� �ִ� ���+���ϸ��� ����
//		  6) FBX File Load�� ���� �غ�Ϸ� -> ���� �ε尡 �Ϸ�Ǿ����Ƿ� fbxsdk::FbxImporter�� �Ҹ��Ű�ԵǸ� �޸� �Ҹ� ���ϼ�����
//		  7) 
class GameEngineFBX : public GameEngineObjectNameBase
{
protected: // FBX File Load �غ� ���� ��ü ���� �� Scene Get
	fbxsdk::FbxManager* Manager;				// FBX File �ε� �� ���� �������̽����� - �̱�������
	fbxsdk::FbxIOSettings* IOSetting;			// FBX File�� �д� ����� ���� - �̱�������
	fbxsdk::FbxImporter* Importer;				// ��(Scene)�� ��Ҹ� ���Ͽ��� �������ų� ���Ϸ� �������� ���θ� ����
	fbxsdk::FbxScene* Scene;					// FBX File���� ��(Scene)�� �ε��ϱ� ���� �� �����̳ʿ� ��ü

protected:
	fbxsdk::FbxNode* RootNode;					// �ش� FBX File�� Scene�� �����ϴ� �ֻ������(��Ʈ���)
	fbxsdk::FbxAMatrix ConvertMatrix;			// �� ��ȯ ���
	fbxsdk::FbxAMatrix JointMatrix;				// ���ʹϾ�
	fbxsdk::FbxVector4 AxisVector;				// ������

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

