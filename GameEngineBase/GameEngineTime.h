#pragma once

struct TimeEvent
{
	float Time_;
	std::function<void()> Event_;
};

// 분류 : 델타타임 관리
// 용도 : 
// 설명 : DeltaTime 관리 및 리셋 기능 제공
class GameEngineTime
{
private:
	static GameEngineTime* Inst;


public:
	static GameEngineTime& GetInst()
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

private:
	LARGE_INTEGER timeCount_;
	LARGE_INTEGER startCheck_;
	LARGE_INTEGER endCheck_;
	double deltaTime_;

private:
	std::list<TimeEvent*> AllEvent_;
	std::list<TimeEvent*> AddEvent_;
	std::map<int, float> TimeScale_;

public:
	GameEngineTime();
	~GameEngineTime();

public:
	GameEngineTime(const GameEngineTime& _Other) = delete;
	GameEngineTime(const GameEngineTime&& _Other) noexcept;

public:
	GameEngineTime& operator=(const GameEngineTime& _Other) = delete;
	GameEngineTime& operator=(const GameEngineTime&& _Other) = delete;

public:
	template<typename EnumType>
	void SetTimeScale(EnumType _Index, float _Scale)
	{
		SetTimeScale(static_cast<int>(_Index), _Scale);
	}

	void SetTimeScale(int _Index, float _Scale)
	{
		if (TimeScale_.end() == TimeScale_.find(_Index))
		{
			TimeScale_[_Index] = 1.0f;
		}

		TimeScale_[_Index] = _Scale;
	}

	template<typename EnumType>
	float GetTimeScale(EnumType _Index)
	{
		return GetTimeScale(static_cast<int>(_Index));
	}

	float GetTimeScale(int _Index)
	{
		if (TimeScale_.end() == TimeScale_.find(_Index))
		{
			TimeScale_[_Index] = 1.0f;
		}

		return TimeScale_[_Index];
	}

public:
	double GetDeltaTimeD()
	{
		return deltaTime_;
	}

	float GetDeltaTime()
	{
		return static_cast<float>(deltaTime_);
	}

	float GetDeltaTime(int _Index)
	{
		if (TimeScale_.end() == TimeScale_.find(_Index))
		{
			TimeScale_[_Index] = 1.0f;
		}

		return static_cast<float>(deltaTime_) * TimeScale_[_Index];
	}

public:
	void TimeCheckReset();
	void TimeCheck();
	void AddTimeEvent(float _Time, std::function<void()> _Event);
};

