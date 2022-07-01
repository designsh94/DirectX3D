
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

VertexOut Blur_VS(VertexIn _in)
{
    VertexOut Out = (VertexOut) 0;

    Out.Position = _in.Position;
    Out.Texcoord = _in.Texcoord;

    return Out;
}

cbuffer BlurData : register(b0)
{
    int FilterStartX; // -2
    int FilterStartY; // -2
    int FilterEndX; // 2
    int FilterEndY; // 2
    int FilterCount;
    float FilterSum;
    float ImagePixelUVX;
    float ImagePixelUVY;
    float FilterPixelX;
    float FilterPixelY;
    float Temp0;
    float Temp1;
};

Texture2D Target : register(t0);
Texture2D Filter : register(t1);
SamplerState PointSmp : register(s0);

float4 Blur_PS(VertexOut _in) : SV_Target0
{
    // _in.Texcoord.x + 1 / 1280
    float4 SumColor = (float4) 0.0f;
    
    for (int y = FilterStartY; y < FilterEndY + 1; ++y)
    {
        for (int x = FilterStartX; x < FilterEndX + 1; ++x)
        {
            float2 CheckUV = float2(_in.Texcoord.x + (ImagePixelUVX * x), _in.Texcoord.y + (ImagePixelUVY * y));
            
            float2 FilterUV = float2(FilterPixelX * (x + FilterEndX), FilterPixelY * (y + FilterEndY));
            float4 Ratio = Filter.Sample(PointSmp, FilterUV.xy);
            SumColor += Target.Sample(PointSmp, CheckUV.xy) * Ratio.x;
            
        }
    }
    
    SumColor /= FilterSum;
    
        
    if (SumColor.a >= 1.0f)
    {
        SumColor.a = 1.0f;
    }
    
    if (SumColor.a <= 0.0f)
    {
        SumColor.a = 0.0f;
    }
    
    return SumColor;
}