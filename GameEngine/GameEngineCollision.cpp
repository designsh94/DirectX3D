#include "PreCompile.h"
#include "GameEngineCollision.h"
#include "GameEngineLevel.h"
#include "GameEngineTransform.h"

std::function<bool(GameEngineTransform*, GameEngineTransform*)> GameEngineCollision::CollisionCheckFunction[static_cast<int>(CollisionType::MAX)][static_cast<int>(CollisionType::MAX)];

void GameEngineCollision::Init()
{
	// CollisionType::Sphere To CollisionType::Sphere
	CollisionCheckFunction[static_cast<int>(CollisionType::CirCle)][static_cast<int>(CollisionType::CirCle)] = std::bind(&GameEngineCollision::CirCleToCirCle, std::placeholders::_1, std::placeholders::_2);
	CollisionCheckFunction[static_cast<int>(CollisionType::Sphere3D)][static_cast<int>(CollisionType::Sphere3D)] = std::bind(&GameEngineCollision::Sphere3DToSphere3D, std::placeholders::_1, std::placeholders::_2);

	// GameEngineCollision::AABBBox To GameEngineCollision::AABBBox
	CollisionCheckFunction[static_cast<int>(CollisionType::Rect)][static_cast<int>(CollisionType::Rect)] = std::bind(&GameEngineCollision::RectToRect, std::placeholders::_1, std::placeholders::_2);
	CollisionCheckFunction[static_cast<int>(CollisionType::AABBBox3D)][static_cast<int>(CollisionType::AABBBox3D)] = std::bind(&GameEngineCollision::AABB3DToAABB3D, std::placeholders::_1, std::placeholders::_2);

	// CollisionType::OBBBox To CollisionType::OBBBox
	CollisionCheckFunction[static_cast<int>(CollisionType::OrientedRect)][static_cast<int>(CollisionType::OrientedRect)] = std::bind(&GameEngineCollision::OrientedRectToOrientedRect, std::placeholders::_1, std::placeholders::_2);
	CollisionCheckFunction[static_cast<int>(CollisionType::OBBBox3D)][static_cast<int>(CollisionType::OBBBox3D)] = std::bind(&GameEngineCollision::OBB3DToOBB3D, std::placeholders::_1, std::placeholders::_2);

	// ============================================================ OtherCollisionType CollisionCheckFunction ============================================================ //
	// CollisionType::Sphere To OtherType
	CollisionCheckFunction[static_cast<int>(CollisionType::CirCle)][static_cast<int>(CollisionType::Rect)] = std::bind(&GameEngineCollision::CirCleToRect, std::placeholders::_1, std::placeholders::_2);
	CollisionCheckFunction[static_cast<int>(CollisionType::CirCle)][static_cast<int>(CollisionType::OrientedRect)] = std::bind(&GameEngineCollision::CirCleToOrientedRect, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::Sphere3D)][static_cast<int>(CollisionType::AABBBox3D)] = std::bind(&GameEngineCollision::Sphere3DToAABBBox3D, std::placeholders::_1, std::placeholders::_2);
	CollisionCheckFunction[static_cast<int>(CollisionType::Sphere3D)][static_cast<int>(CollisionType::OBBBox3D)] = std::bind(&GameEngineCollision::Sphere3DToOBBBox3D, std::placeholders::_1, std::placeholders::_2);

	// CollisionType::AABBBox To OtherType
	CollisionCheckFunction[static_cast<int>(CollisionType::Rect)][static_cast<int>(CollisionType::CirCle)] = std::bind(&GameEngineCollision::RectToCirCle, std::placeholders::_1, std::placeholders::_2);
	CollisionCheckFunction[static_cast<int>(CollisionType::Rect)][static_cast<int>(CollisionType::OrientedRect)] = std::bind(&GameEngineCollision::RectToOrientedRect, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::AABBBox3D)][static_cast<int>(CollisionType::Sphere3D)] = std::bind(&GameEngineCollision::AABBBox3DToSphere3D, std::placeholders::_1, std::placeholders::_2);
	CollisionCheckFunction[static_cast<int>(CollisionType::AABBBox3D)][static_cast<int>(CollisionType::OBBBox3D)] = std::bind(&GameEngineCollision::AABBBox3DToOBBBox3D, std::placeholders::_1, std::placeholders::_2);

	// CollisionType::OBBBox To OtherType
	CollisionCheckFunction[static_cast<int>(CollisionType::OrientedRect)][static_cast<int>(CollisionType::CirCle)] = std::bind(&GameEngineCollision::OrientedRectToCirCle, std::placeholders::_1, std::placeholders::_2);
	CollisionCheckFunction[static_cast<int>(CollisionType::OrientedRect)][static_cast<int>(CollisionType::Rect)] = std::bind(&GameEngineCollision::OrientedRectToRect, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::OBBBox3D)][static_cast<int>(CollisionType::Sphere3D)] = std::bind(&GameEngineCollision::OBBBox3DToSphere3D, std::placeholders::_1, std::placeholders::_2);
	CollisionCheckFunction[static_cast<int>(CollisionType::OBBBox3D)][static_cast<int>(CollisionType::AABBBox3D)] = std::bind(&GameEngineCollision::OBBBox3DToAABBBox3D, std::placeholders::_1, std::placeholders::_2);
}

