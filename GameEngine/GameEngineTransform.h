#pragma once
#include "GameEngineComponent.h"

#include <DirectXCollision.h>
#include <DirectXCollision.inl>

union CollisionData
{
public:
	DirectX::BoundingSphere				Sphere;		// 구
	DirectX::BoundingBox				AABB;		// 회전이 고려하면 안되는 박스
	DirectX::BoundingOrientedBox		OBB;		// 회전한 박스

	CollisionData() : OBB()
	{
	}
};

// 각 물체의 지역 공간상의 위치, 크기, 회전 정보와 월드 공간상의 위치, 크기, 회전 정보를 통하여
// 생성하는 각각의 행렬을 관리하는 클래스
// 셋팅과 동시에 갱신하여 즉시 결과반영한다.
class TransformData 
{
public:
	// 월드 : 월드 공간에서의 원점(0,0,0)을 기준
	float4 vWorldPosition_;
	float4 vWorldRotation_;
	float4 vWorldScaling_;

	// 로컬 : 1. 부모 O : 부모의 위치(원점)을 기준
	//            2. 부모 X : 월드 공간에서의 원점(0,0,0)을 기준
	// Ex) A컴포넌트의 자식 컴포넌트가 B컴포넌트라고 가정한다면
	//        B컴포넌트의 로컬위치는 A컴포넌트의 위치(100, 0, 0)이라면
	//        A컴포넌트의 위치(100,0,0)로부터의 B컴포넌트의 위치가 설정된다
	//        A컴포넌트와 B컴포넌트가 겹쳐서 생성되었다면 A컴포넌트의 로컬위치는 최상위 컴포넌트이므로
	//        100,0,0이 로컬위치가 되고, B컴포넌트는 100,0,0이 원점이므로 위치는 0,0,0이 된다.
	float4 vLocalPosition_;
	float4 vLocalRotation_;
	float4 vLocalScaling_;

	// 변환행렬
	float4x4 LocalScaling_;
	float4x4 LocalRotation_;
	float4x4 LocalPosition_;
	float4x4 LocalWorld_;

	// 부모행렬
	float4x4 Parent_;

	// 월드행렬
	float4x4 WorldWorld_;

	// 뷰행렬
	float4x4 View_;

	// 투영행렬
	float4x4 Projection_;

	// 최종행렬
	float4x4 WV_;
	float4x4 WVP_;

public:
	TransformData() :
		vWorldScaling_(float4::ONE),
		vLocalScaling_(float4::ONE)
	{

	}

public:
	// 로컬행렬(크기/회전/위치/월드) 생성
	void LocalCalculation()
	{
		LocalScaling_.Scaling(vLocalScaling_);
		LocalRotation_.RotationDeg(vLocalRotation_);
		LocalPosition_.Translation(vLocalPosition_);

		LocalWorld_ = LocalScaling_ * LocalRotation_ * LocalPosition_;
	}

	// 부모셋팅 : 부모가 존재한다면 모든 행렬이 계산될때 부모의 행렬의 영향을 받는다.
	void ParentSetting(const float4x4& _Parent) 
	{
		Parent_ = _Parent;
		WorldWorld_ = LocalWorld_;
		WorldWorld_ *= Parent_;
	}

	// 자신의 부모가 존재하지 않는다.
	// 자신이 최상위 트랜스폼이므로 부모의 영향을 받지않는다.
	void RootCalculation()
	{
		WorldWorld_ = LocalWorld_;
	}

	// 최종행렬을 계산
	void WVPCalculation()
	{
		WV_ = WorldWorld_ * View_;
		WVP_ = WorldWorld_ * View_ * Projection_;
	}
};

// 분류 : 트랜스폼(Transform)
// 용도 : 
// 설명 : 각 물체의 지역좌표상의 위치, 크기, 회전 정보를 정의하는 기능을 제공하고 이를 월드상의 좌표(World Space)로 변환하는 기능을 제공하는 클래스
//            단, 각 물체는 부모 물체를 가지고 있는 계층구조(부모-자식)를 이루며 사용자의 선택에 따라 부모의 영향을 받아 위치/크기/회전 정보를 계산하는 경우도 있다.
class GameEngineTransform 
{
protected:
	TransformData TransformData_;
	GameEngineTransform* Parent_;
	std::vector<GameEngineTransform*> Childs_;

protected:
	CollisionData ColData_;

public:
	GameEngineTransform();
	~GameEngineTransform();

public:
	GameEngineTransform& operator=(const GameEngineTransform& _Other) = delete;
	GameEngineTransform& operator=(GameEngineTransform&& _Other) noexcept = delete;

private:
	void AllChildCalculationScaling();
	void AllChildCalculationRotation();
	void AllChildCalculationPosition();

	// 크기행렬
	void CalculationLocalScaling();
	void CalculationWorldScaling();

