#include "PreCompile.h"
#include "GameEngineFBXWindow.h"
#include "GameEngineImageShotWindow.h"
#include "GameEngineFBXMeshManager.h"
#include "GameEngineFBXAnimationManager.h"
#include "GameEngineCore.h"

GameEngineFBXWindow::GameEngineFBXWindow() :
	FBXFileSelect(-1),
	ActorSelect(-1)
{
}

GameEngineFBXWindow::~GameEngineFBXWindow()
{
}

void GameEngineFBXWindow::OnGUI()
{
	// 주말에 다시 만들기 인터페이스가 맘에 안듦



	Names.clear();
	OriNames.clear();
	ActorsNames.clear();

	std::vector<GameEngineFile> Files = FBXFolder.GetAllFile("FBX");


	if (0 == Files.size())
	{
		ImGui::Text("FBX파일이 존재하지 않습니다.");

		return;
	}

	std::vector<const char*> ActorArr;
	for (GameEngineActor* Ref : Actors)
	{
		std::string Name;

		if ("" == Ref->GetName())
		{
			Name = "None";
		}

		ActorsNames.push_back(Name);
		ActorArr.push_back(ActorsNames[ActorsNames.size() - 1].c_str());
	}


	std::vector<const char*> Arr;
	for (auto& Ref : Files)
	{
		std::string& Name = Names.emplace_back();

		Name = Ref.FileName();
		OriNames.push_back(Name);

		if (GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Find(Ref.FileName()))
		{
			Name += " M";
		}
		if (GameEngineFBXAnimation* Mesh = GameEngineFBXAnimationManager::GetInst().Find(Ref.FileName()))
		{
			Name += " A";
		}


		Arr.push_back(Names[Names.size() - 1].c_str());
	}

	ImGui::BeginChildFrame(static_cast<ImGuiID>(reinterpret_cast<uint64_t>("##FBXRANGE")), { 500, 500 });
	ImGui::Text("FBXLIST");

	ImGui::PushItemWidth(200);
	ImGui::ListBox("##FBXLIST", &FBXFileSelect, &Arr[0], static_cast<ImGuiID>(Arr.size()), 20);

	ImGui::SameLine();

	ImGui::PushItemWidth(200);

	const char** ActorNamePtr = nullptr;

	if (Actors.size())
	{
		ActorNamePtr = &ActorArr[0];
	}

	ImGui::ListBox("##ActorList", &ActorSelect, ActorNamePtr, static_cast<ImGuiID>(ActorArr.size()), 20);

	ImGui::EndChildFrame();
	ImGui::SameLine();

	std::string info = "특이사항 ";

	static float Scroll = 0.0f;

	GameEngineFBXMesh* SelectMesh = nullptr;
	if (FBXFileSelect != -1)
	{
		SelectMesh = GameEngineFBXMeshManager::GetInst().Find(OriNames[FBXFileSelect]);
	}

	GameEngineFBXAnimation* SelectAnimation = nullptr;
	if (FBXFileSelect != -1)
	{
		SelectAnimation = GameEngineFBXAnimationManager::GetInst().Find(OriNames[FBXFileSelect]);
	}

	ImGui::BeginChildFrame(static_cast<ImGuiID>(reinterpret_cast<uint64_t>("##NODETREERANGE")), { (Scroll * 3) + 300 , 500 }/*, ImGuiWindowFlags_HorizontalScrollbar*/);

	if (nullptr != SelectMesh)
	{
		bool Check = true;
		SelectMesh->RecursiveAllNode(
			[&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _ParentReturn)
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
					info += "LODGroup";
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

					Result = ImGui::TreeNodeEx(DisName.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf);

					if (Scroll <= ImGui::GetScrollMaxX())
					{
						Scroll = ImGui::GetScrollMaxX();
					}

				}
				return Result;
			}
			,
				[&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _StartReturn)
			{
				if (_StartReturn == 1)
				{
					ImGui::TreePop();
				}
			},
				1
				);
	}
	else if (nullptr != SelectAnimation)
	{
		bool Check = true;
		SelectAnimation->RecursiveAllNode(
			[&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _ParentReturn)
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
					info += "LODGroup";
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

					Result = ImGui::TreeNodeEx(DisName.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf);

					if (Scroll <= ImGui::GetScrollMaxX())
					{
						Scroll = ImGui::GetScrollMaxX();
					}

				}
				return Result;
			}
			,
				[&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _StartReturn)
			{
				if (_StartReturn == 1)
				{
					ImGui::TreePop();
				}
			},
				1
				);
	}

	ImGui::EndChildFrame();

	if (nullptr == SelectMesh)
	{
		if (ImGui::Button("MeshLoad"))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(Files[FBXFileSelect].GetFullPath());
			Mesh->MeshLoad();

			if (0 == Mesh->GetMeshSet().size())
			{
				GameEngineDebug::MsgBox("매쉬정보가 존재하지 않는 FBX입니다");
				GameEngineFBXMeshManager::GetInst().DeletePath(Files[FBXFileSelect].GetFullPath());
			}
			else
			{
				Mesh->CreateRenderingBuffer();
			}
		}
	}

	if (nullptr == SelectAnimation)
	{
		if (ImGui::Button("AnimationLoad"))
		{
			GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(Files[FBXFileSelect].GetFullPath());

			if (false == Animation->LoadAnimation())
			{
				GameEngineDebug::MsgBox("애니메이션이 존재하지 않는 FBX입니다");
				GameEngineFBXAnimationManager::GetInst().DeletePath(Files[FBXFileSelect].GetFullPath());
			}
			else {

			}
		}
	}

	if (nullptr != SelectMesh
		&& 0 != SelectMesh->GetMeshSet().size()
		&& ImGui::Button("Actor Create"))
	{
		if (0 == SelectMesh->GetMeshSet().size())
		{
			SelectMesh->MeshLoad();
			SelectMesh->CreateRenderingBuffer();
		}

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

	ImGui::Text(GameEngineString::AnsiToUTF8Return(info).c_str());

	ActorControl();
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

