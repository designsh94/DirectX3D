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
	// 생성시 해당 윈도우에서 사용하려는 Font를 재정의한다.
	ImGuiIO& IO = ImGui::GetIO();

	// C++ 단일문자 '\'을 지원하지않는다 그러므로 '\\' 로 상/하위 디렉터리를 구분하여야만 AddFontFromFileTTF로 해당 파일을 로드할수있다.
	// English
	TextFontStyle_Eng = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 20.f);
	ButtonFontStyle_Eng = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 12.f);
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
	FBXRelatedListUpdate();

#pragma region Distinction(구별컬러 정의)
	ImGui::PushFont(LabelFontStyle_Eng);

	// Font Color로 특정 목록의 타입을 구별한다.
	ImGui::Text("FBXFile Infomation State Color : ");

	// All Load End(메쉬와 애니메이션을 모두 로드한 파일 : 액터생성가능) - GREEN
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	ImGui::Text("ALL");
	ImGui::PopStyleColor();

	ImGui::SameLine();

	// First Load FBX File - WHILE
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::Text("FILE");
	ImGui::PopStyleColor();

	ImGui::SameLine();

	// Mesh Load End FBX File(메쉬를 로드한 파일 : 액터생성가능) - RED
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

#pragma region FBX File ComboBox

	// 사용자폰트 사용
	ImGui::PushFont(TextFontStyle_Eng);
	ImGui::Text("ALL FBXFILE LIST");
	ImGui::PopFont();

	// FBX File Load State
	// 단, 선택된 파일이 있어야지만 상태가 표시된다.
	if (-1 != FBXFileSelect)
	{
		ImGui::SameLine();

		ImGui::PushFont(TextFontStyle_Eng);
		ImGui::Text("State : ");
		ImGui::PopFont();

		ImGui::SameLine();
		
		// 상태값 렌더링
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
	
	// FBX File ComboBox 작성준비
	std::vector<const char*> FBXFileName_Arr;
	for (auto& FileName : AllFileNames)
	{
		// FBXFileName_Arr List 추가
		FBXFileName_Arr.push_back(FileName.c_str());
	}

	// Create File FBX Select ComboBox
	ImGui::PushItemWidth(500.f);
	if (true == ImGui::Combo("##FBXFILELIST", &FBXFileSelect, &FBXFileName_Arr[0], static_cast<int>(FBXFileName_Arr.size())))
	{
		// 선택한 파일이 존재하고,
		if (-1 != FBXFileSelect)
		{
			// 현재 선택된 파일의 메쉬 Get
			SelectMesh = GameEngineFBXMeshManager::GetInst().Find(AllFileNames[FBXFileSelect]);
			if (nullptr == SelectMesh)
			{
				// 최초 선택
				// 즉, 해당 파일의 Mesh 정보를 로드한적이없다고 판단하여 로드한다.
				SelectMesh = GameEngineFBXMeshManager::GetInst().Load(Files[FBXFileSelect].GetFullPath());
				//SelectMesh->MeshLoad();
				if (0 != SelectMesh->GetMeshSet().size())
				{
					SelectMesh->CreateRenderingBuffer();

					// 해당 파일의 모든 메쉬정보로드에 성공하였으므로 해당 파일의 로드정보상태를 갱신
					std::map<std::string, LoadInfoState>::iterator FindIter = FileState.find(AllFileNames[FBXFileSelect]);
					if (FileState.end() != FindIter)
					{
						(*FindIter).second.MeshLoadFinish();
					}
				}
				else
				{
					GameEngineDebug::MsgBox("매쉬정보가 존재하지 않는 FBX입니다");
					GameEngineFBXMeshManager::GetInst().DeletePath(Files[FBXFileSelect].GetFullPath());
					SelectMesh = nullptr;

					// 오직 Animation의 정보만을 가지는 파일이므로 Animation File List에 해당 파일을 추가




				}
			}
			else
			{
				// 중복선택
				GameEngineDebug::MsgBox("이미 로드한 매쉬정보가 존재합니다");
			}
		}
	}
	ImGui::PopItemWidth();

	// 현재 선택 파일의 메쉬정보 목록 작성
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
					GameEngineDebug::MsgBox("애니메이션이 존재하지 않는 FBX입니다");
					GameEngineFBXAnimationManager::GetInst().DeletePath(Files[FBXFileSelect].GetFullPath());
				}
				else
				{
					// 애니메이션 로드가완료 되었으므로 정보로드 상태를 변경
					std::map<std::string, LoadInfoState>::iterator FindIter = FileState.find(AllFileNames[FBXFileSelect]);
					if (FileState.end() != FindIter)
					{
						(*FindIter).second.AnimationLoadFinish();
					}
				}
			}
		}
	}

	// 현재 선택 파일의 애니메이션정보 목록 작성
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

	// FBX File을 선택했을때만 액터생성 및 생성된 액터목록 표시 기능 활성화
	if (-1 != FBXFileSelect)
	{
		// Mesh 정보를 로드한 파일이라면 액터생성 가능(버튼활성화)
		std::map<std::string, LoadInfoState>::iterator FindIter = FileState.find(AllFileNames[FBXFileSelect]);
		if (FileState.end() != FindIter)
		{
			// 메쉬정보가 로드완료된 FBX File일때 액터생성가능
			if ((*FindIter).second.GetLoadInfoState() == LoadState::MESH ||
				(*FindIter).second.GetLoadInfoState() == LoadState::ALL)
			{
				if (true == ImGui::Button("Create Actor", ImVec2(500.f, 25.f)))
				{
					// 액터 생성
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
						GameEngineDebug::MsgBox("매쉬노드 존재하지 않는 FBX 입니다.");
					}
				}
			}
		}

		// 생성된 액터목록 작성
		// 단, 생성된 액터가 한개라도 존재할때 리스트 활성화
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

			// 현재 생성된 Actor List
			ImGui::BeginChildFrame(static_cast<ImGuiID>(reinterpret_cast<uint64_t>("##ACTORFRAME")), ImVec2(500.f, 150.f));

			ImGui::PushItemWidth(496.f);
			if (true == ImGui::ListBox("##ActorList", &ActorSelect, &ActorName_Arr[0], static_cast<int>(ActorName_Arr.size()), 6))
			{
			}
			ImGui::PopItemWidth();

			ImGui::EndChildFrame();
		}
	}

	// 선택된 액터가 존재할때
	if (-1 != ActorSelect)
	{
		ActorController();
	}

