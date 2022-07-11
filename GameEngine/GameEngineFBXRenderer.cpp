#include "PreCompile.h"
#include "CameraComponent.h"
#include "GameEngineLevel.h"

#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineTransform.h"
#include "GameEngineVertexShader.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"

#include "GameEngineFBXRenderer.h"
#include "GameEngineFBXMesh.h"
#include "GameEngineFBXMeshManager.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineFBXAnimationManager.h"

//--------------------------------------------------- FBX Animation ---------------------------------------------------//
void FBXAnimation::Init()
{
    Animation->CalFbxExBoneFrameTransMatrix(Mesh);
    PixAniData = &Animation->AnimationDatas[0];
    Start = 0;
    End = static_cast<UINT>(PixAniData->AniFrameData[0].BoneMatData.size());
    FrameTime = 0.1f;
}

void FBXAnimation::Update(float _DeltaTime)
{
    CurFrameTime += _DeltaTime;
    if (CurFrameTime >= FrameTime)
    {
        CurFrameTime -= FrameTime;
        ++CurFrame;
    }

    if (CurFrame >= End)
    {
        CurFrame = Start;
    }

    int NextFrame = CurFrame;
    ++NextFrame;
    if (NextFrame >= static_cast<int>(End))
    {
        NextFrame = 0;
    }

    for (int i = 0; i < ParentRenderer->BoneData.size(); i++)
    {
        Bone* BoneData = ParentRenderer->FBXMesh->FindBone(i);

        if (true == PixAniData->AniFrameData[i].BoneMatData.empty())
        {
            ParentRenderer->BoneData[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
            return;
        }

        // 현재프레임과 
        FbxExBoneFrameData& CurData = PixAniData->AniFrameData[i].BoneMatData[CurFrame];

        // 다음프레임의 정보가 필요한데
        FbxExBoneFrameData& NextData = PixAniData->AniFrameData[i].BoneMatData[NextFrame];

        // 로컬 스케일
        float4 LerpScale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);

        // 로컬 쿼터니온
        float4 SLerpQ = float4::SLerp(CurData.Q, NextData.Q, CurFrameTime);

        // 로컬 포지션
        float4 LerpPos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);

        size_t Size = sizeof(float4x4);
        float4x4 Mat = float4x4::Affine(LerpScale, SLerpQ, LerpPos);
        ParentRenderer->BoneData[i] = BoneData->BonePos.Offset * float4x4::Affine(LerpScale, SLerpQ, LerpPos);
    }
}

//---------------------------------------------------- FBX Renderer ---------------------------------------------------//
GameEngineFBXRenderer::GameEngineFBXRenderer() :
    FBXMesh(nullptr),
    CurAnimation(nullptr)
{
}

GameEngineFBXRenderer::~GameEngineFBXRenderer()
{
	for (size_t i = 0; i < RenderSets.size(); i++)
	{
		delete RenderSets[i].PipeLine_;
        delete RenderSets[i].ShaderHelper;
	}
    RenderSets.clear();

    for (auto& Animation : Animations)
    {
        delete Animation.second;
    }
    Animations.clear();
}

void GameEngineFBXRenderer::Update(float _DeltaTime)
{
    if (nullptr != CurAnimation)
    {
        CurAnimation->Update(_DeltaTime);
    }
}

