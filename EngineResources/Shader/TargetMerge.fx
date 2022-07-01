
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

VertexOut TargetMerge_VS(VertexIn _in) 
{
    VertexOut Out = (VertexOut)0;

    Out.Position = _in.Position;
    Out.Texcoord = _in.Texcoord;

    return Out;
}

Texture2D Tex : register(t0);
SamplerState PointSmp : register(s0);

float4 TargetMerge_PS(VertexOut _in) : SV_Target0
{
    float4 Color = Tex.Sample(PointSmp, _in.Texcoord.xy);
    
    if (Color.a >= 1.0f)
    {
        Color.a = 1.0f;
    }
    
    if (Color.a <= 0.0f)
    {
        Color.a = 0.0f;
    }

    // ºû¿¬»ê ÇÒ·¡.
    
    return Color;
}