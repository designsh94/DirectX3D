// ����Ʈ 1���� ������
struct LightData 
{
    float4 ViewLightDir; // ����Ʈ�� ������ ����
	float4 ViewNegLightDir; // ����Ʈ�� ������ -����
    float4 ViewLightPosition; // ����Ʈ�� ��ġ
    float4 AmbientLight;
    float4 DiffuseLightColor;
    float4 AmbientLightColor;
    float4 SpacularLightColor;
    float4 SpacularLightPow;
    float4 LightPower; // x�� ��ǻ�� ����Ʈ�� ���� y�� ����ŧ���� ���� z�� �ں��Ʈ�� ���� w�� ��� ����
    // float4 CameraPosition;
};

cbuffer LightsData : register(b11)
{
    int LightCount;
    LightData Lights[128];
};

float4 CalculateDirectionDiffuseLight(float4 _vViewNormal, LightData _Light)
{
    // �� �븻�� �� �������� ����?
    _vViewNormal = normalize(_vViewNormal); // N
    _Light.ViewNegLightDir = normalize(_Light.ViewNegLightDir); // L
    
    float4 DiffuseLight = max(0.0f, dot(_vViewNormal.xyz, _Light.ViewNegLightDir.xyz));
    DiffuseLight.w = 1.0f;
    return DiffuseLight * _Light.DiffuseLightColor * _Light.LightPower.x * _Light.LightPower.w;
}


float4 CalculateDirectionSpacularLight(float4 _vViewPosition, float4 _vViewNormal, LightData _Light)
{
    float4 SpacularLight = (float)0.0f;
    
    _vViewNormal = normalize(_vViewNormal); // N
    _Light.ViewNegLightDir = normalize(_Light.ViewNegLightDir); // L
    
    float3 Reflection = normalize((2.0f * _vViewNormal.xyz * dot(_Light.ViewNegLightDir.xyz, _vViewNormal.xyz)) - _Light.ViewNegLightDir.xyz); // N
    float3 NegCameraPosition = -normalize(_vViewPosition); // L
    
    float Spc = max(0.0f, dot(Reflection.xyz, NegCameraPosition.xyz));
    SpacularLight.xyz = pow(Spc, _Light.SpacularLightPow.x);
    // SpacularLight.xyz = Spc;
    SpacularLight.w = 1.0f;
    return SpacularLight * _Light.SpacularLightColor * _Light.LightPower.y * _Light.LightPower.w;
}

float4 CalculateDirectionAmbientLight(LightData _Light)
{
    return _Light.AmbientLight * _Light.AmbientLightColor * _Light.LightPower.z * _Light.LightPower.w;
}
