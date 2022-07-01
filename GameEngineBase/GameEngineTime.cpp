#include "PreCompile.h"
#include "GameEngineTime.h"

GameEngineTime* GameEngineTime::Inst = new GameEngineTime();

GameEngineTime::GameEngineTime()
{
	TimeCheckReset();
}   

GameEngineTime::~GameEngineTime()
{
	for (auto& Event : AddEvent_)
	{
		delete Event;
	}
	AddEvent_.clear();

	for (auto& Event : AllEvent_)
	{
		delete Event;
	}
	AllEvent_.clear();
}

GameEngineTime::GameEngineTime(const GameEngineTime&& _Other) noexcept :
	timeCount_(_Other.timeCount_),
	startCheck_(_Other.startCheck_),
	endCheck_(_Other.endCheck_),
	deltaTime_(_Other.deltaTime_)
{
}

void GameEngineTime::TimeCheckReset()
{
	QueryPerformanceFrequency(&timeCount_);
	QueryPerformanceCounter(&startCheck_);
	QueryPerformanceCounter(&endCheck_);
}

void GameEngineTime::TimeCheck()
{
	QueryPerformanceCounter(&endCheck_);
	deltaTime_ = static_cast<double>((endCheck_.QuadPart - startCheck_.QuadPart)) / static_cast<double>(timeCount_.QuadPart);
	startCheck_.QuadPart = endCheck_.QuadPart;

	for (auto& Event : AddEvent_)
	{
		AllEvent_.push_back(Event);
	}
	AddEvent_.clear();

	for (auto& Event : AllEvent_)
	{
		Event->Time_ -= static_cast<float>(deltaTime_);
		if (0 >= Event->Time_)
		{
			Event->Event_();
		}
	}

	std::list<TimeEvent*>::iterator StartIter = AllEvent_.begin();
	std::list<TimeEvent*>::iterator EndIter = AllEvent_.end();
	for (; StartIter != EndIter; )
	{
		if (0 >= (*StartIter)->Time_)
		{
			delete *StartIter;
			StartIter = AllEvent_.erase(StartIter);
			continue;
		}
		++StartIter;
	}
}

void GameEngineTime::AddTimeEvent(float _Time, std::function<void()> _Event) 
{
	AddEvent_.push_back(new TimeEvent{ _Time, _Event });
}
