
cbuffer TransformData : register(b0)
{
    float4 vWorldPosition_;
    float4 vWorldRotation_;
    float4 vWorldScaling_;

    float4 vLocalPosition_;
    float4 vLocalRotation_;
    float4 vLocalScaling_;

    float4x4 LocalScaling_;
    float4x4 LocalRotation_;
    float4x4 LocalPosition_;
    float4x4 LocalWorld_;

    float4x4 Parent_;
    float4x4 WorldWorld_;

    float4x4 View_;
    float4x4 Projection_;

    float4x4 WV_;
    float4x4 WVP_;
};