void GameEngineFBXRenderer::SetFBXMesh(const std::string& _Value, std::string _PipeLine)
{
    FBXMesh = GameEngineFBXMeshManager::GetInst().Find(_Value);
    if (nullptr == FBXMesh)
    {
        GameEngineDebug::MsgBoxError("존재하지 않는 fbx매쉬를 세팅했습니다.");
    }

    GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find(_PipeLine);

    for (int MeshSetIndex = 0; MeshSetIndex < FBXMesh->GetMeshSet().size(); MeshSetIndex++)
    {
        for (int VertexBufferIndex = 0; VertexBufferIndex < FBXMesh->GetMeshSet()[MeshSetIndex].GameEngineVertexBuffers.size(); VertexBufferIndex++)
        {
            for (int IndexBufferIndex = 0; IndexBufferIndex < FBXMesh->GetMeshSet()[MeshSetIndex].GameEngineIndexBuffers[VertexBufferIndex].size(); IndexBufferIndex++)
            {

                GameEngineVertexBuffer* VertexBuffer = FBXMesh->GetMeshSet()[MeshSetIndex].GameEngineVertexBuffers[VertexBufferIndex];
                GameEngineIndexBuffer* IndexBuffer = FBXMesh->GetMeshSet()[MeshSetIndex].GameEngineIndexBuffers[VertexBufferIndex][IndexBufferIndex];

                RenderSet& RenderSetData = RenderSets.emplace_back();
                RenderSetData.PipeLine_ = Pipe->Clone();
                RenderSetData.ShaderHelper = new GameEngineShaderResHelper();

                RenderSetData.ShaderHelper->ShaderResourcesCheck(RenderSetData.PipeLine_->GetPixelShader());
                RenderSetData.ShaderHelper->ShaderResourcesCheck(RenderSetData.PipeLine_->GetVertexShader());

                if (true == RenderSetData.ShaderHelper->IsConstantBuffer("TransformData"))
                {
                    RenderSetData.ShaderHelper->SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
                }

                if (true == RenderSetData.ShaderHelper->IsConstantBuffer("LightsData"))
                {
                    const LightsData& LightData = GetLevel()->GetMainCamera()->GetLightData();
                    RenderSetData.ShaderHelper->SettingConstantBufferLink("LightsData", LightData);
                }

                if (true == RenderSetData.ShaderHelper->IsStructuredBuffer("ArrAniMationMatrix"))
                {
                    FBXMesh->ImportBone();
                    if (0 == BoneData.size())
                    {
                        BoneData.resize(FBXMesh->GetBoneCount());
                    }

                    const LightsData& LightData = GetLevel()->GetMainCamera()->GetLightData();
                    RenderSetData.ShaderHelper->SettingStructuredBuffer("ArrAniMationMatrix", FBXMesh->GetAnimationBuffer());
                    RenderSetData.ShaderHelper->SettingStructuredBufferLink("ArrAniMationMatrix", &BoneData[0], sizeof(float4x4) * BoneData.size());
                }

                RenderSetData.PipeLine_->SetInputAssembler1VertexBufferSetting(VertexBuffer);
                RenderSetData.PipeLine_->SetInputAssembler2IndexBufferSetting(IndexBuffer);
            }
        }
    }
}

void GameEngineFBXRenderer::Start()
{
	GetLevel()->GetMainCamera()->PushRenderer(GetOrder(), this);
}

void GameEngineFBXRenderer::Render(float _DeltaTime)
{
    for (size_t i = 0; i < RenderSets.size(); i++)
    {
        RenderSets[i].ShaderHelper->Setting();
        RenderSets[i].PipeLine_->Rendering();
        RenderSets[i].ShaderHelper->ReSet();
        RenderSets[i].PipeLine_->Reset();
    }
}

//-------------------------------------------------- FBX Animation Function --------------------------------------------------//

void GameEngineFBXRenderer::CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName)
{
    if (nullptr == FBXMesh)
    {
        GameEngineDebug::MsgBoxError("매쉬를 세팅하지 않은 랜더러에 애니메이션을 만들 수가 없습니다.");
        return;
    }

    if (Animations.end() != Animations.find(_AnimationName))
    {
        GameEngineDebug::MsgBoxError("같은 이름의 애니메이션을 또 만들수는 없습니다.");
        return;
    }

    GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Find(_AnimationFBXName);
    if (nullptr == Animation)
    {
        GameEngineDebug::MsgBoxError("같은 이름의 애니메이션을 또 만들수는 없습니다.");
        return;
    }

    FBXAnimation* NewFBXAnimation = new FBXAnimation();
    NewFBXAnimation->Mesh = FBXMesh;
    NewFBXAnimation->Animation = Animation;
    NewFBXAnimation->ParentRenderer = this;
    NewFBXAnimation->Init();

    Animations.insert(std::make_pair(_AnimationName, NewFBXAnimation));
}

void GameEngineFBXRenderer::ChangeFBXAnimation(const std::string& _AnimationName)
{
    std::map<std::string, FBXAnimation*>::iterator FindIter = Animations.find(_AnimationName);
    if (Animations.end() == FindIter)
    {
        GameEngineDebug::MsgBoxError("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
        return;
    }

    CurAnimation = FindIter->second;
}
