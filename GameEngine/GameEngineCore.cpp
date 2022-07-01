#include "PreCompile.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>

#include "GameEngineCore.h"
#include "GameEngineWindow.h"
#include "GameEngineResourcesManager.h"
#include "GameEngineDevice.h"
#include "GameEngineLevel.h"
#include "GameEngineInput.h"
#include "GameEngineCollision.h"
#include "GameEngineGUI.h"

GameEngineThreadQueue GameEngineCore::ThreadQueue = GameEngineThreadQueue("GameEngineThread");

GameEngineCore* GameEngineCore::MainCore_ = nullptr;
std::map<std::string, GameEngineLevel*> GameEngineCore::AllLevel_;
GameEngineLevel* GameEngineCore::NextLevel_ = nullptr;
GameEngineLevel* GameEngineCore::CurrentLevel_ = nullptr;

GameEngineCore::GameEngineCore()
{
}

GameEngineCore::~GameEngineCore()
{
}

GameEngineCore::GameEngineCore(GameEngineCore&& _other) noexcept
{
}

void GameEngineCore::EngineInitialize()
{
	GameEngineDevice::GetInst().Initialize();
	EngineResourcesLoad();
	EngineResourcesCreate();
	GameEngineDevice::GetInst().CreateSwapChain();

	GameEngineGUI::GetInst()->Initialize();
	GameEngineCollision::Init();
	GameEngineSoundManager::GetInst().Initialize();
}

void GameEngineCore::EngineDestroy()
{
	for (auto& Level : AllLevel_)
	{
		if (true)
		{
			delete  Level.second;
			Level.second = nullptr;
		}
	}

	ThreadQueue.Destroy();

	GameEngineManagerHelper::ManagerRelease();
	GameEngineInput::Destroy();
	GameEngineTime::Destroy();
	GameEngineGUI::Destroy();

	GameEngineWindow::Destroy();
	GameEngineDevice::Destroy();
}

void GameEngineCore::MainLoop()
{
	GameEngineTime::GetInst().TimeCheck();
	GameEngineSoundManager::GetInst().SoundUpdate();
	GameEngineInput::GetInst().Update();

	if (nullptr != NextLevel_)
	{
		if (nullptr == CurrentLevel_)
		{
			CurrentLevel_ = NextLevel_;
			NextLevel_->LevelChangeStartActorEvent(NextLevel_);
			NextLevel_->LevelChangeStartEvent(NextLevel_);
		}
		else
		{
			CurrentLevel_->LevelChangeEndActorEvent(NextLevel_);
			CurrentLevel_->LevelChangeEndEvent(NextLevel_);
			CurrentLevel_->SetLevelActorMoveProcess();

			NextLevel_->LevelChangeStartActorEvent(CurrentLevel_);
			NextLevel_->LevelChangeStartEvent(CurrentLevel_);
			NextLevel_->SetLevelActorMoveProcess();

			CurrentLevel_ = NextLevel_;
		}

		NextLevel_ = nullptr;

		GameEngineTime::GetInst().TimeCheckReset();
	}

	if (nullptr == CurrentLevel_)
	{
		GameEngineDebug::MsgBoxError("현재 레벨이 존재하지 않습니다.");
	}

	float Time = GameEngineTime::GetInst().GetDeltaTime();

	CurrentLevel_->LevelUpdate(Time);
	CurrentLevel_->ActorUpdate(Time);
	CurrentLevel_->Render(Time);
	CurrentLevel_->Release(Time);
}

void GameEngineCore::WindowCreate(GameEngineCore& _RuntimeCore)
{
	GameEngineWindow::GetInst().CreateMainWindow("MainWindow", _RuntimeCore.StartWindowSize(), _RuntimeCore.StartWindowPos());
}

void GameEngineCore::LevelDestroy(const std::string& _Level)
{
	GameEngineLevel* Level = LevelFind(_Level);

	if (nullptr == Level)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 레벨을 지우려고 했습니다.");
		return;
	}

	AllLevel_.erase(AllLevel_.find(_Level));
	delete Level;
}

void GameEngineCore::Loop()
{
	GameEngineTime::GetInst().TimeCheckReset();
	GameEngineWindow::GetInst().Loop(&GameEngineCore::MainLoop);
}

GameEngineLevel* GameEngineCore::LevelFind(const std::string& _Level)
{
	std::map<std::string, GameEngineLevel*>::iterator FindIter = AllLevel_.find(_Level);
	if (FindIter != AllLevel_.end())
	{
		return FindIter->second;
	}
	return nullptr;
}

void GameEngineCore::LevelChange(const std::string& _Level)
{
	GameEngineLevel* FindLevel = LevelFind(_Level);

	if (FindLevel == CurrentLevel_)
	{
		return;
	}

	if (nullptr == FindLevel)
	{
		GameEngineDebug::MsgBoxError("Next Level Is Nullptr");
	}

	NextLevel_ = FindLevel;
}
