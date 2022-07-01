#pragma once
#include "GameEngineDevice.h"
#include "ThirdParty/Include/DirectXTex/DirectXTex.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineTexture;
class GameEngineFolderTexture : public GameEngineObjectNameBase
{
private:
	std::vector<GameEngineTexture*> AllTexture_;

public:
	GameEngineFolderTexture();
	~GameEngineFolderTexture();

protected:
	GameEngineFolderTexture(const GameEngineFolderTexture& _other) = delete;
	GameEngineFolderTexture(GameEngineFolderTexture&& _other) noexcept = delete;

private:
	GameEngineFolderTexture& operator=(const GameEngineFolderTexture& _other) = delete;
	GameEngineFolderTexture& operator=(const GameEngineFolderTexture&& _other) = delete;

public:
	inline int GetTextureCount()
	{
		return static_cast<int>(AllTexture_.size());
	}

	inline GameEngineTexture* GetTextureIndex(int _Index)
	{
		return AllTexture_[_Index];
	}

public:
	void Load(const std::string& _Path);
};

