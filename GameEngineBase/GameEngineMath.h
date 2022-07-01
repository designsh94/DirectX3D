#pragma once

class GameEngineMath
{
public:
	static const float PI;
	static const float PI2;
	static const float DegreeToRadian;
	static const float RadianToDegree;
};

class float4x4;
class float4
{
public:
	static const float4 ZERO;
	static const float4 ONE;

	static const float4 LEFT;
	static const float4 RIGHT;
	static const float4 UP;
	static const float4 DOWN;

	static const float4 FRONT;
	static const float4 BACK;

	static const float4 RED;
	static const float4 BLUE;
	static const float4 GREEN;
	static const float4 WHITE;
	static const float4 BLACK;
	static const float4 NONE;

public:
	static float InvSqrt(float f)
	{
		const __m128 fOneHalf = _mm_set_ss(0.5f);
		__m128 Y0, X0, X1, X2, FOver2;
		float temp;

		Y0 = _mm_set_ss(f);
		X0 = _mm_rsqrt_ss(Y0);	// 1/sqrt estimate (12 bits)
		FOver2 = _mm_mul_ss(Y0, fOneHalf);

		// 1st Newton-Raphson iteration
		X1 = _mm_mul_ss(X0, X0);
		X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
		X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

		// 2nd Newton-Raphson iteration
		X2 = _mm_mul_ss(X1, X1);
		X2 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X2));
		X2 = _mm_add_ss(X1, _mm_mul_ss(X1, X2));

		_mm_store_ss(&temp, X2);
		return temp;
	}

	bool IsNearlyZero() const
	{
		return fabsf(x) <= 1.e-4f && fabsf(y) <= 1.e-4f && fabsf(z) <= 1.e-4f;
	}

	static float4 MatrixToQuaternion(const class float4x4& M);

public:
	// 거리 : 두 벡터간의 거리
	static float Distance(float4 _Left, float4 _Right)
	{
		return static_cast<float>(sqrt(pow(_Left.x - _Right.x, 2) + pow(_Left.y - _Right.y, 2)));
	}

	// 외적 : 두 벡터를 외적(두 벡터를 외적하면 두 벡터에 모두 수직인 벡터 도출)
	static float4 Cross3D(float4 _Left, float4 _Right)
	{
		return DirectX::XMVector3Cross(_Left.DirectVector, _Right.DirectVector);
	}

	// 내적 : 두 벡터를 내적(COS(세타)를 도출)
	static float Dot3D(float4 _Left, float4 _Right)
	{
		return DirectX::XMVector3Dot(_Left.DirectVector, _Right.DirectVector).m128_f32[0];
	}

	// 정사영 : 두벡터를 내적하여 정사영 길이를 도출(단, 둘중 하나의 벡터가 반드시 단위벡터이어야만 정사영길이 도출)
	static float Dot3DToLen(float4 _Left, float4 _Right)
	{
		_Right.Normalize3D();
		return DirectX::XMVector3Dot(_Left.DirectVector, _Right.DirectVector).m128_f32[0];
	}

	// 내적하는 두 벡터를 단위벡터화(크기가 1인  벡터)하여 두 벡터를 내적하여 COS(세타) 값을 도출
	static float Dot3DToCos(float4 _Left, float4 _Right)
	{
		_Left.Normalize3D();
		_Right.Normalize3D();
		return DirectX::XMVector3Dot(_Left.DirectVector, _Right.DirectVector).m128_f32[0];
	}

	// 내적하는 두 벡터의 ACOS(세타)를 얻어 라디안 각도를 디그리 각도로 변환하여 반환
	static float DegreeDot3DToACosAngle(float4 _Left, float4 _Right)
	{
		return DirectX::XMConvertToDegrees(acosf(Dot3DToCos(_Left, _Right)));
	}

	// 역함수 : cos세타에서 cos을 제외하고 라디안(각도)값을 도출
	static float Dot3DToCosAngle(float4 _Left, float4 _Right)
	{
		return acos(Dot3DToCos(_Left, _Right));
	}

	// 3차원 회전
	static float4 Rotate3DDegree(const float4& _Vector, const float4& _OriginAngle)
	{
		float4 ResultVector = _Vector;
		ResultVector.Rotate3DDegree(_OriginAngle);
		return ResultVector;
	}

	static float4 RotateYDegree(const float4& _OriginVector, float _Degree)
	{
		return RotateYRadian(_OriginVector, _Degree * GameEngineMath::DegreeToRadian);
	}
	static float4 RotateYRadian(const float4& _OriginVector, float _Radian);

	static float4 RotateXDegree(const float4& _OriginVector, float _Degree)
	{
		return RotateXRadian(_OriginVector, _Degree * GameEngineMath::DegreeToRadian);
	}
	static float4 RotateXRadian(const float4& _OriginVector, float _Radian);

	static float4 RotateZDegree(const float4& _OriginVector, float _Degree)
	{
		return RotateZRadian(_OriginVector, _Degree * GameEngineMath::DegreeToRadian);
	}
	static float4 RotateZRadian(const float4& _OriginVector, float _Radian);

	static float4 DirZDegree(float _Degree)
	{
		return DirZRadian(_Degree * GameEngineMath::DegreeToRadian);
	}

	static float4 DirZRadian(float _Radian);

