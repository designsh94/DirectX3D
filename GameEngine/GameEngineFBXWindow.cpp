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
	SelectMesh(nullptr)
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
	FrameUpdateClear();

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
	ImGui::Text("FBXFILE LIST");
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

		std::map<std::string, LoadInfoState>::iterator FindIter = FileState.find(FileNames[FBXFileSelect]);
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
	for (auto& FileName : FileNames)
	{
		// FBXFileName_Arr List �߰�
		FBXFileName_Arr.push_back(FileName.c_str());
	}

	// Create File FBX Select ComboBox
	if (true == ImGui::Combo("##FBXFILELIST", &FBXFileSelect, &FBXFileName_Arr[0], static_cast<int>(FBXFileName_Arr.size())))
	{
		// ������ ������ �����ϰ�,
		if (-1 != FBXFileSelect)
		{
			// ���� ���õ� ������ �޽� Get
			SelectMesh = GameEngineFBXMeshManager::GetInst().Find(FileNames[FBXFileSelect]);
			if (nullptr == SelectMesh)
			{
				// ���� ����
				// ��, �ش� ������ Mesh ������ �ε������̾��ٰ� �Ǵ��Ͽ� �ε��Ѵ�.
				SelectMesh = GameEngineFBXMeshManager::GetInst().Load(Files[FBXFileSelect].GetFullPath());
				SelectMesh->MeshLoad();
				if (0 != SelectMesh->GetMeshSet().size())
				{
					SelectMesh->CreateRenderingBuffer();

					// �ش� ������ ��� �޽������ε忡 �����Ͽ����Ƿ� �ش� ������ �ε��������¸� ����
					std::map<std::string, LoadInfoState>::iterator FindIter = FileState.find(FileNames[FBXFileSelect]);
					if (FileState.end() != FindIter)
					{
						(*FindIter).second.MeshLoadFinish();
					}
				}
				else
				{
					GameEngineDebug::MsgBox("�Ž������� �������� �ʴ� FBX�Դϴ�");
					GameEngineFBXMeshManager::GetInst().DeletePath(Files[FBXFileSelect].GetFullPath());
				}
			}
			else
			{
				// �ߺ�����
				GameEngineDebug::MsgBox("�̹� �ε��� �Ž������� �����մϴ�");
			}
		}
	}

#pragma endregion

