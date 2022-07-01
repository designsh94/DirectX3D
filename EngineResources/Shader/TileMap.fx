#include "CbufferHeader.fx"

cbuffer TextureCutData : register(b1)
{
    float2 TextureCutDataPos;
    float2 TextureCutDataSize;
};

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

VertexOut TileMap_VS(VertexIn _in)
{
    VertexOut Out;

    Out.Position = _in.Position;

    Out.Position.w = 1.0f;
    Out.Position = mul(Out.Position, WVP_);

    Out.Texcoord.x = (_in.Texcoord.x * TextureCutDataSize.x) + TextureCutDataPos.x;
    Out.Texcoord.y = (_in.Texcoord.y * TextureCutDataSize.y) + TextureCutDataPos.y;

    return Out;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

cbuffer ResultColor : register(b0)
{
    float4 vColor;
};

float4 TileMap_PS(VertexOut _in) : SV_Target0
{
    // 타일맵은 오더 적용 안함
    float4 Color = Tex.Sample(Smp, _in.Texcoord.xy) * vColor;
    if (Color.a == 0)
    {
        clip(-1);
    }
    
    return Color;
}

