#include "PreCompile.h"
#include "GameEnginePathFind.h"

GameEnginePathFind::GameEnginePathFind() :
	PoolCount_(0)
{
}

GameEnginePathFind::~GameEnginePathFind()
{
}

std::list<PathIndex> GameEnginePathFind::CalReturn(AStarNode* _EndNode)
{
	std::list<PathIndex> ReturnList;

	// End ~ Start�������� �θ� Ž���Ͽ� ��θ���� ��ȯ
	AStarNode* FindNode = _EndNode;
	while (nullptr != FindNode->Parent_)
	{
		ReturnList.push_front(FindNode->Index_);

		// �θ��ε����� ����
		FindNode = FindNode->Parent_;
	}
	return ReturnList;
}

AStarNode* GameEnginePathFind::CreateNode(PathIndex _Index, PathIndex End, AStarNode* _ParentNode)
{
	if (PoolCount_ >= NodePool_.size())
	{
		return nullptr;
	}

	AStarNode* NewNode = &NodePool_[PoolCount_++];
	NewNode->Reset();
	NewNode->Index_ = _Index;
	NewNode->CalLen(End);
	return NewNode;
}

std::list<PathIndex> GameEnginePathFind::AStarFind4Way(PathIndex _Start, PathIndex _End, std::function<bool(PathIndex)> _OpenFunction, int _Maximum)
{
	PathIndex ArrDir[4] = { {1, 0 }, {-1, 0}, {0, 1}, {0, -1} };

	// Clear
	PoolCount_ = 0;
	NodePool_.resize(_Maximum);
	OpenList_.clear();
	OpenKeys_.clear();
	CloseList_.clear();
	CloseKeys_.clear();

	// ���۳�� ����
	AStarNode* NewNode = CreateNode(_Start, _End, nullptr);
	OpenList_.insert(std::make_pair(NewNode->TotalLen_, NewNode));
	OpenKeys_.insert(NewNode->Index_.Index_);

	// ���Ž�� ����
	while (0 != OpenList_.size())
	{
		// �޸� �ִ�ġ Ž���� �Ͼ�ٸ� ��θ� ã������
		if (PoolCount_ >= NodePool_.size())
		{
			return std::list<PathIndex>();
		}

		// ������� ��Ͽ��� ���۳�带 Get
		AStarNode* FirstNode = OpenList_.begin()->second;

		// ������� ��Ͽ��� �ش� ��带 ����
		OpenList_.erase(OpenList_.begin());
		OpenKeys_.erase(FirstNode->Index_.Index_);

		// ������� ��Ͽ� �ش� ��� �߰�
		CloseList_.push_back(FirstNode);
		CloseKeys_.insert(FirstNode->Index_.Index_);

		// �ش� ����� 4���� �̿������� üũ
		for (size_t i = 0; i < 4; i++)
		{
			PathIndex FindIndex = FirstNode->Index_ + ArrDir[i];

			// ��������Ͽ� �ش� ��尡 �����ϸ� continue
			if (OpenKeys_.end() != OpenKeys_.find(FindIndex.Index_))
			{
				continue;
			}

			// ��������Ͽ� �ش� ��尡 �����ϸ� continue
			if (CloseKeys_.end() != CloseKeys_.find(FindIndex.Index_))
			{
				continue;
			}

			// ��������ǿ� ���Ͽ� �̵��Ұ������̶�� continue
			if (true == _OpenFunction(FindIndex))
			{
				continue;
			}

			// ��� ������ �����Ͽ� �̵����� ��ζ��
			// ���۳���� �̿��ϴ� ����̹Ƿ� ���۳�尡 �θ���� ����
			AStarNode* NewNode = CreateNode(FindIndex, _End, FirstNode);
			if (nullptr != NewNode)
			{
				// �̿���� 4����˻�� ���� ���õ� ��带 �θ�� ����
				NewNode->Parent_ = FirstNode;

				// ��ǥ ��忡 �����ߴٸ� �̵���� ��ȯ
				if (_End == FindIndex)
				{
					return CalReturn(NewNode);
				}

				// �ƴ϶�� ��������Ͽ� �ش� ��� �߰�
				OpenList_.insert(std::make_pair(NewNode->TotalLen_, NewNode));
				OpenKeys_.insert(FirstNode->Index_.Index_);
			}
		}
	}

	return std::list<PathIndex>();
}

std::list<PathIndex> GameEnginePathFind::AStarFind8Way(PathIndex _Start, PathIndex _End, std::function<bool(PathIndex)> _OpenFunction, int _Maximum)
{
	PathIndex ArrDir[8] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1},
							{1, 1}, {-1, 1}, {-1, -1}, {1, -1} };

	// Clear
	PoolCount_ = 0;
	NodePool_.resize(_Maximum);
	OpenList_.clear();
	OpenKeys_.clear();
	CloseList_.clear();
	CloseKeys_.clear();

	// ���۳�� ����
	AStarNode* NewNode = CreateNode(_Start, _End, nullptr);
	OpenList_.insert(std::make_pair(NewNode->TotalLen_, NewNode));
	OpenKeys_.insert(NewNode->Index_.Index_);

	// ���Ž�� ����
	while (0 != OpenList_.size())
	{
		// �޸� �ִ�ġ Ž���� �Ͼ�ٸ� ��θ� ã������
		if (PoolCount_ >= NodePool_.size())
		{
			return std::list<PathIndex>();
		}

		// ������� ��Ͽ��� ���۳�带 Get
		AStarNode* FirstNode = OpenList_.begin()->second;

		// ������� ��Ͽ��� �ش� ��带 ����
		OpenList_.erase(OpenList_.begin());
		OpenKeys_.erase(FirstNode->Index_.Index_);

		// ������� ��Ͽ� �ش� ��� �߰�
		CloseList_.push_back(FirstNode);
		CloseKeys_.insert(FirstNode->Index_.Index_);

		// �ش� ����� 4���� �̿������� üũ
		for (size_t i = 0; i < 8; i++)
		{
			PathIndex FindIndex = FirstNode->Index_ + ArrDir[i];

			// ��������Ͽ� �ش� ��尡 �����ϸ� continue
			if (OpenKeys_.end() != OpenKeys_.find(FindIndex.Index_))
			{
				continue;
			}

			// ��������Ͽ� �ش� ��尡 �����ϸ� continue
			if (CloseKeys_.end() != CloseKeys_.find(FindIndex.Index_))
			{
				continue;
			}

			// ��������ǿ� ���Ͽ� �̵��Ұ������̶�� continue
			if (true == _OpenFunction(FindIndex))
			{
				continue;
			}

			// ��� ������ �����Ͽ� �̵����� ��ζ��
			// ���۳���� �̿��ϴ� ����̹Ƿ� ���۳�尡 �θ���� ����
			AStarNode* NewNode = CreateNode(FindIndex, _End, FirstNode);
			if (nullptr != NewNode)
			{
				// �̿���� 8����˻�� ���� ���õ� ��带 �θ�� ����
				NewNode->Parent_ = FirstNode;

				// ��ǥ ��忡 �����ߴٸ� �̵���� ��ȯ
				if (_End == FindIndex)
				{
					return CalReturn(NewNode);
				}

				// �ƴ϶�� ��������Ͽ� �ش� ��� �߰�
				OpenList_.insert(std::make_pair(NewNode->TotalLen_, NewNode));
				OpenKeys_.insert(FirstNode->Index_.Index_);
			}
		}
	}

	return std::list<PathIndex>();
}
