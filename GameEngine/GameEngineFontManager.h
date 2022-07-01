#pragma once

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineFont;
class GameEngineFontManager
{
private:
	static GameEngineFontManager* Inst;

public:
	static GameEngineFontManager& GetInst()
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
	std::map<std::string, GameEngineFont*> ResourcesMap;

private:
	GameEngineFontManager();
	~GameEngineFontManager();

protected:
	GameEngineFontManager(const GameEngineFontManager& _other) = delete;
	GameEngineFontManager(GameEngineFontManager&& _other) noexcept;

private:
	GameEngineFontManager& operator=(const GameEngineFontManager& _other) = delete;
	GameEngineFontManager& operator=(const GameEngineFontManager&& _other) = delete;

public:
	GameEngineFont* Load(const std::string& _Path);
	GameEngineFont* Find(const std::string& _Name);

public:
};
