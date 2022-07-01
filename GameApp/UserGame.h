#pragma once
#include <GameEngine/GameEngineCore.h>

// 분류 : 
// 용도 : 
// 설명 : 
class UserGame : public GameEngineCore
{
public:
	static std::atomic<int> LoadingFolder;

private:

public:
	UserGame();
	~UserGame();

protected:
	UserGame(const UserGame& _other) = delete;
	UserGame(UserGame&& _other) noexcept;

private:
	UserGame& operator=(const UserGame& _other) = delete;
	UserGame& operator=(const UserGame&& _other) = delete;

public:
	virtual float4 StartWindowPos()
	{
		return { 0, 0 };
	}

	virtual float4 StartWindowSize()
	{
		return { 1280, 720 };
	}

public:
	virtual void Initialize() override;
	virtual void ResourcesLoad() override;
	virtual void Release() override;
};