public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};

		float Arr1D[4];

		DirectX::XMFLOAT3 DxXmfloat3;
		DirectX::XMFLOAT4 DxXmfloat4;

		DirectX::XMVECTOR DirectVector;
	};

public:
	float4() :
		x(0.0f),
		y(0.0f),
		z(0.0f),
		w(1.0f)
	{
	}

	float4(float _x, float _y) :
		x(_x),
		y(_y),
		z(0.0f),
		w(1.0f)
	{
	}

	float4(float _x, float _y, float _z, float _w = 1.0f) :
		x(_x),
		y(_y),
		z(_z),
		w(_w)
	{
	}

	float4(DirectX::XMVECTOR _value) :
		DirectVector(_value)
	{
	}

	~float4()
	{
	}

public:		// delete constructer
	float4(const float4& _other) :
		x(_other.x),
		y(_other.y),
		z(_other.z),
		w(_other.w)
	{
	}

	float4 operator+(const float4 _value) const
	{
		return DirectX::XMVectorAdd(DirectVector, _value.DirectVector);
	}

	float4 operator-() const
	{
		return DirectX::XMVectorNegate(DirectVector);
	}

	float4 operator-(const float4 _value) const
	{
		return DirectX::XMVectorSubtract(DirectVector, _value.DirectVector);
	}

	float4 operator*(const float _value) const
	{
		float4 CalVector = { _value , _value , _value , 1.0f };
		return DirectX::XMVectorMultiply(DirectVector, CalVector.DirectVector);
	}

	float4 operator*(const float4x4& _Value) const;
	float4& operator*=(const float4x4& _Value);

	float4 operator*(const float4 _value) const
	{
		return DirectX::XMVectorMultiply(DirectVector, _value.DirectVector);
	}

	float4 operator/(const float4 _value) const
	{
		return DirectX::XMVectorDivide(DirectVector, _value.DirectVector);
	}


	float4& operator+=(const float4 _value)
	{
		DirectVector = DirectX::XMVectorAdd(DirectVector, _value.DirectVector);
		return *this;
	}

	float4& operator-=(const float4 _value)
	{
		DirectVector = DirectX::XMVectorSubtract(DirectVector, _value.DirectVector);
		return *this;
	}

	float4& operator*=(const float4 _value)
	{
		DirectVector = DirectX::XMVectorMultiply(DirectVector, _value.DirectVector);
		return *this;
	}

	float4& operator*=(const float _Value)
	{
		float4 Value = float4(_Value, _Value, _Value, 1.0f);
		DirectVector = DirectX::XMVectorMultiply(DirectVector, Value.DirectVector);
		return *this;
	}

	float4& operator/=(const float4& _Value)
	{
		DirectVector = DirectX::XMVectorDivide(DirectVector, _Value.DirectVector);
		return *this;
	}

	float4& operator/=(const float _Value)
	{
		float4 Value = float4(_Value, _Value, _Value, 1.0f);
		DirectVector = DirectX::XMVectorDivide(DirectVector, Value.DirectVector);
		return *this;
	}

	// 대입연산자
	float4& operator=(const float4& _Value)
	{
		DirectVector = DirectX::XMVectorSet(_Value.x, _Value.y, _Value.z, _Value.w);
		return *this;
	}

	bool operator==(const float4& _Value) const
	{
		return x == _Value.x && y == _Value.y && z == _Value.z && w == _Value.w;
	}

	bool operator!=(const float4& _Value) const
	{
		return x != _Value.x || y != _Value.y || z != _Value.z || w != _Value.w;
	}

