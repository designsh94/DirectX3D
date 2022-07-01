#pragma once
#include "GameEngineLevel.h"
#include <GameEngineBase\GameEngineThreadQueue.h>

// 분류 : 게임 코어 및 레벨관리자
// 용도 : 
// 설명 : 게임 구동에 필요한 기본구조를 제공하며, 해당 클래스를 상속받는 객체가 반드시 초기화/리소스로드/게임루프/릴리즈기능을 구현하도록 제공
//            GameEngineLevel을 관리자의 역할도 수행
class GameEngineCore : public GameEngineObjectBase
{
	friend class GameEngineLevelControlWindow;

public:
	static GameEngineThreadQueue ThreadQueue;

// ============================================= Level 관리자 관련 ============================================= //
private:
	static GameEngineLevel* NextLevel_;
	static GameEngineLevel* CurrentLevel_;
	static std::map<std::string, GameEngineLevel*> AllLevel_;

public:
	template<typename LevelType>
	static void LevelCreate(const std::string& _Level)
	{
		if (nullptr != LevelFind(_Level))
		{
			GameEngineDebug::MsgBoxError("같은 이름의 레벨을 2번 만들려고 했습니다");
			return;
		}

		AllLevel_.insert(std::make_pair(_Level, new LevelType()));
		AllLevel_[_Level]->SetName(_Level);
		AllLevel_[_Level]->Init();
		AllLevel_[_Level]->LevelStart();
	}

	static void LevelDestroy(const std::string& _Level);

	static GameEngineLevel* CurrentLevel() 
	{
		return CurrentLevel_;
	}

public:
	static void LevelChange(const std::string& _Level);
	static GameEngineLevel* LevelFind(const std::string& _Level);

// ============================================== GameCore 관련 ============================================== //
private:
	static GameEngineCore* MainCore_;

private:
	static void WindowCreate(GameEngineCore& _RuntimeCore);
	static void Loop();
	static void MainLoop();

public:
	template<typename UserGameType>
	static void Start()
	{
		GameEngineDebug::LeakCheckOn();

#ifdef _DEBUG
		new int();
#endif

		UserGameType NewUserGame;

		// 윈도우 생성
		WindowCreate(NewUserGame);

		// 엔진 초기화 및 리소스 로드
		NewUserGame.EngineInitialize();
		NewUserGame.ResourcesLoad();
		NewUserGame.Initialize();

		// 메인게임 코어 셋팅
		MainCore_ = &NewUserGame;

		// Game Loop
		Loop();

		// 엔진 메모리 소멸
		NewUserGame.Release();
		NewUserGame.EngineDestroy();
	}

private:

protected:
	GameEngineCore();
	~GameEngineCore();

protected:
	GameEngineCore(const GameEngineCore& _other) = delete;
	GameEngineCore(GameEngineCore&& _other) noexcept;

private:
	GameEngineCore& operator=(const GameEngineCore& _other) = delete;
	GameEngineCore& operator=(const GameEngineCore&& _other) = delete;

protected:
	virtual void Initialize() = 0;
	virtual void ResourcesLoad() = 0;
	virtual void Release() = 0;
	virtual float4 StartWindowSize() = 0;
	virtual float4 StartWindowPos() = 0;

protected:
	void EngineResourcesCreate_Mesh();
	void EngineResourcesCreate_Rasterizer();

public:
	void EngineInitialize();
	void EngineResourcesLoad();
	void EngineResourcesCreate();
	void EngineDestroy();
};

