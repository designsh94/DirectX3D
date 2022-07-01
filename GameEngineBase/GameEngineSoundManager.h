#pragma once

// 분류 : 사운드 관리자
// 용도 : 
// 설명 : FMOD 사용을 체크하며 사운드 로드 및 기본 인터페이스 제공
class GameEngineSound;
class GameEngineSoundPlayer;
class GameEngineSoundManager
{
private:
	static GameEngineSoundManager* Inst;

public:
	static GameEngineSoundManager& GetInst()
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

public:
	friend GameEngineSound;
	friend GameEngineSoundPlayer;

private:
	FMOD::System* soundSystem_;

private:
	std::map<std::string, GameEngineSound*> allLoadSound_;
	std::list<GameEngineSoundPlayer*> allSoundPlayer_;

private:		
	GameEngineSoundManager();
	~GameEngineSoundManager();

public:
	GameEngineSoundManager(const GameEngineSoundManager& _other) = delete;
	GameEngineSoundManager(GameEngineSoundManager&& _other) noexcept;

public:
	GameEngineSoundManager& operator=(const GameEngineSoundManager& _other) = delete;
	GameEngineSoundManager& operator=(const GameEngineSoundManager&& _other) = delete;

private:
	GameEngineSound* FindSound(const std::string& _name);

public:
	void Initialize();
	void Load(const std::string& _path);
	void Load(const std::string& _name, const std::string& _path);
	
public:
	GameEngineSoundPlayer* CreateSoundPlayer();
	void PlaySoundOneShot(const std::string& _name);

public:
	void SoundUpdate();
};

