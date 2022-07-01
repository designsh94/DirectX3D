#include "PreCompile.h"
#include "GameEngineSoundPlayer.h"
#include "GameEngineSound.h"
#include "GameEngineDebug.h"

GameEngineSoundPlayer::GameEngineSoundPlayer() :
	playSoundFile_(nullptr),
	playChannel_(nullptr),
	PlayCount(-1),
	Volume_(0.5f)
{
}

GameEngineSoundPlayer::~GameEngineSoundPlayer()
{
}

GameEngineSoundPlayer::GameEngineSoundPlayer(GameEngineSoundPlayer&& _other) noexcept :
	playSoundFile_(_other.playSoundFile_),
	playChannel_(_other.playChannel_),
	PlayCount(_other.PlayCount),
	Volume_(_other.Volume_)
{
}

bool GameEngineSoundPlayer::IsPlay() 
{
	bool Check = false;
	playChannel_->isPlaying(&Check);

	return Check;
}

void GameEngineSoundPlayer::PlayCountReset(int _Count) 
{
	PlayCount = _Count;
}

void GameEngineSoundPlayer::PlayOverLap(const std::string& _name, int _LoopCount)
{
	GameEngineSound* SoundPtr = GameEngineSoundManager::GetInst().FindSound(_name);
	if (nullptr == SoundPtr)
	{
		GameEngineDebug::MsgBoxError("PlaySound Error");
		return;
	}

	if (0 == PlayCount)
	{
		return;
	}

	GameEngineSoundManager::GetInst().soundSystem_->playSound(SoundPtr->sound_, nullptr, false, &playChannel_);

	--PlayCount;
	playChannel_->setLoopCount(_LoopCount);	
}

void GameEngineSoundPlayer::PlayAlone(const std::string& _name, int _LoopCount /*= 1*/) 
{
	GameEngineSound* SoundPtr = GameEngineSoundManager::GetInst().FindSound(_name);
	if (nullptr == SoundPtr)
	{
		GameEngineDebug::MsgBoxError("PlaySound Error");
		return;
	}

	if (true == IsPlay())
	{
		return;
	}

	if (0 == PlayCount)
	{
		return;
	}

	GameEngineSoundManager::GetInst().soundSystem_->playSound(SoundPtr->sound_, nullptr, false, &playChannel_);

	--PlayCount;
	playChannel_->setLoopCount(_LoopCount);

	// 이전의 볼륨으로 셋팅
	SetVolume(Volume_);
}

void GameEngineSoundPlayer::Stop()
{
	if (nullptr == playChannel_)
	{
		return;
	}

	playChannel_->stop();
	playChannel_ = nullptr;
}

void GameEngineSoundPlayer::SetVolume(float _Volume)
{
	Volume_ = _Volume;
	if (playChannel_ != nullptr)
	{
		playChannel_->setVolume(Volume_);
	}
}

void GameEngineSoundPlayer::VolumeUp()
{
	Volume_ += 0.1f;
	if (1.f <= Volume_)
	{
		Volume_ = 1.f;
	}

	if (playChannel_ != nullptr)
	{
		playChannel_->setVolume(Volume_);
	}
}

void GameEngineSoundPlayer::VolumeDown()
{
	Volume_ -= 0.1f;
	if (0.f >= Volume_)
	{
		Volume_ = 0.f;
	}

	if (playChannel_ != nullptr)
	{
		playChannel_->setVolume(Volume_);
	}
}
