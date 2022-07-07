#pragma once
#include "GameEngineDevice.h"
#include <mutex>

// 설명 : 
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
	// 직접 만들수 있다.
	GameEngineStructuredBuffer* Create(const std::string& _Name, const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo);

	GameEngineStructuredBuffer* CreateAndFind(const std::string& _Name, const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo);

	GameEngineStructuredBuffer* Find(const std::string& _Name);

private:
	GameEngineStructuredBufferManager(); // default constructer 디폴트 생성자
	~GameEngineStructuredBufferManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineStructuredBufferManager(const GameEngineStructuredBufferManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineStructuredBufferManager(GameEngineStructuredBufferManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineStructuredBufferManager& operator=(const GameEngineStructuredBufferManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineStructuredBufferManager& operator=(const GameEngineStructuredBufferManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




