#include "PreCompile.h"
#include <GameEngine\EngineVertex.h>
#include <GameEngine\GameEngineFontManager.h>

#include "UserGame.h"
#include "UserGame_Resources_Shader.h"

void TextureLoading(GameEngineDirectory Dir) 
{
	std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

	for (size_t i = 0; i < AllFile.size(); i++)
	{
		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	}

	--UserGame::LoadingFolder;
}

void UserGame::ResourcesLoad()
{
	GameEngineFontManager::GetInst().Load("±Ã¼­");

	{
		GameEngineDirectory SoundDir;
		SoundDir.MoveParent("DirectX3D");
		SoundDir.MoveChild("Resources");
		SoundDir.MoveChild("Sound");

		std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile("mp3");

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}

	{
		GameEngineDirectory TextureDir;
		TextureDir.MoveParent("DirectX3D");
		TextureDir.MoveChild("Resources");
		TextureDir.MoveChild("Image");

		std::vector<GameEngineDirectory> AllDir = TextureDir.GetAllDirectoryRecursive();
		LoadingFolder = static_cast<int>(AllDir.size());

		for (size_t i = 0; i < AllDir.size(); i++)
		{
			GameEngineCore::ThreadQueue.JobPost(std::bind(TextureLoading, AllDir[i]));
		}
	}

	AppShaderLoad();
}
