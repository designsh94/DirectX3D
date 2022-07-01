#pragma once
#include "GameEngineThread.h"
#include "GameEngineIocp.h"

class GameEngineThreadQueue
{
private:
	class Job
	{
	public:
		std::function<void()> JobFunction;
	};

private:
	static void JobWorkThread(GameEngineThread* Thread, GameEngineIocp* _Iocp)
	{
		DWORD lpNumberOfBytesTransferred = 0;
		ULONG_PTR lpCompletionKey = 0;
		LPOVERLAPPED Ptr = nullptr;
		while (true)
		{
			_Iocp->WaitforWork(lpNumberOfBytesTransferred, lpCompletionKey, Ptr);
			switch (lpNumberOfBytesTransferred)
			{
				case -1:
				{
					return;
				}
				case -2:
				{
					Job* PostJob = reinterpret_cast<Job*>(lpCompletionKey);
					PostJob->JobFunction();
					delete PostJob;
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}

private:
	GameEngineIocp Iocp;
	int ThreadCount_;
	std::vector<GameEngineThread*> Threads_;

public:
	GameEngineThreadQueue(const std::string& _ThreadName, int _ThreadCount = 0)
	{
		ThreadCount_ = _ThreadCount;
		if (ThreadCount_ == 0)
		{
			SYSTEM_INFO Info;
			GetSystemInfo(&Info);
			ThreadCount_ = Info.dwNumberOfProcessors - 2;
		}

		if (ThreadCount_ <= 0)
		{
			ThreadCount_ = 1;
		}

		Iocp.Start(ThreadCount_);
		for (size_t i = 0; i < ThreadCount_; i++)
		{
			GameEngineThread* NewThread = new GameEngineThread();
			std::string ThreadName = _ThreadName + std::to_string(static_cast<int>(i));
			NewThread->Start(ThreadName, std::bind(JobWorkThread, NewThread, &Iocp));
			Threads_.push_back(NewThread);
		}
	}

	~GameEngineThreadQueue()
	{
		Destroy();
	}

public:
	void JobPost(std::function<void()> _Job)
	{
		Job* NewJob = new Job();
		NewJob->JobFunction = _Job;
		Iocp.Makework(-2, NewJob);
	}

	void Destroy()
	{
		for (size_t i = 0; i < ThreadCount_; i++)
		{
			Iocp.Makework(-1, nullptr);
			Sleep(1);
		}

		for (size_t i = 0; i < Threads_.size(); i++)
		{
			delete Threads_[i];
			Threads_[i] = nullptr;
		}

		Threads_.clear();
	}
};