public:
	unsigned int ColorToUint() const
	{
		char Color255[4] = { 0, };

		Color255[0] = static_cast<char>(x * 255.0f);
		Color255[1] = static_cast<char>(y * 255.0f);
		Color255[2] = static_cast<char>(z * 255.0f);
		Color255[3] = static_cast<char>(w * 255.0f);

		return *(reinterpret_cast<unsigned int*>(Color255));
	}

	std::string ToString()
	{
		return " x : " + std::to_string(x) + " y : " + std::to_string(y) + " z : " + std::to_string(z);
	}

public:
	int ix() const
	{
		return static_cast<int>(x);
	}

	int iy() const
	{
		return static_cast<int>(y);
	}

	int iz() const
	{
		return static_cast<int>(z);
	}

	int iw() const
	{
		return static_cast<int>(w);
	}

	int uix() const
	{
		return static_cast<unsigned int>(x);
	}

	int uiy() const
	{
		return static_cast<unsigned int>(y);
	}

	int uiz() const
	{
		return static_cast<unsigned int>(z);
	}

	int uiw() const
	{
		return static_cast<unsigned int>(w);
	}


	float hx() const
	{
		return x * 0.5f;
	}

	float hy() const
	{
		return y * 0.5f;
	}

	float hz() const
	{
		return z * 0.5f;
	}

	float4 halffloat4() const
	{
		return { hx(), hy(), hz() };
	}

	int ihx() const
	{
		return static_cast<int>(hx());
	}

	int ihy() const
	{
		return static_cast<int>(hy());
	}

	int ihz() const
	{
		return static_cast<int>(hz());
	}

	bool IsZero() const
	{
		return ix() == 0 && iy() == 0 && iz() == 0;
	}

	float Len2D()
	{
		float4 Len = DirectX::XMVector2Length(DirectVector);
		return Len.x;
	}

	float Len3D()
	{
		float4 Len = DirectX::XMVector3Length(DirectVector);
		return Len.x;
	}

	float4 NormalizeReturn3D() const
	{
		return DirectX::XMVector3Normalize(DirectVector);
	}

	void Normalize3D()
	{
		DirectVector = DirectX::XMVector3Normalize(DirectVector);
	}

	float4 ToRadianAngle() const
	{
		return this->operator*(GameEngineMath::DegreeToRadian);
	}

	float4 ToDegreeQuaternion() const
	{
		return DirectX::XMQuaternionRotationRollPitchYawFromVector(ToRadianAngle().DirectVector);
	}

	POINT GetWindowPoint()
	{
		return { ix(), iy() };
	}

	float4& Rotate3DDegree(const float4& _Angle)
	{
		DirectVector = DirectX::XMVector3Rotate(DirectVector, DirectX::XMQuaternionRotationRollPitchYawFromVector(_Angle.DirectVector));
		return *this;
	}

	void RotateXDegree(float _Deg)
	{
		*this = RotateXDegree(*this, _Deg);
		return;
	}

	void RotateYDegree(float _Deg)
	{
		*this = RotateYDegree(*this, _Deg);
		return;
	}

	void RotateZDegree(float _Deg)
	{
		*this = RotateZDegree(*this, _Deg);
		return;
	}
};