#pragma region Actors

	// Mesh ������ �ε��� �����̶�� ���ͻ��� ����
	if (-1 != FBXFileSelect)
	{
		std::map<std::string, LoadInfoState>::iterator FindIter = FileState.find(FileNames[FBXFileSelect]);
		if (FileState.end() != FindIter)
		{
			// �޽������� �ε�Ϸ�� FBX File�϶� ���ͻ�������
			if ((*FindIter).second.GetLoadInfoState() == LoadState::MESH)
			{
				if (true == ImGui::Button("Create Actor"))
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









	}

#pragma endregion



#pragma region InfoLoadEnd List(�����ε�Ϸ�� ���)




#pragma endregion



#pragma region ActorList(������ ���͸��)

	// ������ ���͸��





#pragma endregion

	//Names.clear();
	//OriNames.clear();
	//ActorsNames.clear();

	//std::vector<GameEngineFile> Files = FBXFolder.GetAllFile("FBX");


	//if (0 == Files.size())
	//{
	//	ImGui::Text("FBX������ �������� �ʽ��ϴ�.");

	//	return;
	//}

	//std::vector<const char*> ActorArr;
	//for (GameEngineActor* Ref : Actors)
	//{
	//	std::string Name;

	//	if ("" == Ref->GetName())
	//	{
	//		Name = "None";
	//	}

	//	ActorsNames.push_back(Name);
	//	ActorArr.push_back(ActorsNames[ActorsNames.size() - 1].c_str());
	//}


	//std::vector<const char*> Arr;
	//for (auto& Ref : Files)
	//{
	//	std::string& Name = Names.emplace_back();

	//	Name = Ref.FileName();
	//	OriNames.push_back(Name);

	//	if (GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Find(Ref.FileName()))
	//	{
	//		Name += " M";
	//	}
	//	if (GameEngineFBXAnimation* Mesh = GameEngineFBXAnimationManager::GetInst().Find(Ref.FileName()))
	//	{
	//		Name += " A";
	//	}


	//	Arr.push_back(Names[Names.size() - 1].c_str());
	//}

	//ImGui::BeginChildFrame(static_cast<ImGuiID>(reinterpret_cast<uint64_t>("##FBXRANGE")), { 500, 500 });
	//ImGui::Text("FBXLIST");

	//ImGui::PushItemWidth(200);
	//ImGui::ListBox("##FBXLIST", &FBXFileSelect, &Arr[0], static_cast<ImGuiID>(Arr.size()), 20);

	//ImGui::SameLine();

	//ImGui::PushItemWidth(200);

	//const char** ActorNamePtr = nullptr;

	//if (Actors.size())
	//{
	//	ActorNamePtr = &ActorArr[0];
	//}

	//ImGui::ListBox("##ActorList", &ActorSelect, ActorNamePtr, static_cast<ImGuiID>(ActorArr.size()), 20);

	//ImGui::EndChildFrame();
	//ImGui::SameLine();

	//std::string info = "Ư�̻��� ";

	//static float Scroll = 0.0f;

	//GameEngineFBXMesh* SelectMesh = nullptr;
	//if (FBXFileSelect != -1)
	//{
	//	SelectMesh = GameEngineFBXMeshManager::GetInst().Find(OriNames[FBXFileSelect]);
	//}

	//GameEngineFBXAnimation* SelectAnimation = nullptr;
	//if (FBXFileSelect != -1)
	//{
	//	SelectAnimation = GameEngineFBXAnimationManager::GetInst().Find(OriNames[FBXFileSelect]);
	//}

	//ImGui::BeginChildFrame(static_cast<ImGuiID>(reinterpret_cast<uint64_t>("##NODETREERANGE")), { (Scroll * 3) + 300 , 500 }/*, ImGuiWindowFlags_HorizontalScrollbar*/);

	//if (nullptr != SelectMesh)
	//{
	//	bool Check = true;
	//	SelectMesh->RecursiveAllNode(
	//		[&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _ParentReturn)
	//		{
	//			int Result = 0;

	//			std::string TypeName = " ";

	//			switch (_Type)
	//			{
	//			case fbxsdk::FbxNodeAttribute::eUnknown:
	//				TypeName += "Unknown";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eNull:
	//				TypeName += "Null";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eMarker:
	//				TypeName += "Marker";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eSkeleton:
	//				TypeName += "Skeleton";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eMesh:
	//				TypeName += "Mesh";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eNurbs:
	//				TypeName += "Nurbs";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::ePatch:
	//				TypeName += "Patch";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eCamera:
	//				TypeName += "Camera";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eCameraStereo:
	//				TypeName += "CameraStereo";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
	//				TypeName += "CameraSwitcher";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eLight:
	//				TypeName += "Light";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eOpticalReference:
	//				TypeName += "OpticalReference";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eOpticalMarker:
	//				TypeName += "OpticalMarker";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eNurbsCurve:
	//				TypeName += "NurbsCurve";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
	//				TypeName += "TrimNurbsSurface";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eBoundary:
	//				TypeName += "Boundary";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eNurbsSurface:
	//				TypeName += "NurbsSurface";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eShape:
	//				TypeName += "Shape";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eLODGroup:
	//				TypeName += "LODGroup";
	//				info += "LODGroup";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eSubDiv:
	//				TypeName += "SubDiv";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eCachedEffect:
	//				TypeName += "CachedEffect";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eLine:
	//				TypeName += "Line";
	//				break;
	//			default:
	//				break;
	//			}

	//			if (1 == _ParentReturn)
	//			{
	//				std::string DisName = _Node->GetName() + TypeName;

	//				Result = ImGui::TreeNodeEx(DisName.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf);

	//				if (Scroll <= ImGui::GetScrollMaxX())
	//				{
	//					Scroll = ImGui::GetScrollMaxX();
	//				}

	//			}
	//			return Result;
	//		}
	//		,
	//			[&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _StartReturn)
	//		{
	//			if (_StartReturn == 1)
	//			{
	//				ImGui::TreePop();
	//			}
	//		},
	//			1
	//			);
	//}
	//else if (nullptr != SelectAnimation)
	//{
	//	bool Check = true;
	//	SelectAnimation->RecursiveAllNode(
	//		[&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _ParentReturn)
	//		{
	//			int Result = 0;

	//			std::string TypeName = " ";

	//			switch (_Type)
	//			{
	//			case fbxsdk::FbxNodeAttribute::eUnknown:
	//				TypeName += "Unknown";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eNull:
	//				TypeName += "Null";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eMarker:
	//				TypeName += "Marker";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eSkeleton:
	//				TypeName += "Skeleton";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eMesh:
	//				TypeName += "Mesh";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eNurbs:
	//				TypeName += "Nurbs";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::ePatch:
	//				TypeName += "Patch";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eCamera:
	//				TypeName += "Camera";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eCameraStereo:
	//				TypeName += "CameraStereo";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
	//				TypeName += "CameraSwitcher";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eLight:
	//				TypeName += "Light";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eOpticalReference:
	//				TypeName += "OpticalReference";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eOpticalMarker:
	//				TypeName += "OpticalMarker";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eNurbsCurve:
	//				TypeName += "NurbsCurve";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
	//				TypeName += "TrimNurbsSurface";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eBoundary:
	//				TypeName += "Boundary";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eNurbsSurface:
	//				TypeName += "NurbsSurface";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eShape:
	//				TypeName += "Shape";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eLODGroup:
	//				TypeName += "LODGroup";
	//				info += "LODGroup";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eSubDiv:
	//				TypeName += "SubDiv";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eCachedEffect:
	//				TypeName += "CachedEffect";
	//				break;
	//			case fbxsdk::FbxNodeAttribute::eLine:
	//				TypeName += "Line";
	//				break;
	//			default:
	//				break;
	//			}

	//			if (1 == _ParentReturn)
	//			{
	//				std::string DisName = _Node->GetName() + TypeName;

	//				Result = ImGui::TreeNodeEx(DisName.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf);

	//				if (Scroll <= ImGui::GetScrollMaxX())
	//				{
	//					Scroll = ImGui::GetScrollMaxX();
	//				}

	//			}
	//			return Result;
	//		}
	//		,
	//			[&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _StartReturn)
	//		{
	//			if (_StartReturn == 1)
	//			{
	//				ImGui::TreePop();
	//			}
	//		},
	//			1
	//			);
	//}

	//ImGui::EndChildFrame();

	//if (nullptr == SelectMesh)
	//{
	//	if (ImGui::Button("MeshLoad"))
	//	{
	//		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(Files[FBXFileSelect].GetFullPath());
	//		Mesh->MeshLoad();

	//		if (0 == Mesh->GetMeshSet().size())
	//		{
	//			GameEngineDebug::MsgBox("�Ž������� �������� �ʴ� FBX�Դϴ�");
	//			GameEngineFBXMeshManager::GetInst().DeletePath(Files[FBXFileSelect].GetFullPath());
	//		}
	//		else
	//		{
	//			Mesh->CreateRenderingBuffer();
	//		}
	//	}
	//}

	//if (nullptr == SelectAnimation)
	//{
	//	if (ImGui::Button("AnimationLoad"))
	//	{
	//		GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(Files[FBXFileSelect].GetFullPath());

	//		if (false == Animation->LoadAnimation())
	//		{
	//			GameEngineDebug::MsgBox("�ִϸ��̼��� �������� �ʴ� FBX�Դϴ�");
	//			GameEngineFBXAnimationManager::GetInst().DeletePath(Files[FBXFileSelect].GetFullPath());
	//		}
	//		else {

	//		}
	//	}
	//}

	//if (nullptr != SelectMesh
	//	&& 0 != SelectMesh->GetMeshSet().size()
	//	&& ImGui::Button("Actor Create"))
	//{
	//	if (0 == SelectMesh->GetMeshSet().size())
	//	{
	//		SelectMesh->MeshLoad();
	//		SelectMesh->CreateRenderingBuffer();
	//	}

	//	if (0 != SelectMesh->GetMeshSet().size())
	//	{
	//		GameEngineActor* NewActor = GameEngineCore::CurrentLevel()->CreateActor<GameEngineActor>();
	//		Actors.push_back(NewActor);

	//		GameEngineFBXRenderer* Renderer = NewActor->CreateTransformComponent<GameEngineFBXRenderer>(NewActor->GetTransform());

	//		Renderer->SetFBXMesh(SelectMesh->GetName(), "Color");

	//		for (int i = 0; i < Renderer->GetRenderSetCount(); i++)
	//		{
	//			Renderer->GetRenderSet(i).ShaderHelper->SettingConstantBufferSet("ResultColor", float4::RED);
	//		}
	//	}
	//	else
	//	{
	//		GameEngineDebug::MsgBox("�Ž���� �������� �ʴ� FBX �Դϴ�.");
	//	}
	//}

	//ImGui::Text(GameEngineString::AnsiToUTF8Return(info).c_str());

	//ActorControl();
}

bool GameEngineFBXWindow::FBXFilePathCompare()
{
	// ������ ����� ���Ϻ�ȭ�� �ִٸ� ���ϸ�ϰ���
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
	if (FileNames.size() == NewFiles.size() && true == std::equal(FileNames.begin(), FileNames.end(), NewFiles.begin()))
	{
		// ���Ͽ� ������ ����̸� false ��ȯ
		return false;
	}

	// �ƴϸ� ��ȭ�����Ƿ� �����̸���� ���� �� ���ϸ�� �ε尡 �������Ƿ� ���� �ε�������� ����
	FileNames.clear();
	FileState.clear();
	for (auto& FileName : Files)
	{
		// ���ϸ� ��� �ۼ�
		FileNames.push_back(FileName.GetFileName());

		// ���Ϻ� �����ε� �������� �ۼ�
		FileState.insert(std::make_pair(FileName.GetFileName(), LoadInfoState(LoadState::FILE)));
	}

	return true;
}

void GameEngineFBXWindow::FrameUpdateClear()
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
}

