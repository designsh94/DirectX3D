#pragma once
#include <GameEngineBase\GameEngineObjectNameBase.h>
#include <GameEngineBase\GameEngineDirectory.h>

#include "imgui.h"

class GameEngineGUIWindow;
class GameEngineGUI
{
	friend class GameEngineCore;
	friend class GameEngineLevel;

private:
	static GameEngineGUI* Inst_;

public:
	inline static GameEngineGUI* GetInst()
	{
		return Inst_;
	}

	static void Destroy()
	{
		if (nullptr != Inst_)
		{
			delete Inst_;
			Inst_ = nullptr;
		}
	}

private:
	std::list<GameEngineGUIWindow*> Windows_;
	
private:
	GameEngineGUI();
	~GameEngineGUI();

private:
	GameEngineGUI(const GameEngineGUI& _Other) = delete;
	GameEngineGUI(GameEngineGUI&& _Other) noexcept = delete;
	GameEngineGUI& operator=(const GameEngineGUI& _Other) = delete;
	GameEngineGUI& operator=(GameEngineGUI&& _Other) noexcept = delete;

private:
	void Initialize();
	void GUIRenderStart();
	void GUIRenderEnd();

public:
	template<typename WindowType>
	WindowType* CreateGUIWindow(const std::string& _Name)
	{
		WindowType* NewWindow = new WindowType();

		NewWindow->SetName(_Name);

		Windows_.push_back(NewWindow);

		return NewWindow;
	}

	template<typename ConvertType>
	ConvertType* FindGUIWindowConvert(const std::string& _Name)
	{
		return dynamic_cast<ConvertType*>(FindGUIWindow(_Name));
	}

public:
	GameEngineGUIWindow* FindGUIWindow(const std::string& _Name);
	std::list<GameEngineGUIWindow*> FindGUIWindowForList(const std::string& _Name);
};

class GameEngineGUIWindow : public GameEngineObjectNameBase
{
	friend GameEngineGUI;

protected:
	int Style_;

public:
	GameEngineGUIWindow();
	~GameEngineGUIWindow();

private:
	GameEngineGUIWindow(const GameEngineGUIWindow& _Other) = delete;
	GameEngineGUIWindow(GameEngineGUIWindow&& _Other) noexcept = delete;
	GameEngineGUIWindow& operator=(const GameEngineGUIWindow& _Other) = delete;
	GameEngineGUIWindow& operator=(GameEngineGUIWindow&& _Other) noexcept = delete;

public:
	virtual void Start() {};

public:
	void Begin()
	{
		ImGui::Begin(GetName().c_str(), &GetIsUpdateRef(), Style_);
	}

	virtual void OnGUI() = 0;

	void End()
	{
		ImGui::End();
	}
};