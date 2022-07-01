#include "PreCompile.h"
#include "GameEngineDebug.h"

GameEngineDebug::GameEngineDebug()
{
}

GameEngineDebug::~GameEngineDebug()
{
}

GameEngineDebug::GameEngineDebug(GameEngineDebug&& _other) noexcept
{
}

void GameEngineDebug::LeakCheckOn() 
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
}

void GameEngineDebug::MsgBoxError(const std::string& _Ptr)
{
	MessageBoxA(nullptr, _Ptr.c_str(), "Error", MB_OK);
	assert(false);
}

void GameEngineDebug::MsgBox(const std::string& _Ptr) 
{
	MessageBoxA(nullptr, _Ptr.c_str(), "Error", MB_OK);
}

void GameEngineDebug::AssertFalse()
{
	assert(false);
}

void GameEngineDebug::OutPutDebugString(const std::string& _Text)
{
	OutputDebugStringA(_Text.c_str());
}
