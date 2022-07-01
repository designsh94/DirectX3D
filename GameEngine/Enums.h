#pragma once

enum class CollisionType
{
	// 2D Collider Type
	Point2D,
	CirCle,
	Rect,
	OrientedRect,

	// 3D Collider Type
	Point3D,
	Sphere3D,
	AABBBox3D,
	OBBBox3D,
	MAX,
};

enum class RenderingOrder
{
	BackBuffer,
	World,
	UI,
	Debug,
};
