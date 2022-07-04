#pragma once

// 분류 : 
// 용도 : Management
// 설명 : 게임의 input들을 관리해주고 정리하는 클래스
class GameEngineInput
{
private:
    class GameEngineKey 
    {
        friend GameEngineInput;

    private:
        bool Down_;
        bool Press_;
        bool Up_;
        bool Free_;

    private:
        std::vector<int> CheckKey_;

    public:
        GameEngineKey() :
            Down_(false), Press_(false), Up_(false), Free_(false)
        {

        }

    public:
        void PushKey(int _CurKey)
        {
            CheckKey_.push_back(_CurKey);
        }

        void PushKey()
        {
        }

    private:
        bool KeyCheck();
        void Update();
        void Reset();
    };

private:
    static GameEngineInput* Inst;

public:
    static GameEngineInput& GetInst()
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
    static bool IsKey(const std::string& _Name);
    static bool Down(const std::string& _Name);
    static bool Up(const std::string& _Name);
    static bool Press(const std::string& _Name);
    static bool Free(const std::string& _Name);

public:
    static float4 GetMousePos();
    static float4 GetMouse3DPos();
    static float4 GetMouse3DDir();

public:
    static void HideCursor() 
    {
        ShowCursor(false);
    }

private: // Input Key
    std::map<std::string, GameEngineKey*> AllKey_;

private: // Mouse
    float4 MousePos_;
    float4 MousePos3D_;
    float4 PrevMousePos3D_;
    float4 MouseDir3D_;

public:
    GameEngineInput();
    ~GameEngineInput();
    GameEngineInput(const GameEngineInput& _Other) = delete;
    GameEngineInput(const GameEngineInput&& _Other) = delete;

public:
    GameEngineInput operator=(const GameEngineInput& _Other) = delete;
    GameEngineInput operator=(const GameEngineInput&& _Other) = delete;

private:
    GameEngineKey* FindKey(const std::string& _KeyName);

public:
    void CreateKey(const std::string& _KeyName, int _Key)
    {
        if (nullptr != FindKey(_KeyName))
        {
            assert("if (nullptr != FindKey(_KeyName))");
            return;
        }

        if ('a' <= _Key && 'z' >= _Key)
        {
            _Key = std::toupper(_Key);
        }

        GameEngineKey* NewKey = new GameEngineKey();
        NewKey->Reset();
        NewKey->PushKey(_Key);
        AllKey_.insert(std::map<std::string, GameEngineKey*>::value_type(_KeyName, NewKey));
    }

public:
    void Update();
};

