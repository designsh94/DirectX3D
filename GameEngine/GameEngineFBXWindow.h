#pragma once
#include "GameEngineGUI.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineFBXMeshManager.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>

// FBX File 정보 로드상태정보
enum class LoadState
{
	NONE = -1,
	FILE,
	MESH,
	ANIMATION,
	ALL
};

struct LoadInfoState
{
private:
	bool File;
	bool Mesh;
	bool Animation;
	bool All;

public:
	LoadInfoState() :
		File(false), Mesh(false), Animation(false), All(false)
	{
	}

	LoadInfoState(LoadState _LoadState) :
		File(true), Mesh(false), Animation(false), All(false)
	{

	}

public:
	inline LoadState GetLoadInfoState()
	{
		// 파일만 로드된경우
		if (true == File &&
			false == Mesh && false == Animation && false == All)
		{
			return LoadState::FILE;
		}

		// 파일, 메쉬까지 로드된경우
		if (true == File && true == Mesh
			&& false == Animation && false == All)
		{
			return LoadState::MESH;
		}

		// 파일, 애니메이션까지 로드된 경우
		if (true == File && true == Animation
			&& false == Mesh && false == All)
		{
			return LoadState::ANIMATION;
		}

		// 파일, 메쉬, 애니메이션까지 로드된 경우
		if (true == File && true == Mesh && true == Animation && true == All)
		{
			return LoadState::ALL;
		}

		return LoadState::NONE;
	}

public:
	inline void FileLoadFinish()
	{
		File = true;
	}

	inline void MeshLoadFinish()
	{
		Mesh = true;

		// 애니메이션도 가지고있는 파일이며, 로드가 완료되어있는 상태일때
		if (true == Animation)
		{
			AllLoadFinish();
		}
	}

	inline void AnimationLoadFinish()
	{
		Animation = true;

		// 메쉬도 가지고있는 파일이며, 로드가 완료되어있는 상태일때
		if (true == Mesh)
		{
			AllLoadFinish();
		}
	}

	inline void AllLoadFinish()
	{
		All = true;
	}
};

class GameEngineFBXWindow : public GameEngineGUIWindow
{
#pragma region FONT 관련

private:
	ImFont* TextFontStyle_Eng;							// (알파벳용) : arialbd.ttf(Arial 굵게)
	ImFont* LabelFontStyle_Eng;							// (알파벳용) : arialbd.ttf(Arial 굵게)
	ImFont* ButtonFontStyle_Eng;						// (알파벳용) : arialbd.ttf(Arial 굵게)

	ImFont* TextFontStyle_Kor;							// (한글용) : malgunbd.ttf(맑은고딕 굵게)
	ImFont* LabelFontStyle_Kor;							// (한글용) : malgunbd.ttf(맑은고딕 굵게)
	ImFont* ButtonFontStyle_Kor;						// (한글용) : malgunbd.ttf(맑은고딕 굵게)

	ImGuiStyle* FontColor;								// Styel의 Color 0번째 인덱스를 이용하여 텍스트색상을 변경

#pragma endregion

private: // FBX File List
	std::vector<GameEngineFile> Files;					// 현재 지정된경로의 FBX File Info
	std::vector<std::string> FileNames;					// 현재 지정된경로의 FBX 파일명목록

private: // FBXFile Infomation State
	std::map<std::string, LoadInfoState> FileState;		// FBX File 정보로드 상태
														// ALL  : GREEN	-> 모든정보 로드완료(액터생성 가능, 애니메이션 실행)
														// FILE : WHITE	-> 파일 로드완료( )
														// MESH : RED	-> 메쉬 로드완료( )
														// ANIM : BLUE	-> 애니메이션 로드완료( )

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
	bool FBXFilePathCompare();
	void FrameUpdateClear();

private:
	void CreateActorControl();


private:
	void ActorControl();

};

