#include "PreCompile.h"
#include "GameEngineMath.h"

// Static Var
const float GameEngineMath::PI = 3.14159265359f;
const float GameEngineMath::PI2 = 3.14159265359f * 2;
const float GameEngineMath::DegreeToRadian = GameEngineMath::PI / 180.0f;
const float GameEngineMath::RadianToDegree = 180.0f / GameEngineMath::PI;

const float4 float4::ZERO = { 0.0f, 0.0f, 0.0f, 0.0f };
const float4 float4::ONE = { 1.0f, 1.0f, 1.0f, 1.0f };

const float4 float4::LEFT = { -1.0f, 0.0f };
const float4 float4::RIGHT = { 1.0f, 0.0f };
const float4 float4::UP = { 0.0f, 1.0f };
const float4 float4::DOWN = { 0.0f, -1.0f };

const float4 float4::FRONT = { 0.0f, 0.0f, 1.f };
const float4 float4::BACK = { 0.0f, 0.0f, -1.f };

const float4 float4::RED = { 1.0f, 0.0f, 0.0f, 1.0f };
const float4 float4::BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
const float4 float4::GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
const float4 float4::WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
const float4 float4::BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
const float4 float4::NONE = { 0.0f, 0.0f, 0.0f, 0.0f };

// Static Func

float4 float4::MatrixToQuaternion(const float4x4& M)
{
	float4 Return;

	if (M.ArrVector[0].IsNearlyZero() || M.ArrVector[1].IsNearlyZero() || M.ArrVector[2].IsNearlyZero())
	{
		Return.x = 0.0f;
		Return.y = 0.0f;
		Return.z = 0.0f;
		Return.w = 1.0f;
		return Return;
	}

	float	s;

	// Check diagonal (trace)
	const float tr = M.Arr2D[0][0] + M.Arr2D[1][1] + M.Arr2D[2][2];

	if (tr > 0.0f)
	{
		float InvS = InvSqrt(tr + 1.f);
		Return.w = 0.5f * (1.f / InvS);
		s = 0.5f * InvS;

		Return.x = (M.Arr2D[1][2] - M.Arr2D[2][1]) * s;
		Return.y = (M.Arr2D[2][0] - M.Arr2D[0][2]) * s;
		Return.z = (M.Arr2D[0][1] - M.Arr2D[1][0]) * s;
	}
	else
	{
		// diagonal is negative
		int i = 0;

		if (M.Arr2D[1][1] > M.Arr2D[0][0])
			i = 1;

		if (M.Arr2D[2][2] > M.Arr2D[i][i])
			i = 2;

		static const int nxt[3] = { 1, 2, 0 };
		const int j = nxt[i];
		const int k = nxt[j];

		s = M.Arr2D[i][i] - M.Arr2D[j][j] - M.Arr2D[k][k] + 1.0f;

		float InvS = InvSqrt(s);

		float qt[4];
		qt[i] = 0.5f * (1.f / InvS);

		s = 0.5f * InvS;

		qt[3] = (M.Arr2D[j][k] - M.Arr2D[k][j]) * s;
		qt[j] = (M.Arr2D[i][j] + M.Arr2D[j][i]) * s;
		qt[k] = (M.Arr2D[i][k] + M.Arr2D[k][i]) * s;

		Return.x = qt[0];
		Return.y = qt[1];
		Return.z = qt[2];
		Return.w = qt[3];
	}

	return Return;
}

float4 float4::RotateYRadian(const float4& _OriginVector, float _Radian)
{
	float4x4 Rot;
	Rot.RotationYRad(_Radian);
	return _OriginVector * Rot;
}

float4 float4::RotateXRadian(const float4& _OriginVector, float _Radian)
{
	float4x4 Rot;
	Rot.RotationXRad(_Radian);
	return _OriginVector * Rot;
}

float4 float4::RotateZRadian(const float4& _OriginVector, float _Radian)
{
	float4x4 Rot;
	Rot.RotationZRad(_Radian);
	return _OriginVector * Rot;
}

float4 float4::DirZRadian(float _Radian)
{
	float4x4 Rot;
	Rot.RotationZRad(_Radian);
	return float4::RIGHT * Rot;
}

float4 float4::operator*(const float4x4& _Value) const
{
	return DirectX::XMVector4Transform(DirectVector, _Value.DirectMatrix);
}

float4& float4::operator*=(const float4x4& _Value)
{
	DirectVector = DirectX::XMVector4Transform(DirectVector, _Value.DirectMatrix);
	return *this;
}
