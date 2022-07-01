#pragma once
#include "Enums.h"

// 분류 : 
// 용도 : 
// 설명 : 
class CameraActor;
class CameraComponent;
class GameEngineActor;
class GameEngineRenderer;
class GameEngineTransform;
class GameEngineCollision;
class GameEngineDebugRenderData;
class GameEnginePostProcessRender;
class GameEngineLevel : public GameEngineObjectNameBase
{
	friend class GameEngineLevelControlWindow;
	friend class GameEngineCore;
	friend class GameEngineRenderer;
	friend class GameEngineCollision;

private:
	class NextLevelActor
	{
	public:
		GameEngineActor* Actor;
		GameEngineLevel* Level;
	};

private:
	std::map<int, std::list<GameEngineActor*>> ActorList_;
	std::vector<NextLevelActor> NextLevelActorsData_;
	std::map<int, std::list<GameEngineCollision*>> CollisionList_;
	std::map<std::string, std::vector<GameEnginePostProcessRender*>> PostRender;

private:
	std::list<TimeEvent*> AllEvent_;
	std::list<TimeEvent*> AddEvent_;

private:
	CameraActor* MainCameraActor_;
	CameraActor* UICameraActor_;

public:
	GameEngineLevel();
	~GameEngineLevel();

protected:
	GameEngineLevel(const GameEngineLevel& _Other) = delete;
	GameEngineLevel(GameEngineLevel&& _Other) noexcept = delete;

private:
	GameEngineLevel& operator=(const GameEngineLevel& _Other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _Other) noexcept = delete;

private:
	inline std::list<GameEngineCollision*>& GetCollisionGroup(int _Group)
	{
		return CollisionList_[_Group];
	}

public:
	virtual void LevelStart() = 0;
	virtual void LevelUpdate(float _DeltaTime) = 0;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) = 0;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) = 0;

public:
	template<typename ActorType>
	ActorType* CreateActor(int _UpdateOrder = 0);

	template<typename UserEnumType>
	void PushCollision(GameEngineCollision* _Collision, UserEnumType _Group);

	template<typename PostProcess, typename ... Parameter>
	PostProcess* AddPostProcessCameraMergePrev(Parameter ... _Arg);

	template<typename PostProcess, typename ... Parameter >
	PostProcess* AddPostProcessCameraMergeNext(Parameter ... _Arg);

	template<typename PostProcess, typename ... Parameter >
	PostProcess* AddPostProcess(const std::string& _Key, Parameter ... _Arg);

public:
	void SetLevelActorMove(GameEngineLevel* _NextLevel, GameEngineActor* _Actor);
	void PushCollision(GameEngineCollision* _Collision, int _Group);
	void PushDebugRender(GameEngineTransform* _Transform, CollisionType _Type);
	void AddTimeEvent(float _Time, std::function<void()> _Event);
	void ClearAll();

public:
	void ActorUpdate(float _DeltaTime);
	void Render(float _DeltaTime);
	void Release(float _DeltaTime);

public:
	CameraActor* GetMainCameraActor();
	CameraComponent* GetMainCamera();
	CameraActor* GetUICameraActor();
	CameraComponent* GetUICamera();

private:
	void Init();
	void LevelChangeEndActorEvent(GameEngineLevel* _NextLevel);
	void LevelChangeStartActorEvent(GameEngineLevel* _PrevLevel);
	void SetLevelActorMoveProcess();
	void TimeEventUpdate();
	void ChangeCollisionGroup(int _Group, GameEngineCollision* _Collision);
};

template<typename UserEnumType>
inline void GameEngineLevel::PushCollision(GameEngineCollision* _Collision, UserEnumType _Group)
{
	PushCollision(_Collision, static_cast<int>(_Group));
}

template<typename PostProcess, typename ...Parameter>
inline PostProcess* GameEngineLevel::AddPostProcessCameraMergePrev(Parameter ..._Arg)
{
	return AddPostProcess<PostProcess>("CameraMergePrev", _Arg...);
}

template<typename PostProcess, typename ...Parameter>
inline PostProcess* GameEngineLevel::AddPostProcessCameraMergeNext(Parameter ..._Arg)
{
	return AddPostProcess<PostProcess>("CameraMergeNext", _Arg...);
}

template<typename PostProcess, typename ...Parameter>
inline PostProcess* GameEngineLevel::AddPostProcess(const std::string& _Key, Parameter ..._Arg)
{
	PostProcess* NewPost = new PostProcess(_Arg...);
	GameEnginePostProcessRender* ParentType = dynamic_cast<GameEnginePostProcessRender*>(NewPost);
	ParentType->Initialize();
	PostRender[_Key].push_back(NewPost);
	return NewPost;
}

template<typename ActorType>
inline ActorType* GameEngineLevel::CreateActor(int _UpdateOrder)
{
	GameEngineActor* NewActor = new ActorType();
	NewActor->SetLevel(this);
	NewActor->Start();
	NewActor->SetOrder(_UpdateOrder);

	// Insert + Find
	std::list<GameEngineActor*>& List = ActorList_[_UpdateOrder];
	List.push_back(NewActor);
	return dynamic_cast<ActorType*>(NewActor);
}
