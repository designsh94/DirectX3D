#pragma once
#include "GameEngineDevice.h"
#include <mutex>

// ���� : 
class GameEngineStructuredBuffer;
class GameEngineStructuredBufferManager
{
private:
	static GameEngineStructuredBufferManager* Inst;
	static std::mutex ManagerLock;

public:
	static GameEngineStructuredBufferManager& GetInst()
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

private:	// member Var
	std::map<std::string, GameEngineStructuredBuffer*> ResourcesMap;

public:
	// ���� ����� �ִ�.
	GameEngineStructuredBuffer* Create(const std::string& _Name, const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo);

	GameEngineStructuredBuffer* CreateAndFind(const std::string& _Name, const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo);

	GameEngineStructuredBuffer* Find(const std::string& _Name);

private:
	GameEngineStructuredBufferManager(); // default constructer ����Ʈ ������
	~GameEngineStructuredBufferManager(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineStructuredBufferManager(const GameEngineStructuredBufferManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineStructuredBufferManager(GameEngineStructuredBufferManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineStructuredBufferManager& operator=(const GameEngineStructuredBufferManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineStructuredBufferManager& operator=(const GameEngineStructuredBufferManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
};




