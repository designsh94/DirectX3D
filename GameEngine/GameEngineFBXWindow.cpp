#include "PreCompile.h"
#include "GameEngineFBXWindow.h"
#include "GameEngineImageShotWindow.h"
#include "GameEngineFBXMeshManager.h"
#include "GameEngineFBXAnimationManager.h"
#include "GameEngineCore.h"

#include "Enums.h"

GameEngineFBXWindow::GameEngineFBXWindow() :
	TextFontStyle_Eng(nullptr),
	LabelFontStyle_Eng(nullptr),
	ButtonFontStyle_Eng(nullptr),
	FBXFileSelect(-1),
	ActorSelect(-1),
	SelectMesh(nullptr),
	SelectAnimation(nullptr),
	SelectActor(nullptr),
	CurActor_Position(float4::ZERO),
	CurActor_Rotation(float4::ZERO),
	CurActor_Scaling(float4::ONE)
{
	// ������ �ش� �����쿡�� ����Ϸ��� Font�� �������Ѵ�.
	ImGuiIO& IO = ImGui::GetIO();

	// C++ ���Ϲ��� '\'�� ���������ʴ´� �׷��Ƿ� '\\' �� ��/���� ���͸��� �����Ͽ��߸� AddFontFromFileTTF�� �ش� ������ �ε��Ҽ��ִ�.
	// English
	TextFontStyle_Eng = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 20.f);
	ButtonFontStyle_Eng = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 12.f);
	LabelFontStyle_Eng = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 16.f);

	// Korean
	TextFontStyle_Kor = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgunbd.ttf", 20.f);
	ButtonFontStyle_Kor = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgunbd.ttf", 18.f);
	LabelFontStyle_Kor = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgunbd.ttf", 10.f);

	// ������ �ش� �����쿡�� ����Ϸ��� Color�� �������Ѵ�.
	// imgui style���� Color�� 0��° �ε����� �ؽ�Ʈ�� ������ �����Ѵ�.
	FontColor = &ImGui::GetStyle();
	FontColor->Colors[ImGuiCol_::ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

GameEngineFBXWindow::~GameEngineFBXWindow()
{
}

void GameEngineFBXWindow::OnGUI()
{
	// �����ؾ��ϴ� ��� Ŭ����
	FBXRelatedListUpdate();

#pragma region Distinction(�����÷� ����)
	ImGui::PushFont(LabelFontStyle_Eng);

	// Font Color�� Ư�� ����� Ÿ���� �����Ѵ�.
	ImGui::Text("FBXFile Infomation State Color : ");

	// All Load End(�޽��� �ִϸ��̼��� ��� �ε��� ���� : ���ͻ�������) - GREEN
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	ImGui::Text("ALL");
	ImGui::PopStyleColor();

	ImGui::SameLine();

	// First Load FBX File - WHILE
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::Text("FILE");
	ImGui::PopStyleColor();

	ImGui::SameLine();

	// Mesh Load End FBX File(�޽��� �ε��� ���� : ���ͻ�������) - RED
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::Text("MESH");
	ImGui::PopStyleColor();

	ImGui::SameLine();

	// Animation Load End FBX File(�ִϸ��̼��� �ε��� ����) - BLUE
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	ImGui::Text("ANIMATION");
	ImGui::PopStyleColor();

	// �ش� Font Style ����
	ImGui::PopFont();

#pragma endregion

	// ���� ���� ����
	ImGui::Text("");

#pragma region FBX File ComboBox

	// �������Ʈ ���
	ImGui::PushFont(TextFontStyle_Eng);
	ImGui::Text("ALL FBXFILE LIST");
	ImGui::PopFont();

	// FBX File Load State
	// ��, ���õ� ������ �־������ ���°� ǥ�õȴ�.
	if (-1 != FBXFileSelect)
	{
		ImGui::SameLine();

		ImGui::PushFont(TextFontStyle_Eng);
		ImGui::Text("State : ");
		ImGui::PopFont();

		ImGui::SameLine();
		
		// ���°� ������
		ImVec4 StateColor = ImVec4(0.f, 0.f, 0.f, 0.f);

		std::map<std::string, LoadInfoState>::iterator FindIter = FileState.find(AllFileNames[FBXFileSelect]);
		if (FileState.end() != FindIter)
		{
			switch ((*FindIter).second.GetLoadInfoState())
			{
				case LoadState::FILE:
				{
					StateColor = ImVec4(1.f, 1.f, 1.f, 1.f);
					break;
				}
				case LoadState::MESH:
				{
					StateColor = ImVec4(1.f, 0.f, 0.f, 1.f);
					break;
				}
				case LoadState::ANIMATION:
				{
					StateColor = ImVec4(0.f, 0.f, 1.f, 1.f);
					break;
				}
				case LoadState::ALL:
				{
					StateColor = ImVec4(0.f, 1.f, 0.f, 1.f);
					break;
				}
			}
		}

		const ImVec2 posR = ImGui::GetCursorScreenPos();
		ImDrawList* DrawList = ImGui::GetWindowDrawList();
		DrawList->AddRectFilled(posR, ImVec2(posR.x + 20.f, posR.y + 20.f), ImGui::GetColorU32(StateColor));

		// \n
		ImGui::Text("");
	}
	
	// FBX File ComboBox �ۼ��غ�
	std::vector<const char*> FBXFileName_Arr;
	for (auto& FileName : AllFileNames)
	{
		// FBXFileName_Arr List �߰�
		FBXFileName_Arr.push_back(FileName.c_str());
	}

	// Create File FBX Select ComboBox
	ImGui::PushItemWidth(500.f);
	if (true == ImGui::Combo("##FBXFILELIST", &FBXFileSelect, &FBXFileName_Arr[0], static_cast<int>(FBXFileName_Arr.size())))
	{
		// ������ ������ �����ϰ�,
		if (-1 != FBXFileSelect)
		{
			// ���� ���õ� ������ �޽� Get
			SelectMesh = GameEngineFBXMeshManager::GetInst().Find(AllFileNames[FBXFileSelect]);
			if (nullptr == SelectMesh)
			{
				// ���� ����
				// ��, �ش� ������ Mesh ������ �ε������̾��ٰ� �Ǵ��Ͽ� �ε��Ѵ�.
				SelectMesh = GameEngineFBXMeshManager::GetInst().Load(Files[FBXFileSelect].GetFullPath());
				//SelectMesh->MeshLoad();
				if (0 != SelectMesh->GetMeshSet().size())
				{
					SelectMesh->CreateRenderingBuffer();

					// �ش� ������ ��� �޽������ε忡 �����Ͽ����Ƿ� �ش� ������ �ε��������¸� ����
					std::map<std::string, LoadInfoState>::iterator FindIter = FileState.find(AllFileNames[FBXFileSelect]);
					if (FileState.end() != FindIter)
					{
						(*FindIter).second.MeshLoadFinish();
					}
				}
				else
				{
					GameEngineDebug::MsgBox("�Ž������� �������� �ʴ� FBX�Դϴ�");
					GameEngineFBXMeshManager::GetInst().DeletePath(Files[FBXFileSelect].GetFullPath());
					SelectMesh = nullptr;

					// ���� Animation�� �������� ������ �����̹Ƿ� Animation File List�� �ش� ������ �߰�




				}
			}
			else
			{
				// �ߺ�����
				GameEngineDebug::MsgBox("�̹� �ε��� �Ž������� �����մϴ�");
			}
		}
	}
	ImGui::PopItemWidth();

	// ���� ���� ������ �޽����� ��� �ۼ�
	if (-1 != FBXFileSelect && nullptr != SelectMesh)
	{
		ImGui::BeginChildFrame(static_cast<ImGuiID>(reinterpret_cast<uint64_t>("##MESHINFO_FRAME")), ImVec2(500.f, 150.f), ImGuiWindowFlags_HorizontalScrollbar);

		SelectMesh->RecursiveAllNode([&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _ParentReturn)
			{
				int Result = 0;
				std::string TypeName = " ";

				switch (_Type)
				{
				case fbxsdk::FbxNodeAttribute::eUnknown:
					TypeName += "Unknown";
					break;
				case fbxsdk::FbxNodeAttribute::eNull:
					TypeName += "Null";
					break;
				case fbxsdk::FbxNodeAttribute::eMarker:
					TypeName += "Marker";
					break;
				case fbxsdk::FbxNodeAttribute::eSkeleton:
					TypeName += "Skeleton";
					break;
				case fbxsdk::FbxNodeAttribute::eMesh:
					TypeName += "Mesh";
					break;
				case fbxsdk::FbxNodeAttribute::eNurbs:
					TypeName += "Nurbs";
					break;
				case fbxsdk::FbxNodeAttribute::ePatch:
					TypeName += "Patch";
					break;
				case fbxsdk::FbxNodeAttribute::eCamera:
					TypeName += "Camera";
					break;
				case fbxsdk::FbxNodeAttribute::eCameraStereo:
					TypeName += "CameraStereo";
					break;
				case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
					TypeName += "CameraSwitcher";
					break;
				case fbxsdk::FbxNodeAttribute::eLight:
					TypeName += "Light";
					break;
				case fbxsdk::FbxNodeAttribute::eOpticalReference:
					TypeName += "OpticalReference";
					break;
				case fbxsdk::FbxNodeAttribute::eOpticalMarker:
					TypeName += "OpticalMarker";
					break;
				case fbxsdk::FbxNodeAttribute::eNurbsCurve:
					TypeName += "NurbsCurve";
					break;
				case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
					TypeName += "TrimNurbsSurface";
					break;
				case fbxsdk::FbxNodeAttribute::eBoundary:
					TypeName += "Boundary";
					break;
				case fbxsdk::FbxNodeAttribute::eNurbsSurface:
					TypeName += "NurbsSurface";
					break;
				case fbxsdk::FbxNodeAttribute::eShape:
					TypeName += "Shape";
					break;
				case fbxsdk::FbxNodeAttribute::eLODGroup:
					TypeName += "LODGroup";
					break;
				case fbxsdk::FbxNodeAttribute::eSubDiv:
					TypeName += "SubDiv";
					break;
				case fbxsdk::FbxNodeAttribute::eCachedEffect:
					TypeName += "CachedEffect";
					break;
				case fbxsdk::FbxNodeAttribute::eLine:
					TypeName += "Line";
					break;
				default:
					break;
				}

				if (1 == _ParentReturn)
				{
					std::string DisName = _Node->GetName() + TypeName;

					Result = ImGui::TreeNodeEx(DisName.c_str());
				}
				return Result;
			}
			, [&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _StartReturn)
			{
				if (_StartReturn == 1)
				{
					ImGui::TreePop();
				}
			}, 1);

		ImGui::EndChildFrame();
	}

#pragma endregion

#pragma region Animation Load
	if (-1 != FBXFileSelect)
	{
		if (ImGui::Button("AnimationLoad", ImVec2(500.f, 25.f)))
		{
			SelectAnimation = GameEngineFBXAnimationManager::GetInst().Find(AllFileNames[FBXFileSelect]);
			if (nullptr == SelectAnimation)
			{
				SelectAnimation = GameEngineFBXAnimationManager::GetInst().Load(Files[FBXFileSelect].GetFullPath());
				if (false == SelectAnimation->LoadAnimation())
				{
					GameEngineDebug::MsgBox("�ִϸ��̼��� �������� �ʴ� FBX�Դϴ�");
					GameEngineFBXAnimationManager::GetInst().DeletePath(Files[FBXFileSelect].GetFullPath());
				}
				else
				{
					// �ִϸ��̼� �ε尡�Ϸ� �Ǿ����Ƿ� �����ε� ���¸� ����
					std::map<std::string, LoadInfoState>::iterator FindIter = FileState.find(AllFileNames[FBXFileSelect]);
					if (FileState.end() != FindIter)
					{
						(*FindIter).second.AnimationLoadFinish();
					}
				}
			}
		}
	}

	// ���� ���� ������ �ִϸ��̼����� ��� �ۼ�
	if (-1 != FBXFileSelect && nullptr != SelectAnimation)
	{
		ImGui::BeginChildFrame(static_cast<ImGuiID>(reinterpret_cast<uint64_t>("##ANIMATIONINFO_FRAME")), ImVec2(500.f, 150.f), ImGuiWindowFlags_HorizontalScrollbar);

		SelectAnimation->RecursiveAllNode([&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _ParentReturn)
			{
				int Result = 0;

				std::string TypeName = " ";

				switch (_Type)
				{
				case fbxsdk::FbxNodeAttribute::eUnknown:
					TypeName += "Unknown";
					break;
				case fbxsdk::FbxNodeAttribute::eNull:
					TypeName += "Null";
					break;
				case fbxsdk::FbxNodeAttribute::eMarker:
					TypeName += "Marker";
					break;
				case fbxsdk::FbxNodeAttribute::eSkeleton:
					TypeName += "Skeleton";
					break;
				case fbxsdk::FbxNodeAttribute::eMesh:
					TypeName += "Mesh";
					break;
				case fbxsdk::FbxNodeAttribute::eNurbs:
					TypeName += "Nurbs";
					break;
				case fbxsdk::FbxNodeAttribute::ePatch:
					TypeName += "Patch";
					break;
				case fbxsdk::FbxNodeAttribute::eCamera:
					TypeName += "Camera";
					break;
				case fbxsdk::FbxNodeAttribute::eCameraStereo:
					TypeName += "CameraStereo";
					break;
				case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
					TypeName += "CameraSwitcher";
					break;
				case fbxsdk::FbxNodeAttribute::eLight:
					TypeName += "Light";
					break;
				case fbxsdk::FbxNodeAttribute::eOpticalReference:
					TypeName += "OpticalReference";
					break;
				case fbxsdk::FbxNodeAttribute::eOpticalMarker:
					TypeName += "OpticalMarker";
					break;
				case fbxsdk::FbxNodeAttribute::eNurbsCurve:
					TypeName += "NurbsCurve";
					break;
				case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
					TypeName += "TrimNurbsSurface";
					break;
				case fbxsdk::FbxNodeAttribute::eBoundary:
					TypeName += "Boundary";
					break;
				case fbxsdk::FbxNodeAttribute::eNurbsSurface:
					TypeName += "NurbsSurface";
					break;
				case fbxsdk::FbxNodeAttribute::eShape:
					TypeName += "Shape";
					break;
				case fbxsdk::FbxNodeAttribute::eLODGroup:
					TypeName += "LODGroup";
					break;
				case fbxsdk::FbxNodeAttribute::eSubDiv:
					TypeName += "SubDiv";
					break;
				case fbxsdk::FbxNodeAttribute::eCachedEffect:
					TypeName += "CachedEffect";
					break;
				case fbxsdk::FbxNodeAttribute::eLine:
					TypeName += "Line";
					break;
				default:
					break;
				}

				if (1 == _ParentReturn)
				{
					std::string DisName = _Node->GetName() + TypeName;

					Result = ImGui::TreeNodeEx(DisName.c_str());
				}
				return Result;
			}, [&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _StartReturn)
			{
				if (_StartReturn == 1)
				{
					ImGui::TreePop();
				}
			}, 1);

		ImGui::EndChildFrame();
	}

#pragma endregion

#pragma region Actors

	// FBX File�� ������������ ���ͻ��� �� ������ ���͸�� ǥ�� ��� Ȱ��ȭ
	if (-1 != FBXFileSelect)
	{
		// Mesh ������ �ε��� �����̶�� ���ͻ��� ����(��ưȰ��ȭ)
		std::map<std::string, LoadInfoState>::iterator FindIter = FileState.find(AllFileNames[FBXFileSelect]);
		if (FileState.end() != FindIter)
		{
			// �޽������� �ε�Ϸ�� FBX File�϶� ���ͻ�������
			if ((*FindIter).second.GetLoadInfoState() == LoadState::MESH ||
				(*FindIter).second.GetLoadInfoState() == LoadState::ALL)
			{
				if (true == ImGui::Button("Create Actor", ImVec2(500.f, 25.f)))
				{
					// ���� ����
					if (0 != SelectMesh->GetMeshSet().size())
					{
						GameEngineActor* NewActor = GameEngineCore::CurrentLevel()->CreateActor<GameEngineActor>();
						Actors.push_back(NewActor);

						GameEngineFBXRenderer* Renderer = NewActor->CreateTransformComponent<GameEngineFBXRenderer>(NewActor->GetTransform());
						Renderer->SetFBXMesh(SelectMesh->GetName(), "Color");
						for (int i = 0; i < Renderer->GetRenderSetCount(); i++)
						{
							Renderer->GetRenderSet(i).ShaderHelper->SettingConstantBufferSet("ResultColor", float4::RED);
						}
					}
					else
					{
						GameEngineDebug::MsgBox("�Ž���� �������� �ʴ� FBX �Դϴ�.");
					}
				}
			}
		}

		// ������ ���͸�� �ۼ�
		// ��, ������ ���Ͱ� �Ѱ��� �����Ҷ� ����Ʈ Ȱ��ȭ
		if (0 != Actors.size())
		{
			std::vector<const char*> ActorName_Arr;
			for (GameEngineActor* Ref : Actors)
			{
				std::string Name;

				if ("" == Ref->GetName())
				{
					Name = "None";
				}

				ActorsNames.push_back(Name);
				ActorName_Arr.push_back(ActorsNames[ActorsNames.size() - 1].c_str());
			}

			// ���� ������ Actor List
			ImGui::BeginChildFrame(static_cast<ImGuiID>(reinterpret_cast<uint64_t>("##ACTORFRAME")), ImVec2(500.f, 150.f));

			ImGui::PushItemWidth(496.f);
			if (true == ImGui::ListBox("##ActorList", &ActorSelect, &ActorName_Arr[0], static_cast<int>(ActorName_Arr.size()), 6))
			{
			}
			ImGui::PopItemWidth();

			ImGui::EndChildFrame();
		}
	}

	// ���õ� ���Ͱ� �����Ҷ�
	if (-1 != ActorSelect)
	{
		ActorController();
	}

#pragma endregion

}

bool GameEngineFBXWindow::FBXFilePathCompare()
{
	// ������ ����� ���Ϻ�ȭ�� �ִٸ� ���ϸ�ϰ���

	// ���� ������ ����� ���ϸ���� Get
	Files.clear();
	Files = FBXFilePath.GetAllFile("FBX");
	if (0 == Files.size())
	{
		std::string CurPath = FBXFilePath.GetFullPath();
		GameEngineDebug::MsgBoxError(CurPath + " : " "��ο� ������ ���������ʽ��ϴ�.");
		return false;
	}

	std::vector<std::string> NewFiles;
	for (auto& NewFileName : Files)
	{
		NewFiles.push_back(NewFileName.GetFileName());
	}

	// ���� �����Ϳ� ���Ե� ���ϸ�ϰ� ��ó��
	if (AllFileNames.size() == NewFiles.size() && true == std::equal(AllFileNames.begin(), AllFileNames.end(), NewFiles.begin()))
	{
		// ���Ͽ� ������ ����̸� false ��ȯ
		return false;
	}

	// �ƴϸ� ��ȭ�����Ƿ� �����̸���� ���� �� ���ϸ�� �ε尡 �������Ƿ� ���� �ε�������� ����
	AllFileNames.clear();
	FileState.clear();
	for (auto& FileName : Files)
	{
		// ��ü ���ϸ� ��� �ۼ�
		AllFileNames.push_back(FileName.GetFileName());

		// ���Ϻ� �����ε� �������� �ۼ�
		FileState.insert(std::make_pair(FileName.GetFileName(), LoadInfoState(LoadState::FILE)));
	}

	return true;
}

void GameEngineFBXWindow::FBXRelatedListUpdate()
{
	// FBX FILE PATH �˻�
	// ��, ��ȭ�� �ִٸ� ���ϸ���� �ٲ��.
	if (true == FBXFilePathCompare())
	{
		// ������ ��ο� ������ ��ȭ�� �Ͼ���Ƿ� 
		// ���� ���õ� �޽��� FBX File Index�� �ʱ�ȭ

		// Select Current Mesh Clear
		SelectMesh = nullptr;

		// Select FBXFile Current Index Clear
		FBXFileSelect = -1;
	}

	// ������ ���� ��� Ŭ����
	ActorsNames.clear();

	// 
}

void GameEngineFBXWindow::ActorController()
{
	GameEngineActor* Actor = Actors[ActorSelect];
	if (nullptr == Actor)
	{
		return;
	}

	// Current Actor World Value Get
	CurActor_Position = Actor->GetTransform()->GetWorldPosition();
	CurActor_Rotation = Actor->GetTransform()->GetWorldRotation();
	CurActor_Scaling = Actor->GetTransform()->GetWorldScaling();

	// POSITION
	//ImGui::Text("POSITION  : ");
	//ImGui::SameLine();

	//ImGui::PushID("RESETPOSITION");
	//if (true == ImGui::Button("RESET"))
	//{
	//	CurActor_Position = float4::ZERO;
	//}
	//ImGui::PopID();

	// X
	//ImGui::Text("X : ");
	//ImGui::SameLine();

	//ImGui::PushItemWidth(100.f);
	//ImGui::SliderFloat("POSX", &CurActor_Position.x, -FLT_MAX + 0.1f, FLT_MAX - 0.1f, "%.3f", 1.0f);
	//ImGui::PopItemWidth();
	//ImGui::SameLine();
	

	//ImGui::InputFloat("##POSITION_X", &CurActor_Position.x);

	// Y

	// Z

	

	// SCALE
	//ImGui::Text("SCALE       : ");





	// ROTATION
	//ImGui::Text("ROTATION : ");







	ImGui::DragFloat3("POSITION", CurActor_Position.Arr1D, 1.0f, -FLT_MAX, FLT_MAX);
	ImGui::SameLine();
	ImGui::PushID("RESETPOSITION");
	if (ImGui::Button("RESET"))
	{
		CurActor_Position = float4::ZERO;
	}
	ImGui::PopID();

	ImGui::DragFloat3("SCALE", CurActor_Scaling.Arr1D, 1.0f, -FLT_MAX, FLT_MAX);
	ImGui::SameLine();
	ImGui::PushID("RESETSCALE");
	if (ImGui::Button("RESET"))
	{
		CurActor_Scaling = float4::ONE;
	}
	ImGui::PopID();

	ImGui::DragFloat3("ROTATION", CurActor_Rotation.Arr1D, 1.0f, -FLT_MAX, FLT_MAX);
	ImGui::SameLine();
	ImGui::PushID("RESETROTATION");
	if (ImGui::Button("RESET"))
	{
		CurActor_Rotation = float4::ZERO;
	}
	ImGui::PopID();

	// ������� ��� ����
	Actor->GetTransform()->SetWorldPosition(CurActor_Position);
	Actor->GetTransform()->SetWorldRotationDegree(CurActor_Rotation);
	Actor->GetTransform()->SetWorldScaling(CurActor_Scaling);

	// Death Button
	if (true == ImGui::Button("Death", ImVec2(500.f, 25.f)))
	{
		std::vector<GameEngineActor*>::iterator Iter = Actors.begin();
		for (size_t i = 0; i < ActorSelect; i++)
		{
			++Iter;
		}

		if (*Iter == Actor)
		{
			Actor->Death();
			Actors.erase(Iter);
		}

		ActorSelect = -1;
	}
}
