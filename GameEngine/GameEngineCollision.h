#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineShaderResHelper.h"
#include "Enums.h"

// �з� : �浹
// �뵵 : �浹����
// ���� : 
class GameEngineCore;
class GameEngineLevel;
class GameEngineCollision : public GameEngineTransformComponent
{
	friend GameEngineCore;
	friend GameEngineLevel;

public:
	static std::function<bool(GameEngineTransform*, GameEngineTransform*)> CollisionCheckFunction[static_cast<int>(CollisionType::MAX)][static_cast<int>(CollisionType::MAX)];

public:
	static void Init();

public:
	static bool CirCleToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool Sphere3DToSphere3D(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool RectToRect(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool AABB3DToAABB3D(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool OrientedRectToOrientedRect(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool OBB3DToOBB3D(GameEngineTransform* _Left, GameEngineTransform* _Right);

public:
	static bool CirCleToRect(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool CirCleToOrientedRect(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool Sphere3DToAABBBox3D(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool Sphere3DToOBBBox3D(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool RectToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool RectToOrientedRect(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool AABBBox3DToSphere3D(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool AABBBox3DToOBBBox3D(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool OrientedRectToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool OrientedRectToRect(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool OBBBox3DToSphere3D(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool OBBBox3DToAABBBox3D(GameEngineTransform* _Left, GameEngineTransform* _Right);

private:
	bool Collision_;

public:
	GameEngineCollision();
	~GameEngineCollision();

protected:
	GameEngineCollision(const GameEngineCollision& _Other) = delete;
	GameEngineCollision(GameEngineCollision&& _Other) noexcept = delete;

private:
	GameEngineCollision& operator=(const GameEngineCollision& _Other) = delete;
	GameEngineCollision& operator=(GameEngineCollision&& _Other) noexcept = delete;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	template<typename UserType>
	void SetCollisionGroup(UserType _Type)
	{
		SetCollisionGroup(static_cast<int>(_Type));
	}

	void SetCollisionGroup(int _Type);

public:
	// �ش� ��ü�� ���ϴ� _OtherGroup���� �浹üũ �� �浹�� �浹ü�� CallbackFunction ȣ��
	void Collision(CollisionType _ThisType, CollisionType _OtherType, int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack, std::function<void(GameEngineCollision*)> _EndCallBack = nullptr);

public: // CollisionType �� �浹
	void SphereToSphereCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);
	void SphereToAABBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);
	void SphereToOBBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);

public:
	void AABBBoxToAABBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);
	void AABBBoxToSphereCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);
	void AABBBoxToOBBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);

public:
	void OBBBoxToOBBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);
	void OBBBoxToSphereCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);
	void OBBBoxToAABBBoxCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);
};

