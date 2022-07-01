
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

VertexOut Fade_VS(VertexIn _in)
{
    VertexOut Out = (VertexOut)0;

    Out.Position = _in.Position;
    Out.Texcoord = _in.Texcoord;

    return Out;
}

cbuffer FadeData : register(b0)
{
    float Dir; // 1
    float Ratio; // 
    int Clip; // 1 투명도가 0인 애들은 안한다.
    int Temp;
};

Texture2D Target : register(t0);
SamplerState PointSmp : register(s0);

float4 Fade_PS(VertexOut _in) : SV_Target0
{
    float4 Color = Target.Sample(PointSmp, _in.Texcoord.xy);
    
    Color.xyz *= Ratio;
    
    if (Color.a >= 1.0f)
    {
        Color.a = 1.0f;
    }
    
    if (Color.a <= 0.0f)
    {
        Color.a = 0.0f;
    }
    
    return Color;
}