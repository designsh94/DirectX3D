#include "CbufferHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float4 Texcoord : TEXTURECOORD;
};


cbuffer TextureAtlasData : register(b1)
{
    // 0.0f 0.0f 
    float2 TextureCutDataPos;
    // 1 / 8 1 / 8
    float2 TextureCutDataSize;
};


VertexOut TextureAtlas_VS(VertexIn _in)
{
    VertexOut Out;

    Out.Position = _in.Position;

    Out.Position.w = 1.0f;
    Out.Position = mul(Out.Position, WVP_);

    // 0 0 
    // 1, 0
    // 1, 1
    //                   1                 0.125             +      0.125 * _x
    Out.Texcoord.x = (_in.Texcoord.x * TextureCutDataSize.x) + TextureCutDataPos.x;
    //                   1                 0.125             +      0.125 * _x
    Out.Texcoord.y = (_in.Texcoord.y * TextureCutDataSize.y) + TextureCutDataPos.y;

    return Out;
}


cbuffer TextureAtlasResultColor : register(b0)
{
    float4 vMulColor;
    float4 vPlusColor;
};

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 TextureAtlas_PS(VertexOut _in) : SV_Target0
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


