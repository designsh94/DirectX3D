#include "PreCompile.h"
#include "GameEngineInput.h"
#include "GameEngineWindow.h"

//--------------------------------------------------- GameEngineInput::GameEngineKey:: ---------------------------------------------------//
bool GameEngineInput::GameEngineKey::KeyCheck() 
{
    if (0 == CheckKey_.size())
    {
        return false;
    }

    for (size_t i = 0; i < CheckKey_.size(); i++)
    {
        if (0 == GetAsyncKeyState(CheckKey_[i]))
        {
            return false;
        }
    }

    return true;
}

void GameEngineInput::GameEngineKey::Update()
{
    if (true == KeyCheck())
    {
        if (false == Press_)
        {
            Down_ = true;
            Press_ = true;
            Up_ = false;
            Free_ = false;
        }
        else 
        {
            Down_ = false;
            Press_ = true;
            Up_ = false;
            Free_ = false;
        }
    }
    else 
    {
        if (true == Press_)
        {
            Down_ = false;
            Press_ = false;
            Up_ = true;
            Free_ = false;
        }
        else
        {
            Down_ = false;
            Press_ = false;
            Up_ = false;
            Free_ = true;
        }

    }
}

void GameEngineInput::GameEngineKey::Reset() 
{
    Down_ = false;
    Press_ = false;
    Up_ = false;
    Free_ = true;
}

//---------------------------------------------------------- GameEngineInput:: -----------------------------------------------------------//
GameEngineInput* GameEngineInput::Inst = new GameEngineInput();

bool GameEngineInput::IsKey(const std::string & _Name)
{
    if (nullptr != GetInst().FindKey(_Name))
    {
        return true;
    }
    return false;
}

bool GameEngineInput::Down(const std::string& _Name)
{
    GameEngineKey* FindKey = GetInst().FindKey(_Name);

    if (nullptr == FindKey)
    {
        return false;
    }

    return FindKey->Down_;
}
bool GameEngineInput::Up(const std::string& _Name)
{
    GameEngineKey* FindKey = GetInst().FindKey(_Name);

    if (nullptr == FindKey)
    {
        return false;
    }

    return FindKey->Up_;
}
bool GameEngineInput::Press(const std::string& _Name)
{
    GameEngineKey* FindKey = GetInst().FindKey(_Name);

    if (nullptr == FindKey)
    {
        return false;
    }

    return FindKey->Press_;
}
bool GameEngineInput::Free(const std::string& _Name)
{
    GameEngineKey* FindKey = GetInst().FindKey(_Name);

    if (nullptr == FindKey)
    {
        return false;
    }

    return FindKey->Free_;
}

float4 GameEngineInput::GetMousePos()
{
    return Inst->MousePos_;
}

float4 GameEngineInput::GetMouse3DPos()
{
    return Inst->MousePos3D_;
}

float4 GameEngineInput::GetMouse3DDir()
{
    return Inst->MouseDir3D_;
}

GameEngineInput::GameEngineInput()
{
}

GameEngineInput::~GameEngineInput()
{
    for (std::pair<std::string, GameEngineKey*> PairData : AllKey_)
    {
        delete PairData.second;
        PairData.second = nullptr;
    }
}

GameEngineInput::GameEngineKey* GameEngineInput::FindKey(const std::string& _KeyName)
{
    std::map<std::string, GameEngineInput::GameEngineKey*>::iterator FindIter = AllKey_.find(_KeyName);

    if (FindIter == AllKey_.end())
    {
        return nullptr;
    }

    return FindIter->second;
}

void GameEngineInput::Update() 
{
    for (std::pair<std::string, GameEngineInput::GameEngineKey*>&& _Key : AllKey_)
    {
        _Key.second->Update();
    }

    POINT P;
    GetCursorPos(&P);
    ScreenToClient(GameEngineWindow::GetInst().GetWindowHWND(), &P);

    MousePos_.x = static_cast<float>(P.x);
    MousePos_.y = static_cast<float>(P.y);

    PrevMousePos3D_ = MousePos3D_;
    MousePos3D_.x = MousePos_.x - GameEngineWindow::GetInst().GetSize().hx();
    MousePos3D_.y = MousePos_.y - GameEngineWindow::GetInst().GetSize().hy();
    MousePos3D_.y *= -1.0f;

    MouseDir3D_ = MousePos3D_ - PrevMousePos3D_;
    MouseDir3D_.Normalize3D();
}
