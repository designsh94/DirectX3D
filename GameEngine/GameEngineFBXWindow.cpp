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
	ActorSelect(-1)
{
	// 생성시 해당 윈도우에서 사용하려는 Font를 재정의한다.
	ImGuiIO& IO = ImGui::GetIO();

	// C++ 단일문자 '\'을 지원하지않는다 그러므로 '\\' 로 상/하위 디렉터리를 구분하여야만 AddFontFromFileTTF로 해당 파일을 로드할수있다.
	// English
	TextFontStyle_Eng = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 20.f);
	ButtonFontStyle_Eng = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 18.f);
	LabelFontStyle_Eng = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 16.f);

	// Korean
	TextFontStyle_Kor = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgunbd.ttf", 20.f);
	ButtonFontStyle_Kor = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgunbd.ttf", 18.f);
	LabelFontStyle_Kor = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgunbd.ttf", 10.f);

	// 생성시 해당 윈도우에서 사용하려는 Color를 재정의한다.
	// imgui style에서 Color의 0번째 인덱스가 텍스트의 색상을 결정한다.
	FontColor = &ImGui::GetStyle();
	FontColor->Colors[ImGuiCol_::ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

GameEngineFBXWindow::~GameEngineFBXWindow()
{
}

void GameEngineFBXWindow::OnGUI()
{
	// 갱신해야하는 목록 클리어
	FrameUpdateClear();

#pragma region Distinction(구별컬러 정의)
	ImGui::PushFont(LabelFontStyle_Eng);

	// Font Color로 특정 목록의 타입을 구별한다.
	ImGui::Text("Distinction : ");

	// All Load End(메쉬와 애니메이션을 모두 로드한 파일) - MAGENTA
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(1.0f, 0.0f, 1.0f, 1.0f));
	ImGui::Text("ALL");
	ImGui::PopStyleColor();

	ImGui::SameLine();

	// First Load FBX File - WHILE
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::Text("FILE");
	ImGui::PopStyleColor();

	ImGui::SameLine();

	// Mesh Load End FBX File(메쉬를 로드한 파일) - RED
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::Text("MESH");
	ImGui::PopStyleColor();

	ImGui::SameLine();

	// Animation Load End FBX File(애니메이션을 로드한 파일) - BLUE
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	ImGui::Text("ANIMATION");
	ImGui::PopStyleColor();

	// 해당 Font Style 제거
	ImGui::PopFont();

#pragma endregion

	// 한줄 띄우기 위함
	ImGui::Text("");

#pragma region FBX File ListBox
	// 사용자폰트 사용
	ImGui::PushFont(TextFontStyle_Eng);
	ImGui::Text("FBX File List");
	ImGui::PopFont();
	
	// 지정된경로의 FBX File List Get
	std::vector<GameEngineFile> Files = FBXFilePath.GetAllFile("FBX");
	if (0 == Files.size())
	{
		std::string CurPath = FBXFilePath.GetFullPath();
		GameEngineDebug::MsgBoxError(CurPath + " : " "경로에 파일이 존재하지않습니다.");
		return;
	}

	// FBX File ListBox 작성을 위한 준비
	// 1. 파일명 저장(원본, 갱신)
	// 2. 로드한 파일들의 현재 정보상태를 저장
	std::vector<const char*> FBXFileName_Arr;
	for (auto& Ref : Files)
	{
		// 지정된 경로에 위치한 모든 파일의 이름을 Vector에 수집
		std::string& Name = FileNames.emplace_back();
		Name = Ref.FileName();

		// 원본파일명 목록에 저장
		FileNames_Origin.push_back(Name);

		// 현재 파일의 로드정보 상태를 저장
		LoadInfoCheck.insert(std::make_pair(Name, LoadInfoState::FILE));

		// ListBox 추가
		FBXFileName_Arr.push_back(FileNames[FileNames.size() - 1].c_str());
	}

	// File FBX ListBox 생성
	ImGui::PushItemWidth(200.f);
	ImGui::ListBox("##FBXFILELIST", &FBXFileSelect, &FBXFileName_Arr[0], static_cast<ImGuiID>(FBXFileName_Arr.size()), 20);
	ImGui::PopItemWidth();

	



	

	


	//// ListBox의 모든아이템을 검사하면서 로드정보상태에 따라 텍스트컬러를 변경한다.
	//for (int i = 0; i < static_cast<int>(FileNames_Origin.size()); ++i)
	//{
	//	std::string FileName = FileNames_Origin[i];

	//	// 상태체크
	//	LoadInfoState CurState = GetCurFileLoadFlag(FileName);
	//	if (LoadInfoState::NONE != CurState)
	//	{
	//		// 상태에 따른 Item 글자색변경
	//		const char* CurItem = nullptr;
	//		CurItem = FileName.c_str();

	//		switch (CurState)
	//		{
	//			case LoadInfoState::FILE:
	//			{
	//				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), CurItem);
	//				break;
	//			}
	//			case LoadInfoState::MESH:
	//			{
	//				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), CurItem);
	//				break;
	//			}
	//			case LoadInfoState::ANIMATION:
	//			{
	//				ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), CurItem);
	//				break;
	//			}
	//			case LoadInfoState::ALL:
	//			{
	//				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), CurItem);
	//				break;
	//			}
	//		}
	//	}
	//}

	int a = 0;

#pragma endregion



	//Names.clear();
	//OriNames.clear();
	//ActorsNames.clear();

	//std::vector<GameEngineFile> Files = FBXFolder.GetAllFile("FBX");


	//if (0 == Files.size())
	//{
	//	ImGui::Text("FBX파일이 존재하지 않습니다.");

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

	//std::string info = "특이사항 ";

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
	//			GameEngineDebug::MsgBox("매쉬정보가 존재하지 않는 FBX입니다");
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
	//			GameEngineDebug::MsgBox("애니메이션이 존재하지 않는 FBX입니다");
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
	//		GameEngineDebug::MsgBox("매쉬노드 존재하지 않는 FBX 입니다.");
	//	}
	//}

	//ImGui::Text(GameEngineString::AnsiToUTF8Return(info).c_str());

	//ActorControl();
}

void GameEngineFBXWindow::FrameUpdateClear()
{
	// List 관련 목록 클리어
	FileNames.clear();
	FileNames_Origin.clear();
	LoadInfoCheck.clear();

	// 


}

LoadInfoState GameEngineFBXWindow::GetCurFileLoadFlag(const std::string& _FileName)
{
	// 해당 파일의 로드된 정보를 체크
	std::map<std::string, LoadInfoState>::iterator FindIter = LoadInfoCheck.find(_FileName);
	if (LoadInfoCheck.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("해당 파일정보가 존재하지않습니다");
		return LoadInfoState::NONE;
	}

	// 현재 로드정보가 어디까지 완료되었는지 반환
	LoadInfoState LoadIndex = (*FindIter).second;
	return LoadIndex;
}

void GameEngineFBXWindow::SetCurFileLoadFlag(const std::string& _FileName, LoadInfoState _Index)
{
	// 해당 파일을 Get
	std::map<std::string, LoadInfoState>::iterator FindIter = LoadInfoCheck.find(_FileName);
	if (LoadInfoCheck.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("해당 파일정보가 존재하지않습니다");
		return;
	}

	// 해당 파일의 정보로드 상태를 변경한다.
	(*FindIter).second = _Index;
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

