#pragma once
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineMesh : public GameEngineObjectNameBase
{
private:
	GameEngineVertexBuffer* VertexBuffer_;
	GameEngineIndexBuffer* IndexBuffer_;

public:
	GameEngineMesh();
	~GameEngineMesh();

protected:
	GameEngineMesh(const GameEngineMesh& _other) = delete;
	GameEngineMesh(GameEngineMesh&& _other) noexcept;

private:
	GameEngineMesh& operator=(const GameEngineMesh& _other) = delete;
	GameEngineMesh& operator=(const GameEngineMesh&& _other) = delete;

public:
};