class int4
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			__int64 HighValue;
			__int64 LowValue;
		};
	};
};

class Figure
{
public:
	float4 pos_;
	float4 size_;

public:
	float Left()
	{
		return pos_.x - size_.hx();
	}

	float Right()
	{
		return pos_.x + size_.hx();
	}

	float Top()
	{
		return pos_.y - size_.hy();
	}

	float Bot()
	{
		return pos_.y + size_.hx();
	}

	int iLeft()
	{
		return pos_.ix() - size_.ihx();
	}

	int iRight()
	{
		return pos_.ix() + size_.ihx();
	}

	int iTop()
	{
		return pos_.iy() - size_.ihy();
	}

	int iBot()
	{
		return pos_.iy() + size_.ihy();
	}

	float4 LeftTopfloat4()
	{
		return { Left(), Top() };
	}

	float4 RightTopfloat4()
	{
		return { Right(), Top() };
	}

	float4 LeftBotfloat4()
	{
		return { Left(), Bot() };
	}

	float4 RightBotfloat4()
	{
		return { Right(), Bot() };
	}

public:
	Figure(float4 _Pos, float4 _Size) :
		pos_(_Pos), 
		size_(_Size)
	{

	}
};

// 행렬
class float4x4
{
public:
	static float4x4 Transformation(float4 _Scale, float4 _RotQ, float4 _Pos)
	{
		return DirectX::XMMatrixTransformation(float4::ZERO.DirectVector, float4::ZERO.DirectVector, _Scale.DirectVector, float4::ZERO.DirectVector, _RotQ.DirectVector, _Pos.DirectVector);
	}

public:
	union
	{
		float Arr2D[4][4];
		float4 ArrVector[4];

		struct
		{
			float4 vx;
			float4 vy;
			float4 vz;
			float4 vw;
		};

		float Arr1D[4 * 4];

		DirectX::XMFLOAT4X4 DxXmfloat4x4;
		DirectX::XMMATRIX DirectMatrix;
	};

public:
	float4x4() :
		DirectMatrix(DirectX::XMMatrixIdentity())
	{

	}

	float4x4(const float4x4& _Other) :
		DirectMatrix(_Other.DirectMatrix)
	{

	}

	float4x4(const DirectX::XMMATRIX& _Other) :
		DirectMatrix(_Other)
	{

	}

	~float4x4()
	{

	}

public:
	float4x4 operator*(const float4x4& _value)
	{
		return DirectX::XMMatrixMultiply(DirectMatrix, _value.DirectMatrix);
	}

	float4x4& operator*=(const float4x4& _value)
	{
		DirectMatrix = DirectX::XMMatrixMultiply(DirectMatrix, _value.DirectMatrix);

		return *this;
	}

	float4x4& operator=(const float4x4& _value)
	{
		DirectMatrix = _value.DirectMatrix;
		return *this;
	}

public: // 크기변환 행렬
	void Scaling(const float4& _Value)
	{
		DirectMatrix = DirectX::XMMatrixScalingFromVector(_Value.DirectVector);
	}

	void Scaling(const float _Value)
	{
		DirectMatrix = DirectX::XMMatrixScalingFromVector(float4(_Value, _Value, _Value).DirectVector);
	}

	void Scaling2D(const float _Value)
	{
		DirectMatrix = DirectX::XMMatrixScalingFromVector(float4(_Value, _Value, 1.0f).DirectVector);
	}

public: // 위치변환 행렬
	void Translation(const float4& _Value)
	{
		DirectMatrix = DirectX::XMMatrixTranslationFromVector(_Value.DirectVector);
	}

public: // 회전변환 행렬
	void RotationDeg(const float4& _Value)
	{
		RotationRad(_Value * GameEngineMath::DegreeToRadian);
	}

