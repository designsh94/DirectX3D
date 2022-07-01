#include "PreCompile.h"
#include "GameEngineObjectBase.h"

GameEngineObjectBase::GameEngineObjectBase() :
	isDeath_(false),
	isUpdate_(true),
	isDebug_(false),
	Order_(0),
	isOrigin_(true),
	parent_(nullptr)
{
}

GameEngineObjectBase::~GameEngineObjectBase()
{
}

GameEngineObjectBase::GameEngineObjectBase(GameEngineObjectBase&& _other) noexcept :
	isDeath_(_other.isDeath_),
	isUpdate_(_other.isUpdate_),
	isDebug_(_other.isDebug_),
	Order_(_other.Order_),
	isOrigin_(_other.isOrigin_),
	parent_(_other.parent_)
{
}

GameEngineObjectBase::GameEngineObjectBase(const GameEngineObjectBase& _other) :
	isDeath_(_other.isDeath_),
	isUpdate_(_other.isUpdate_),
	isDebug_(_other.isDebug_),
	Order_(_other.Order_),
	isOrigin_(_other.isOrigin_),
	parent_(_other.parent_)
{
}