	// 회전행렬
	void CalculationWorldRotation();
	void CalculationLocalRotation();

	// 이동행렬
	void CalculationLocalPosition();
	void CalculationWorldPosition();

public:
	inline TransformData& GetTransformData()
	{
		return TransformData_;
	}

	inline const CollisionData& GetCollisionData()
	{
		return ColData_;
	}

	inline const DirectX::BoundingSphere& GetSphere()
	{
		return ColData_.Sphere;
	}

	inline const DirectX::BoundingOrientedBox& GetOBB()
	{
		return ColData_.OBB;
	}

	inline const DirectX::BoundingBox& GetAABB()
	{
		return ColData_.AABB;
	}

public:
	inline void AddLocalDeltaTimeRotation(const float4& _Value)
	{
		SetLocalRotationDegree(TransformData_.vLocalRotation_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

	inline void AddWorldDeltaTimeRotation(const float4& _Value)
	{
		SetWorldRotationDegree(TransformData_.vWorldRotation_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

	inline void SetLocalDeltaTimeMove(const float4& _Value)
	{
		SetLocalPosition(TransformData_.vLocalPosition_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

	inline void SetWorldDeltaTimeMove(const float4& _Value)
	{
		SetWorldPosition(TransformData_.vWorldPosition_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

public:
	inline void SetLocalMove(const float4& _Value)
	{
		SetLocalPosition(TransformData_.vLocalPosition_ + _Value);
	}

	inline void SetWorldMove(const float4& _Value)
	{
		SetWorldPosition(TransformData_.vWorldPosition_ + _Value);
	}

public:
	void DetachChildTransform(GameEngineTransform* _Child);
	void AttachTransform(GameEngineTransform* _Transform);

public:
	void SetLocalScaling(const float4& _Value);
	void SetWorldScaling(const float4& _Value);
	void SetLocalRotationDegree(const float4& _Value);
	void SetWorldRotationDegree(const float4& _Value);
	void SetLocalPosition(const float4& _Value);
	void SetWorldPosition(const float4& _Value);

public:
	void AddLocalRotationDegreeX(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.x += _Value;
		SetLocalRotationDegree(Local);
	}

	void AddWorldRotationDegreeX(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.x += _Value;
		SetWorldRotationDegree(Local);
	}

	void AddLocalRotationDegreeY(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.y += _Value;
		SetLocalRotationDegree(Local);
	}

	void AddWorldRotationDegreeY(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.y += _Value;
		SetWorldRotationDegree(Local);
	}

	void AddLocalRotationDegreeZ(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.z += _Value;
		SetLocalRotationDegree(Local);
	}

	void AddWorldRotationDegreeZ(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.z += _Value;
		SetWorldRotationDegree(Local);
	}

public:
	void Copy(const GameEngineTransform& _Other);

public:
	float4 GetLocalScaling() { return TransformData_.vLocalScaling_; }
	float4 GetWorldScaling() { return TransformData_.vWorldScaling_; }
	float4 GetLocalRotation() { return TransformData_.vLocalRotation_; }
	float4 GetWorldRotation() { return TransformData_.vWorldRotation_; }
	float4 GetLocalPosition() { return TransformData_.vLocalPosition_; }
	float4 GetWorldPosition() { return TransformData_.vWorldPosition_; }

public:
	float4 GetWorldForwardVector() { return TransformData_.WorldWorld_.vz.NormalizeReturn3D(); }
	float4 GetLocalForwardVector() { return TransformData_.LocalWorld_.vz.NormalizeReturn3D(); }
	float4 GetWorldBackVector() { return -TransformData_.WorldWorld_.vz.NormalizeReturn3D(); }
	float4 GetLocalBackVector() { return -TransformData_.LocalWorld_.vz.NormalizeReturn3D(); }
	float4 GetWorldRightVector() { return TransformData_.WorldWorld_.vx.NormalizeReturn3D(); }
	float4 GetLocalRightVector() { return TransformData_.LocalWorld_.vx.NormalizeReturn3D(); }
	float4 GetWorldLeftVector() { return -TransformData_.WorldWorld_.vx.NormalizeReturn3D(); }
	float4 GetLocalLeftVector() { return -TransformData_.LocalWorld_.vx.NormalizeReturn3D(); }
	float4 GetWorldUpVector() { return TransformData_.WorldWorld_.vy.NormalizeReturn3D(); }
	float4 GetLocalUpVector() { return TransformData_.LocalWorld_.vy.NormalizeReturn3D(); }
	float4 GetWorldDownVector() { return -TransformData_.WorldWorld_.vy.NormalizeReturn3D(); }
	float4 GetLocalDownVector() { return -TransformData_.LocalWorld_.vy.NormalizeReturn3D(); }

public:
	void TransformUpdate();
};

