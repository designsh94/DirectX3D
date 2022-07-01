#pragma once

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineRenderTarget;
class GameEngineRenderTargetManager
{
private:
	static GameEngineRenderTargetManager* Inst;

public:
	static GameEngineRenderTargetManager& GetInst()
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
	std::map<std::string, GameEngineRenderTarget*> ResourcesMap;

private:
	GameEngineRenderTargetManager();
	~GameEngineRenderTargetManager();

protected:
	GameEngineRenderTargetManager(const GameEngineRenderTargetManager& _other) = delete;
	GameEngineRenderTargetManager(GameEngineRenderTargetManager&& _other) noexcept;

private:
	GameEngineRenderTargetManager& operator=(const GameEngineRenderTargetManager& _other) = delete;
	GameEngineRenderTargetManager& operator=(const GameEngineRenderTargetManager&& _other) = delete;

public:
	GameEngineRenderTarget* Create(const std::string& _Name, const std::string& _TextureName, float4 _Color);
	GameEngineRenderTarget* Load(const std::string& _Path);
	GameEngineRenderTarget* Load(const std::string& _Name, const std::string& _Path);
	GameEngineRenderTarget* Find(const std::string& _Name);

public:
};