	void RotationXDeg(const float& _Value)
	{
		RotationXRad(_Value * GameEngineMath::DegreeToRadian);
	}

	void RotationYDeg(const float& _Value)
	{
		RotationYRad(_Value * GameEngineMath::DegreeToRadian);
	}

	void RotationZDeg(const float& _Value)
	{
		RotationZRad(_Value * GameEngineMath::DegreeToRadian);
	}

	void RotationRad(const float4& _Value)
	{
		DirectMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(_Value.DirectVector);
	}

	void RotationXRad(const float& _Value)
	{
		DirectMatrix = DirectX::XMMatrixRotationX(_Value);
	}

	void RotationYRad(const float& _Value)
	{
		DirectMatrix = DirectX::XMMatrixRotationY(_Value);
	}

	void RotationZRad(const float& _Value)
	{
		DirectMatrix = DirectX::XMMatrixRotationZ(_Value);
	}

public: // 단위행렬 생성
	void Identity()
	{
		DirectMatrix = DirectX::XMMatrixIdentity();
	}

public: // 전치행렬 : 주대각선(LT->RB)를 기준으로 반사대칭한 행렬 생성
	void Transpose()
	{
		DirectMatrix = DirectX::XMMatrixTranspose(DirectMatrix);
	}

public: // 역행렬
	float4x4 InverseReturn()
	{
		float4x4 ReturnMat = DirectX::XMMatrixInverse(nullptr, DirectMatrix);
		return ReturnMat;
	}

public: // 뷰포트
	void ViewPortCenter(float _ScreenX, float _ScreenY, float _StartX, float _StartY, float _MinZ, float _MaxZ)
	{
		Identity();

		Arr2D[0][0] = _ScreenX * 0.5f;
		Arr2D[1][1] = -_ScreenY * 0.5f;
		Arr2D[2][2] = _MaxZ - _MinZ;
		Arr2D[3][0] = _StartX + Arr2D[0][0];
		Arr2D[3][1] = _ScreenY * 0.5f + _StartY;
		Arr2D[3][2] = _MinZ;
	}

public: // 뷰행렬
	// 관측자가 어떠한 물체를 바라본다.
	void ViewAtLH(const float4& _EyePos, const float4& _EyeFocus, const float4& _EyeUp)
	{
		DirectMatrix = DirectX::XMMatrixLookAtLH(_EyePos.DirectVector, _EyeFocus.DirectVector, _EyeUp.DirectVector);
	}

	// 관측자가 어떠한 방향을 바라본다.
	void ViewToLH(const float4& _EyePos, const float4& _EyeDir, const float4& _EyeUp)
	{
		DirectMatrix = DirectX::XMMatrixLookToLH(_EyePos.DirectVector, _EyeDir.DirectVector, _EyeUp.DirectVector);
	}

public: // 투영행렬
	// 원근투영
	void PerspectiveFovLH(float _FovAngleY, float _Width, float _Height, float _NearZ, float _FarZ)
	{
		PerspectiveFovLH(_FovAngleY * GameEngineMath::DegreeToRadian, _Width / _Height, _NearZ, _FarZ);
	}

	void PerspectiveFovLH(float _FovAngleY, float _AspectRatio, float _Near, float _Far)
	{
		DirectMatrix = DirectX::XMMatrixPerspectiveFovLH(_FovAngleY, _AspectRatio, _Near, _Far);
	}

	// 직교투영
	void OrthographicLH(float _Width, float _Height, float _Near, float _Far)
	{
		DirectMatrix = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);
	}

