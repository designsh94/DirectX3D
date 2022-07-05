#pragma once
#include "GameEngineGUI.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineFBXMeshManager.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>

// FBX File ���� �ε��������
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
		// ���ϸ� �ε�Ȱ��
		if (true == File &&
			false == Mesh && false == Animation && false == All)
		{
			return LoadState::FILE;
		}

		// ����, �޽����� �ε�Ȱ��
		if (true == File && true == Mesh
			&& false == Animation && false == All)
		{
			return LoadState::MESH;
		}

		// ����, �ִϸ��̼Ǳ��� �ε�� ���
		if (true == File && true == Animation
			&& false == Mesh && false == All)
		{
			return LoadState::ANIMATION;
		}

		// ����, �޽�, �ִϸ��̼Ǳ��� �ε�� ���
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

		// �ִϸ��̼ǵ� �������ִ� �����̸�, �ε尡 �Ϸ�Ǿ��ִ� �����϶�
		if (true == Animation)
		{
			AllLoadFinish();
		}
	}

	inline void AnimationLoadFinish()
	{
		Animation = true;

		// �޽��� �������ִ� �����̸�, �ε尡 �Ϸ�Ǿ��ִ� �����϶�
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
#pragma region FONT ����

private:
	ImFont* TextFontStyle_Eng;							// (���ĺ���) : arialbd.ttf(Arial ����)
	ImFont* LabelFontStyle_Eng;							// (���ĺ���) : arialbd.ttf(Arial ����)
	ImFont* ButtonFontStyle_Eng;						// (���ĺ���) : arialbd.ttf(Arial ����)

	ImFont* TextFontStyle_Kor;							// (�ѱۿ�) : malgunbd.ttf(������� ����)
	ImFont* LabelFontStyle_Kor;							// (�ѱۿ�) : malgunbd.ttf(������� ����)
	ImFont* ButtonFontStyle_Kor;						// (�ѱۿ�) : malgunbd.ttf(������� ����)

	ImGuiStyle* FontColor;								// Styel�� Color 0��° �ε����� �̿��Ͽ� �ؽ�Ʈ������ ����

#pragma endregion

private: // FBX File List
	std::vector<GameEngineFile> Files;					// ���� �����Ȱ���� FBX File Info
	std::vector<std::string> FileNames;					// ���� �����Ȱ���� FBX ���ϸ���

private: // FBXFile Infomation State
	std::map<std::string, LoadInfoState> FileState;		// FBX File �����ε� ����
														// ALL  : GREEN	-> ������� �ε�Ϸ�(���ͻ��� ����, �ִϸ��̼� ����)
														// FILE : WHITE	-> ���� �ε�Ϸ�( )
														// MESH : RED	-> �޽� �ε�Ϸ�( )
														// ANIM : BLUE	-> �ִϸ��̼� �ε�Ϸ�( )

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
	bool FBXFilePathCompare();
	void FrameUpdateClear();

private:
	void CreateActorControl();


private:
	void ActorControl();

};

