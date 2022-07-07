#include "PreCompile.h"
#include "GameEngineStructuredBufferManager.h"
#include "GameEngineStructuredBuffer.h"

GameEngineStructuredBufferManager* GameEngineStructuredBufferManager::Inst = new GameEngineStructuredBufferManager();
std::mutex GameEngineStructuredBufferManager::ManagerLock;

GameEngineStructuredBufferManager::GameEngineStructuredBufferManager() // default constructer ����Ʈ ������
{

}

// [90] * float4x4

GameEngineStructuredBufferManager::~GameEngineStructuredBufferManager() // default destructer ����Ʈ �Ҹ���
{
	for (const std::pair<std::string, GameEngineStructuredBuffer*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineStructuredBufferManager::GameEngineStructuredBufferManager(GameEngineStructuredBufferManager&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}


GameEngineStructuredBuffer* GameEngineStructuredBufferManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineStructuredBuffer*>::iterator FindIter;
	{
		std::lock_guard Lock(ManagerLock);

		FindIter = ResourcesMap.find(GameEngineString::toupper(_Name));
	}

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;

}

GameEngineStructuredBuffer* GameEngineStructuredBufferManager::Create(const std::string& _Name, const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo)
{
	std::string UpperName = GameEngineString::toupper(_Name);


	GameEngineStructuredBuffer* FindRes = Find(UpperName);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
	}


	GameEngineStructuredBuffer* NewRes = new GameEngineStructuredBuffer();
	NewRes->SetName(UpperName);
	// NewRes->Create(_BufferDesc, _VarInfo, nullptr);

	// �׸��� ���Ұų�?

	ResourcesMap.insert(std::map<std::string, GameEngineStructuredBuffer*>::value_type(UpperName, NewRes));
	return NewRes;
}

GameEngineStructuredBuffer* GameEngineStructuredBufferManager::CreateAndFind(const std::string& _Name, const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineStructuredBuffer* FindRes = Find(UpperName);

	if (nullptr != FindRes)
	{
		return FindRes;
	}


	GameEngineStructuredBuffer* NewRes = new GameEngineStructuredBuffer();
	NewRes->SetName(UpperName);
	// NewRes->Create(_BufferDesc, _VarInfo);

	ResourcesMap.insert(std::map<std::string, GameEngineStructuredBuffer*>::value_type(UpperName, NewRes));

	return NewRes;
}