#pragma endregion

}

bool GameEngineFBXWindow::FBXFilePathCompare()
{
	// 지정된 경로의 파일변화가 있다면 파일목록갱신

	// 현재 지정된 경로의 파일목록을 Get
	Files.clear();
	Files = FBXFilePath.GetAllFile("FBX");
	if (0 == Files.size())
	{
		std::string CurPath = FBXFilePath.GetFullPath();
		GameEngineDebug::MsgBoxError(CurPath + " : " "경로에 파일이 존재하지않습니다.");
		return false;
	}

	std::vector<std::string> NewFiles;
	for (auto& NewFileName : Files)
	{
		NewFiles.push_back(NewFileName.GetFileName());
	}

	// 현재 에디터에 포함된 파일목록과 비교처리
	if (AllFileNames.size() == NewFiles.size() && true == std::equal(AllFileNames.begin(), AllFileNames.end(), NewFiles.begin()))
	{
		// 비교하여 동일한 목록이면 false 반환
		return false;
	}

	// 아니면 변화있으므로 파일이름목록 갱신 및 파일목록 로드가 끝났으므로 현재 로드상태정보 셋팅
	AllFileNames.clear();
	FileState.clear();
	for (auto& FileName : Files)
	{
		// 전체 파일명 목록 작성
		AllFileNames.push_back(FileName.GetFileName());

		// 파일별 정보로드 상태정보 작성
		FileState.insert(std::make_pair(FileName.GetFileName(), LoadInfoState(LoadState::FILE)));
	}

	return true;
}

void GameEngineFBXWindow::FBXRelatedListUpdate()
{
	// FBX FILE PATH 검사
	// 단, 변화가 있다면 파일목록이 바뀐다.
	if (true == FBXFilePathCompare())
	{
		// 지정된 경로에 파일의 변화가 일어났으므로 
		// 현재 선택된 메쉬와 FBX File Index를 초기화

		// Select Current Mesh Clear
		SelectMesh = nullptr;

		// Select FBXFile Current Index Clear
		FBXFileSelect = -1;
	}

	// 생성된 액터 목록 클리어
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

	// 변경사항 즉시 적용
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
