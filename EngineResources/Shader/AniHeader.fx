
StructuredBuffer<float4x4> ArrAniMationMatrix : register(t12);

void Skinning(inout float4 _Pos, float4 _Weight, int4 _Index, StructuredBuffer<float4x4> _ArrAniMationMatrix)
{
    float4 CalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _Pos.w = 1.0F;

    _Weight[3] = 1.f - _Weight[0] - _Weight[1] - _Weight[2];
    
    for (int i = 0; i < 4; ++i)
    {
        CalPos += _Weight[i] * mul(_Pos, ArrAniMationMatrix[_Index[i]]);
    }
    
    _Pos = CalPos;
}