// ========================================= SameCollisionType 충돌체크 ========================================= //
bool GameEngineCollision::CirCleToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingSphere Left = _Left->GetSphere();
	DirectX::BoundingSphere Right = _Right->GetSphere();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::Sphere3DToSphere3D(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return _Left->GetSphere().Intersects(_Right->GetSphere());
}

bool GameEngineCollision::RectToRect(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingBox Left = _Left->GetAABB();
	DirectX::BoundingBox Right = _Right->GetAABB();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::AABB3DToAABB3D(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return _Left->GetAABB().Intersects(_Right->GetAABB());
}

bool GameEngineCollision::OrientedRectToOrientedRect(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingOrientedBox Left = _Left->GetOBB();
	DirectX::BoundingOrientedBox Right = _Right->GetOBB();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

// ========================================= OtherCollisionType 충돌체크 ========================================= //
bool GameEngineCollision::OBB3DToOBB3D(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return _Left->GetOBB().Intersects(_Right->GetOBB());
}

bool GameEngineCollision::CirCleToRect(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingSphere Left = _Left->GetSphere();
	DirectX::BoundingBox Right = _Right->GetAABB();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::CirCleToOrientedRect(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingSphere Left = _Left->GetSphere();
	DirectX::BoundingOrientedBox Right = _Right->GetOBB();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::Sphere3DToAABBBox3D(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return _Left->GetSphere().Intersects(_Right->GetAABB());
}

bool GameEngineCollision::Sphere3DToOBBBox3D(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return _Left->GetSphere().Intersects(_Right->GetOBB());
}

bool GameEngineCollision::RectToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingBox Left = _Left->GetAABB();
	DirectX::BoundingSphere Right = _Right->GetSphere();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::RectToOrientedRect(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingBox Left = _Left->GetAABB();
	DirectX::BoundingOrientedBox Right = _Right->GetOBB();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::AABBBox3DToSphere3D(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return _Left->GetAABB().Intersects(_Right->GetSphere());
}

bool GameEngineCollision::AABBBox3DToOBBBox3D(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return _Left->GetAABB().Intersects(_Right->GetOBB());
}

bool GameEngineCollision::OrientedRectToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingOrientedBox Left = _Left->GetOBB();
	DirectX::BoundingSphere Right = _Right->GetSphere();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::OrientedRectToRect(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingOrientedBox Left = _Left->GetOBB();
	DirectX::BoundingBox Right = _Right->GetAABB();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::OBBBox3DToSphere3D(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return _Left->GetOBB().Intersects(_Right->GetSphere());
}

bool GameEngineCollision::OBBBox3DToAABBBox3D(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return _Left->GetOBB().Intersects(_Right->GetAABB());
}


GameEngineCollision::GameEngineCollision() :
	Collision_(false)
{
}

GameEngineCollision::~GameEngineCollision()
{
}

void GameEngineCollision::Start()
{
	GetLevel()->PushCollision(this, GetOrder());
}

void GameEngineCollision::Update(float _DeltaTime)
{
}

void GameEngineCollision::SetCollisionGroup(int _Type)
{
	GetLevel()->ChangeCollisionGroup(_Type, this);
}

void GameEngineCollision::Collision(CollisionType _ThisType, CollisionType _OtherType, int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack, std::function<void(GameEngineCollision*)> _EndCallBack)
{
	std::list<GameEngineCollision*>& Group = GetLevel()->GetCollisionGroup(_OtherGroup);
	for (GameEngineCollision* OtherCollision : Group)
	{
		if (false == OtherCollision->IsUpdate())
		{
			// 충돌중상태 -> 충돌종료상태
			if (nullptr != _EndCallBack)
			{
				if (true == OtherCollision->Collision_)
				{
					_EndCallBack(OtherCollision);
					OtherCollision->Collision_ = false;
				}
			}
			continue;
		}

		auto& CheckFunction = CollisionCheckFunction[static_cast<int>(_ThisType)][static_cast<int>(_OtherType)];

		if (nullptr == CheckFunction)
		{
			GameEngineDebug::MsgBoxError("아직 구현하지 않는 타입간에 충돌을 하려고 했습니다.");
		}

		if (false == CheckFunction(GetTransform(), OtherCollision->GetTransform()))
		{
			// 충돌중상태 -> 충돌종료상태
			if (nullptr != _EndCallBack)
			{
				if (true == OtherCollision->Collision_)
				{
					_EndCallBack(OtherCollision);
					OtherCollision->Collision_ = false;
				}
			}
			continue;
		}

		_CallBack(OtherCollision);
		OtherCollision->Collision_ = true;
	}
}

void GameEngineCollision::SphereToSphereCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack)
{
	Collision(CollisionType::Sphere3D, CollisionType::Sphere3D, _OtherGroup, _CallBack);
}

void GameEngineCollision::SphereToAABBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack)
{
	Collision(CollisionType::Sphere3D, CollisionType::AABBBox3D, _OtherGroup, _CallBack);
}

void GameEngineCollision::SphereToOBBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack)
{
	Collision(CollisionType::Sphere3D, CollisionType::OBBBox3D, _OtherGroup, _CallBack);
}

void GameEngineCollision::AABBBoxToAABBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack)
{
	Collision(CollisionType::AABBBox3D, CollisionType::AABBBox3D, _OtherGroup, _CallBack);
}

void GameEngineCollision::AABBBoxToSphereCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack)
{
	Collision(CollisionType::AABBBox3D, CollisionType::Sphere3D, _OtherGroup, _CallBack);
}

void GameEngineCollision::AABBBoxToOBBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack)
{
	Collision(CollisionType::AABBBox3D, CollisionType::OBBBox3D, _OtherGroup, _CallBack);
}

void GameEngineCollision::OBBBoxToOBBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack)
{
	Collision(CollisionType::OBBBox3D, CollisionType::OBBBox3D, _OtherGroup, _CallBack);
}

void GameEngineCollision::OBBBoxToSphereCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack)
{
	Collision(CollisionType::OBBBox3D, CollisionType::Sphere3D, _OtherGroup, _CallBack);
}

void GameEngineCollision::OBBBoxToAABBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack)
{
	Collision(CollisionType::OBBBox3D, CollisionType::AABBBox3D, _OtherGroup, _CallBack);
}
