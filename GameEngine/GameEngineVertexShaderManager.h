#pragma once

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineVertexShader;
class GameEngineVertexShaderManager
{
private:
	static GameEngineVertexShaderManager* Inst;

public:
	static GameEngineVertexShaderManager& GetInst()
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
	std::map<std::string, GameEngineVertexShader*> ResourcesMap;

private:
	GameEngineVertexShaderManager();
	~GameEngineVertexShaderManager();

protected:
	GameEngineVertexShaderManager(const GameEngineVertexShaderManager& _other) = delete;
	GameEngineVertexShaderManager(GameEngineVertexShaderManager&& _other) noexcept;

private:
	GameEngineVertexShaderManager& operator=(const GameEngineVertexShaderManager& _other) = delete;
	GameEngineVertexShaderManager& operator=(const GameEngineVertexShaderManager&& _other) = delete;

public:
	GameEngineVertexShader* Create(const std::string& _Name, const std::string& _ShaderCode, UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		return Create(_Name, _ShaderCode, _Name, _VersionHigh, _VersionLow);
	}

public:
	GameEngineVertexShader* Create(const std::string& _Name, const std::string& _ShaderCode, const std::string& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);
	GameEngineVertexShader* Load(const std::string& _Path, const std::string& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);
	GameEngineVertexShader* Load(const std::string& _Name, const std::string& _Path, const std::string& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);
	GameEngineVertexShader* Find(const std::string& _Name);

public:
};
