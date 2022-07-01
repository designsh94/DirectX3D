#pragma once
#include "GameEngineDevice.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineBlend;
class GameEngineBlendManager
{
private:
	static GameEngineBlendManager* Inst;

public:
	static GameEngineBlendManager& GetInst()
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

private:
	std::map<std::string, GameEngineBlend*> ResourcesMap;

private:
	GameEngineBlendManager();
	~GameEngineBlendManager();

protected:
	GameEngineBlendManager(const GameEngineBlendManager& _other) = delete;
	GameEngineBlendManager(GameEngineBlendManager&& _other) noexcept;

private:
	GameEngineBlendManager& operator=(const GameEngineBlendManager& _other) = delete;
	GameEngineBlendManager& operator=(const GameEngineBlendManager&& _other) = delete;

public:
	GameEngineBlend* Create(const std::string& _Name, const D3D11_BLEND_DESC& _Info, float4 _Fector = float4::ONE, unsigned int _Mask = 0xfffffff);
	GameEngineBlend* Find(const std::string& _Name);
	GameEngineBlend* CreateAndFind(const std::string& _Name, const D3D11_BLEND_DESC& _Info, float4 _Fector = float4::ONE, unsigned int _Mask = 0xfffffff);

public:
};




