#pragma once
#include "GameEngineLevel.h"
#include <GameEngineBase\GameEngineThreadQueue.h>

// �з� : ���� �ھ� �� ����������
// �뵵 : 
// ���� : ���� ������ �ʿ��� �⺻������ �����ϸ�, �ش� Ŭ������ ��ӹ޴� ��ü�� �ݵ�� �ʱ�ȭ/���ҽ��ε�/���ӷ���/���������� �����ϵ��� ����
//            GameEngineLevel�� �������� ���ҵ� ����
class GameEngineCore : public GameEngineObjectBase
{
	friend class GameEngineLevelControlWindow;

public:
	static GameEngineThreadQueue ThreadQueue;

// ============================================= Level ������ ���� ============================================= //
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
			GameEngineDebug::MsgBoxError("���� �̸��� ������ 2�� ������� �߽��ϴ�");
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

// ============================================== GameCore ���� ============================================== //
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

		// ������ ����
		WindowCreate(NewUserGame);

		// ���� �ʱ�ȭ �� ���ҽ� �ε�
		NewUserGame.EngineInitialize();
		NewUserGame.ResourcesLoad();
		NewUserGame.Initialize();

		// ���ΰ��� �ھ� ����
		MainCore_ = &NewUserGame;

		// Game Loop
		Loop();

		// ���� �޸� �Ҹ�
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

