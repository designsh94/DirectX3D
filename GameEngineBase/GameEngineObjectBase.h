#pragma once

// 분류 : 오브젝트 관리
// 용도 : 
// 설명 : 오브젝트의 기본적인 기능을 제공하며 관리
class GameEngineObjectBase
{
private:
	bool isDeath_;
	bool isUpdate_;
	bool isDebug_;
	int Order_;
	bool isOrigin_;
	GameEngineObjectBase* parent_;

public:
	GameEngineObjectBase();
	virtual ~GameEngineObjectBase();

protected:
	GameEngineObjectBase(const GameEngineObjectBase& _other);
	GameEngineObjectBase(GameEngineObjectBase&& _other) noexcept;

private:
	GameEngineObjectBase& operator=(const GameEngineObjectBase& _other) = delete;
	GameEngineObjectBase& operator=(const GameEngineObjectBase&& _other) = delete;

public:
	bool IsNull()
	{
		return nullptr == this;
	}

	void SetParent(GameEngineObjectBase* _parent)
	{
		parent_ = _parent;
	}

public:
	bool IsDeath()
	{
		if (nullptr == parent_)
		{
			return isDeath_;
		}

		return true == isDeath_ || parent_->isDeath_;
	}

	bool IsUpdate()
	{
		if (nullptr == parent_)
		{
			return false == IsDeath() && isUpdate_;
		}

		return false == IsDeath() && true == isUpdate_ && parent_->isUpdate_;
	}

	void CloneOn()
	{
		isOrigin_ = false;
	}

	bool IsClone()
	{
		return isOrigin_ == false;
	}

	void Death()
	{
		isDeath_ = true;
	}

	void OnOffChange()
	{
		isUpdate_ = !isUpdate_;
	}

	void On()
	{
		isUpdate_ = true;
	}

	void Off()
	{
		isUpdate_ = false;
	}

	void DebugOn()
	{
		isDebug_ = true;
	}

	void DebugOff()
	{
		isDebug_ = false;
	}

	bool IsDebugCheck()
	{
		if (true == isDebug_)
		{
		}

		return isDebug_;
	}

	void SetOrder(int _Order)
	{
		Order_ = _Order;
	}

	int GetOrder()
	{
		return Order_;
	}

	bool& GetIsUpdateRef()
	{
		return isUpdate_;
	}
};