public: // 월드행렬에서 크기(Scale)만 추출
	float4 ExtractScaling()
	{
		float4 ret = float4::ZERO;

		float Tolerance = 1.e-8f;

		const float SquareSum0 = (Arr2D[0][0] * Arr2D[0][0]) + (Arr2D[0][1] * Arr2D[0][1]) + (Arr2D[0][2] * Arr2D[0][2]);
		const float SquareSum1 = (Arr2D[1][0] * Arr2D[1][0]) + (Arr2D[1][1] * Arr2D[1][1]) + (Arr2D[1][2] * Arr2D[1][2]);
		const float SquareSum2 = (Arr2D[2][0] * Arr2D[2][0]) + (Arr2D[2][1] * Arr2D[2][1]) + (Arr2D[2][2] * Arr2D[2][2]);

		if (SquareSum0 > Tolerance)
		{
			float Scale0 = sqrtf(SquareSum0);
			ret.x = Scale0;
			float InvScale0 = 1.f / Scale0;
			Arr2D[0][0] *= InvScale0;
			Arr2D[0][1] *= InvScale0;
			Arr2D[0][2] *= InvScale0;
		}
		else
		{
			ret.x = 0;
		}

		if (SquareSum1 > Tolerance)
		{
			float Scale1 = sqrtf(SquareSum1);
			ret.y = Scale1;
			float InvScale1 = 1.f / Scale1;
			Arr2D[1][0] *= InvScale1;
			Arr2D[1][1] *= InvScale1;
			Arr2D[1][2] *= InvScale1;
		}
		else
		{
			ret.y = 0;
		}

		if (SquareSum2 > Tolerance)
		{
			float Scale2 = sqrtf(SquareSum2);
			ret.z = Scale2;
			float InvScale2 = 1.f / Scale2;
			Arr2D[2][0] *= InvScale2;
			Arr2D[2][1] *= InvScale2;
			Arr2D[2][2] *= InvScale2;
		}
		else
		{
			ret.z = 0;
		}

		return ret;
	}

public:
	inline float Determinant() const
	{
		return	Arr2D[0][0] * (
			Arr2D[1][1] * (Arr2D[2][2] * Arr2D[3][3] - Arr2D[2][3] * Arr2D[3][2]) -
			Arr2D[2][1] * (Arr2D[1][2] * Arr2D[3][3] - Arr2D[1][3] * Arr2D[3][2]) +
			Arr2D[3][1] * (Arr2D[1][2] * Arr2D[2][3] - Arr2D[1][3] * Arr2D[2][2])
			) -
			Arr2D[1][0] * (
				Arr2D[0][1] * (Arr2D[2][2] * Arr2D[3][3] - Arr2D[2][3] * Arr2D[3][2]) -
				Arr2D[2][1] * (Arr2D[0][2] * Arr2D[3][3] - Arr2D[0][3] * Arr2D[3][2]) +
				Arr2D[3][1] * (Arr2D[0][2] * Arr2D[2][3] - Arr2D[0][3] * Arr2D[2][2])
				) +
			Arr2D[2][0] * (
				Arr2D[0][1] * (Arr2D[1][2] * Arr2D[3][3] - Arr2D[1][3] * Arr2D[3][2]) -
				Arr2D[1][1] * (Arr2D[0][2] * Arr2D[3][3] - Arr2D[0][3] * Arr2D[3][2]) +
				Arr2D[3][1] * (Arr2D[0][2] * Arr2D[1][3] - Arr2D[0][3] * Arr2D[1][2])
				) -
			Arr2D[3][0] * (
				Arr2D[0][1] * (Arr2D[1][2] * Arr2D[2][3] - Arr2D[1][3] * Arr2D[2][2]) -
				Arr2D[1][1] * (Arr2D[0][2] * Arr2D[2][3] - Arr2D[0][3] * Arr2D[2][2]) +
				Arr2D[2][1] * (Arr2D[0][2] * Arr2D[1][3] - Arr2D[0][3] * Arr2D[1][2])
				);
	}
};
