#include "PreCompile.h"
#include "GameEngineSound.h"
#include "GameEngineDebug.h"
#include "GameEngineSound.h"
#include "GameEngineSoundPlayer.h"
#include "GameEnginePath.h"

GameEngineSoundManager* GameEngineSoundManager::Inst = new GameEngineSoundManager();

GameEngineSoundManager::GameEngineSoundManager() :
	soundSystem_(nullptr)
{
}

GameEngineSoundManager::~GameEngineSoundManager()
{
	// 사운드 플레이어
	std::list<GameEngineSoundPlayer*>::iterator SoundPlayerStartIter = allSoundPlayer_.begin();
	std::list<GameEngineSoundPlayer*>::iterator SoundPlayerEndIter = allSoundPlayer_.end();
	for (; SoundPlayerStartIter != SoundPlayerEndIter; ++SoundPlayerStartIter)
	{
		if (nullptr != *SoundPlayerStartIter)
		{
			delete *SoundPlayerStartIter;
		}
	}

	allSoundPlayer_.clear();

	// 사운드
	std::map<std::string, GameEngineSound*>::iterator SoundStartIter = allLoadSound_.begin();
	std::map<std::string, GameEngineSound*>::iterator SoundEndIter = allLoadSound_.end();
	for (; SoundStartIter != SoundEndIter; ++SoundStartIter)
	{
		if (nullptr != SoundStartIter->second)
		{
			delete SoundStartIter->second;
			SoundStartIter->second = nullptr;
		}
	}

	allLoadSound_.clear();

	// 사운드 시스템 반환
	if (nullptr != soundSystem_)
	{
		soundSystem_->release();
		soundSystem_ = nullptr;
	}
}

GameEngineSoundManager::GameEngineSoundManager(GameEngineSoundManager&& _other) noexcept :
	soundSystem_(nullptr)
{
}

GameEngineSound* GameEngineSoundManager::FindSound(const std::string& _name)
{
	std::map<std::string, GameEngineSound*>::iterator FindIter = allLoadSound_.find(_name);

	if (FindIter == allLoadSound_.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void GameEngineSoundManager::Initialize()
{
	FMOD::System_Create(&soundSystem_);
	if (nullptr == soundSystem_)
	{
		GameEngineDebug::MsgBoxError("sound system create Error");
		return;
	}

	if (FMOD_OK != soundSystem_->init(32, FMOD_DEFAULT, nullptr))
	{
		GameEngineDebug::MsgBoxError("sound system init Error");
		return;
	}
}

void GameEngineSoundManager::Load(const std::string& _path) 
{
	Load(GameEnginePath::GetFileName(_path), _path);
}

void GameEngineSoundManager::Load(const std::string& _name, const std::string& _path)
{
	if (nullptr != FindSound(_name))
	{
		GameEngineDebug::MsgBoxError("Sound Load overlap error");
		return;
	}

	GameEngineSound* newLoadSound = new GameEngineSound();
	if (false == newLoadSound->Load(_path))
	{
		GameEngineDebug::MsgBoxError("Sound Load Error");
		delete newLoadSound;
		return;
	}

	allLoadSound_.insert(std::map<std::string, GameEngineSound*>::value_type(_name, newLoadSound));
}

GameEngineSoundPlayer* GameEngineSoundManager::CreateSoundPlayer()
{
	GameEngineSoundPlayer* NewSoundplayer = new GameEngineSoundPlayer();
	allSoundPlayer_.push_back(NewSoundplayer);

	return  NewSoundplayer;
}

void GameEngineSoundManager::PlaySoundOneShot(const std::string& _name) 
{
	GameEngineSound* SoundPtr = FindSound(_name);

	if (nullptr == SoundPtr)
	{
		GameEngineDebug::MsgBoxError("PlaySound Error");
		return;
	}

	soundSystem_->playSound(SoundPtr->sound_, nullptr, false, nullptr);
}

void GameEngineSoundManager::SoundUpdate()
{
	if (nullptr == soundSystem_)
	{
		GameEngineDebug::MsgBoxError("SoundSystem Is null");
		return;
	}

	soundSystem_->update();
}
