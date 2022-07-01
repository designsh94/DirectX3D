#pragma once

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineFSM
{
private:
	class State 
	{
	public:
		std::string Name_;
		float Time;

	public:
		std::function<void()> Start_;
		std::function<void()> Update_;
		std::function<void()> End_;

		State(std::string _Name, std::function<void()> _Start, std::function<void()> _Update, std::function<void()> _End) :
			Name_(_Name),
			Time(),
			Start_(_Start),
			Update_(_Update),
			End_(_End)
		{
		}
	};

private:
	std::map<std::string, State*> AllState_;

private:
	State* Current_;
	State* Next_;

public:
	GameEngineFSM();
	~GameEngineFSM();

protected:
	GameEngineFSM(const GameEngineFSM& _Other) = delete;
	GameEngineFSM(GameEngineFSM&& _Other) noexcept = delete;

private:
	GameEngineFSM& operator=(const GameEngineFSM& _Other) = delete;
	GameEngineFSM& operator=(GameEngineFSM&& _Other) noexcept = delete;

public:
	inline bool IsCurrentState(const std::string& _Name) const
	{
		return Current_->Name_ == _Name;
	}

public:
	State* GetCurrentState()
	{
		return Current_;
	}

public:
	void CreateState(const std::string& _Name, std::function<void()> _Update, std::function<void()> _Start = nullptr, std::function<void()> _EndStart = nullptr, std::function<void()> _Init = nullptr);
	void ChangeState(const std::string& _Name);

public:
	void Update(float _Time);
};

