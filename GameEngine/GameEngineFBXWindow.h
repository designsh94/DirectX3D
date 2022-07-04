#pragma once
#include "GameEngineGUI.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineFBXMeshManager.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>

enum class LoadInfoState
{
	NONE = -1,
	FILE,
	MESH,
	ANIMATION,
	ALL
};

class GameEngineFBXWindow : public GameEngineGUIWindow
{
private: // Font ����
	ImFont* TextFontStyle_Eng;							// (���ĺ���) : 24pt, arialbd.ttf(Arial ����)
	ImFont* LabelFontStyle_Eng;							// (���ĺ���) : 18pt, arialbd.ttf(Arial ����)
	ImFont* ButtonFontStyle_Eng;						// (���ĺ���) : 10pt, arialbd.ttf(Arial ����)

	ImFont* TextFontStyle_Kor;							// (�ѱۿ�) : 24pt, malgunbd.ttf(������� ����)
	ImFont* LabelFontStyle_Kor;							// (�ѱۿ�) : 18pt, malgunbd.ttf(������� ����)
	ImFont* ButtonFontStyle_Kor;						// (�ѱۿ�) : 10pt, malgunbd.ttf(������� ����)

	ImGuiStyle* FontColor;								// Styel�� Color 0��° �ε����� �̿��Ͽ� �ؽ�Ʈ������ ����

private: // Vector List
	std::vector<std::string> FileNames_Origin;			// Ž���� ���ϸ� ���(����)
	std::vector<std::string> FileNames;					// ������ ���ϸ� ���

private: // ListBox Select Index
	int FBXFileSelect;									// �������� ���õ� ���� �ε���
	int ActorSelect;									// �������� ���õ� ���� �ε���

private: // 
	GameEngineFBXMesh* SelectMesh;						// ���缱�õ� Mesh

public:
	struct RenderTargetDrawData
	{
	public:
		std::string Name;
		GameEngineRenderTarget* Target;
		float4 Size_;
		bool ScaleCheck;
	};

public:
	GameEngineDirectory FBXFilePath;

public:
	std::vector<std::string> ActorsNames;
	std::vector<GameEngineActor*> Actors;

public:
	GameEngineFBXWindow();
	~GameEngineFBXWindow();

public:
	GameEngineFBXWindow(const GameEngineFBXWindow& _Other) = delete;
	GameEngineFBXWindow(GameEngineFBXWindow&& _Other) noexcept = delete;
	GameEngineFBXWindow& operator=(const GameEngineFBXWindow& _Other) = delete;
	GameEngineFBXWindow& operator=(GameEngineFBXWindow&& _Other) noexcept = delete;

protected:
	void OnGUI() override;

private:
	void FrameUpdateClear();

private:
	LoadInfoState GetCurFileLoadFlag(const std::string& _FileName);
	void SetCurFileLoadFlag(const std::string& _FileName, LoadInfoState _Index);

private:
	void CreateActorControl();


private:
	void ActorControl();

};

