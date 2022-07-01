#include "PreCompile.h"
#include "GameEngineMesh.h"

GameEngineMesh::GameEngineMesh() :
	VertexBuffer_(nullptr),
	IndexBuffer_(nullptr)
{
}

GameEngineMesh::~GameEngineMesh()
{
}

GameEngineMesh::GameEngineMesh(GameEngineMesh&& _other) noexcept :
	VertexBuffer_(_other.VertexBuffer_),
	IndexBuffer_(_other.IndexBuffer_)
{
}
