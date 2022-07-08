#include "CbufferHeader.fx"
#include "LightHeader.fx"
#include "AniHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Normal : NORMAL;
    float4 Weight : BLENDWEIGHT; // 내가 곱해져야할 뼈에게 얼마나 영향을 받느냐에 대한 정보.
    int4 Index : BLENDINDICES; // 내가 곱해져야할 뼈의 인덱스 정보
};

struct VertexOut
{
    // 
    float4 Position : SV_POSITION;
    float4 ViewPosition : POSITION;
    float4 ViewNormal : NORMAL;
};

VertexOut ColorAni_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut)0;
    
    
    Skinning(_In.Position, _In.Weight, _In.Index, ArrAniMationMatrix);
    
    // [][][][]
    // [][][][]
    // [][][][1]
    // [][][][]
    
    _In.Position.w = 1.0f;
    Out.Position = mul(_In.Position, WVP_);
    Out.ViewPosition = mul(_In.Position, WV_);
    
    // w에 z값 들어있잖아.
    // Out.Position.w = 1.0f;
    
    
    // 픽셀을 건져내기 위한 포지션
    
    _In.Normal.w = 0.0f;
    Out.ViewNormal = normalize(mul(_In.Normal, WV_));
    Out.ViewNormal.w = 0.0f;

    return Out;
}

cbuffer ResultColor : register(b0)
{
    float4 vDiffuseColor;
};

float4 ColorAni_PS(VertexOut _In) : SV_Target0
{
    float4 DiffuseLight = (float) 0.0f;
    float4 SpacularLight = (float) 0.0f;
    float4 AmbientLight = (float) 0.0f;
    for (int i = 0; i < LightCount; ++i)
    {
        DiffuseLight += CalculateDirectionDiffuseLight(_In.ViewNormal, Lights[i]);
        SpacularLight += CalculateDirectionSpacularLight(_In.ViewPosition, _In.ViewNormal, Lights[i]);
        AmbientLight += CalculateDirectionAmbientLight(Lights[i]);
    }
    
    float4 Color = vDiffuseColor;
    
    if (LightCount != 0)
    {
        Color *= (DiffuseLight + SpacularLight + AmbientLight);
    }
    
    Color.w = 1.0f;
    
    return
Color;
}