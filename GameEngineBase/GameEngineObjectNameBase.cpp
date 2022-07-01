#include "PreCompile.h"
#include "GameEngineObjectNameBase.h"

GameEngineObjectNameBase::GameEngineObjectNameBase()
{
}

GameEngineObjectNameBase::~GameEngineObjectNameBase()
{
}

GameEngineObjectNameBase::GameEngineObjectNameBase(GameEngineObjectNameBase&& _other) noexcept
{
}

GameEngineObjectNameBase::GameEngineObjectNameBase(const GameEngineObjectNameBase& _other) :
	name_(_other.name_)
{
}
