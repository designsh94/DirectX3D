#pragma once
#include "GameEngineMath.h"

class PathIndex
{
public:
	union
	{
		struct
		{
			int X_;
			int Y_;
		};

		__int64 Index_;
	};

public:
	PathIndex operator + (const PathIndex& _Other)
	{
		return { _Other.X_ + X_, _Other.Y_ + Y_ };
	}

	bool operator == (const PathIndex& _Other)
	{
		return _Other.X_ == X_ && _Other.Y_ == Y_;
	}

	bool operator != (const PathIndex& _Other)
	{
		return _Other.X_ != X_ || _Other.Y_ != Y_;
	}

public:
	PathIndex() :
		X_(0), Y_(0)
	{
	}

	PathIndex(int _X, int _Y) :
		X_(_X), Y_(_Y)
	{
	}
};

class AStarNode
{
public:
	PathIndex Index_;
	AStarNode* Parent_;
	float PathLen_;
	float EndLen_;
	float TotalLen_;

public:
	void CalLen(PathIndex _EndIndex)
	{
		if (nullptr == Parent_)
		{
			PathLen_ = 0.0f;
		}
		else
		{
			float4 ParentIndex = float4(static_cast<float>(Parent_->Index_.X_), static_cast<float>(Parent_->Index_.Y_));
			float4 ThisIndex = float4(static_cast<float>(Index_.X_), static_cast<float>(Index_.Y_));
			PathLen_ = Parent_->PathLen_ + (ParentIndex - ThisIndex).Len2D();
		}

		float4 EndIndex = float4(static_cast<float>(_EndIndex.X_), static_cast<float>(_EndIndex.Y_));
		float4 ThisIndex = float4(static_cast<float>(Index_.X_), static_cast<float>(Index_.Y_));
		EndLen_ = (EndIndex - ThisIndex).Len2D();

		TotalLen_ = PathLen_ + EndLen_;
	}

	void Reset()
	{
		memset(this, 0x00, sizeof(*this));
	}
};

// 분류 : 
// 용도 : 
// 설명 : 
class GameEnginePathFind
{
private:
	int PoolCount_;
	std::vector<AStarNode> NodePool_;

private:
	std::multimap<float, AStarNode*> OpenList_;
	std::set<__int64> OpenKeys_;

private:
	std::list<AStarNode*> CloseList_;
	std::set<__int64> CloseKeys_;

public:
	GameEnginePathFind();
	~GameEnginePathFind();

protected:
	GameEnginePathFind(const GameEnginePathFind& _other) = delete;
	GameEnginePathFind(GameEnginePathFind&& _other) noexcept = delete;

private:
	GameEnginePathFind& operator=(const GameEnginePathFind& _other) = delete;
	GameEnginePathFind& operator=(const GameEnginePathFind&& _other) = delete;

private:
	std::list<PathIndex> CalReturn(AStarNode* _EndNode);

protected:
	AStarNode* CreateNode(PathIndex _Index, PathIndex End, AStarNode* _ParentNode = nullptr);

public:
	std::list<PathIndex> AStarFind4Way(PathIndex Start, PathIndex End, std::function<bool(PathIndex)> _OpenFunction, int _Maximum = 1000);	// 이웃노드(4방향탐색)
	std::list<PathIndex> AStarFind8Way(PathIndex Start, PathIndex End, std::function<bool(PathIndex)> _OpenFunction, int _Maximum = 1000);	// 이웃노드(8방향탐색)
};

