#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase\GameEngineRandom.h>

// Ό³Έν :
class GameEngineImageRenderer;
class Map : public GameEngineActor
{
public:
	// constrcuter destructer
	Map();
	~Map();

	// delete Function
	Map(const Map& _Other) = delete;
	Map(Map&& _Other) noexcept = delete;
	Map& operator=(const Map& _Other) = delete;
	Map& operator=(Map&& _Other) noexcept = delete;

protected:

	void Start() override;
};