#include "CbufferHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 Normal : NORMAL;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 Normal : NORMAL;
};

VertexOut Texture_VS(VertexIn _in) 
{
    VertexOut Out;

    Out.Position = _in.Position;

    Out.Position.w = 1.0f;
    Out.Position = mul(Out.Position, WVP_);
    Out.Texcoord = _in.Texcoord;
    return Out;
}


cbuffer TextureResultColor : register(b0)
{
    float4 vMulColor;
    float4 vPlusColor;
};

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 Texture_PS(VertexOut _in) : SV_Target0
{
    float4 Color = (Tex.Sample(Smp, _in.Texcoord.xy) * vMulColor);

    if (0.0f == Color.a)
    {
        // 출력안하고 정지
        clip(-1);
    }

    Color += vPlusColor;

    return Color;
}