void GameEngineFBXWindow::CreateActorControl()
{
}

void GameEngineFBXWindow::ActorControl()
{
	if (ActorSelect == -1)
	{
		return;
	}


	GameEngineActor* Actor = Actors[ActorSelect];

	if (nullptr == Actor)
	{
		return;
	}

	float4 Position = Actor->GetTransform()->GetWorldPosition();
	float4 Rotation = Actor->GetTransform()->GetWorldRotation();
	float4 Scaling = Actor->GetTransform()->GetWorldScaling();

	ImGui::DragFloat3("POSITION", Position.Arr1D, 1.0f, -FLT_MAX, FLT_MAX);
	ImGui::SameLine();
	ImGui::PushID("RESETPOSITION");
	if (ImGui::Button("RESET"))
	{
		Position = float4::ZERO;
	}
	ImGui::PopID();

	ImGui::DragFloat3("SCALE", Scaling.Arr1D, 1.0f, -FLT_MAX, FLT_MAX);
	ImGui::SameLine();
	ImGui::PushID("RESETSCALE");
	if (ImGui::Button("RESET"))
	{
		Scaling = float4::ONE;
	}
	ImGui::PopID();

	ImGui::DragFloat3("ROTATION", Rotation.Arr1D, 1.0f, -FLT_MAX, FLT_MAX);
	ImGui::SameLine();
	ImGui::PushID("RESETROTATION");
	if (ImGui::Button("RESET"))
	{
		Rotation = float4::ZERO;
	}
	ImGui::PopID();


	Actor->GetTransform()->SetWorldPosition(Position);
	Actor->GetTransform()->SetWorldRotationDegree(Rotation);
	Actor->GetTransform()->SetWorldScaling(Scaling);

	if (true == ImGui::Button("Death"))
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

