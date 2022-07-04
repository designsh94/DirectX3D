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
private: // Font 관련
	ImFont* TextFontStyle_Eng;							// (알파벳용) : 24pt, arialbd.ttf(Arial 굵게)
	ImFont* LabelFontStyle_Eng;							// (알파벳용) : 18pt, arialbd.ttf(Arial 굵게)
	ImFont* ButtonFontStyle_Eng;						// (알파벳용) : 10pt, arialbd.ttf(Arial 굵게)

	ImFont* TextFontStyle_Kor;							// (한글용) : 24pt, malgunbd.ttf(맑은고딕 굵게)
	ImFont* LabelFontStyle_Kor;							// (한글용) : 18pt, malgunbd.ttf(맑은고딕 굵게)
	ImFont* ButtonFontStyle_Kor;						// (한글용) : 10pt, malgunbd.ttf(맑은고딕 굵게)

	ImGuiStyle* FontColor;								// Styel의 Color 0번째 인덱스를 이용하여 텍스트색상을 변경

private: // Vector List
	std::vector<std::string> FileNames_Origin;			// 탐색용 파일명 목록(원본)
	std::vector<std::string> FileNames;					// 편집용 파일명 목록

private: // ListBox Select Index
	int FBXFileSelect;									// 현재목록의 선택된 파일 인덱스
	int ActorSelect;									// 현재목록의 선택된 액터 인덱스

private: // 
	GameEngineFBXMesh* SelectMesh;						// 현재선택된 Mesh

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

