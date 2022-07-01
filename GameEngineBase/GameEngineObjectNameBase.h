#pragma once
#include "GameEngineObjectBase.h"

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineObjectNameBase : public GameEngineObjectBase
{
private:
	std::string name_;

public:
	GameEngineObjectNameBase();
	~GameEngineObjectNameBase();

protected:
	GameEngineObjectNameBase(const GameEngineObjectNameBase& _other);
	GameEngineObjectNameBase(GameEngineObjectNameBase&& _other) noexcept;

private:
	GameEngineObjectNameBase& operator=(const GameEngineObjectNameBase& _other) = delete;
	GameEngineObjectNameBase& operator=(const GameEngineObjectNameBase&& _other) = delete;

public:
	std::string GetName()
	{
		return name_;
	}

	void SetName(const std::string& _name)
	{
		name_ = _name;
	}
};

