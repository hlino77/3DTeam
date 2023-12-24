#pragma once
#include <thread>
#include <functional>
#include <mutex>
#include <stack>
#include <map>
#include "Base.h"
#include "Lock.h"
#include <queue>


BEGIN(Engine)
/*------------------
	ThreadManager
-------------------*/

class ENGINE_DLL ThreadManager : public CBase
{
	DECLARE_SINGLETON(ThreadManager)


private:
	ThreadManager();
	~ThreadManager();

public:
	void	ReserveManager(_uint iJobThreadCount);

	void	Launch(function<void(void)> callback);
	void	Join();

	void DestroyTLS();


	// job �� �߰��Ѵ�.
	void EnqueueJob(std::function<void()> job);



private:
	// �� Worker �������� ����.
	_uint m_iJobThreadCount;
	// Worker �����带 �����ϴ� ����.
	std::vector<std::thread> m_JobThreads;
	// ���ϵ��� �����ϴ� job ť.
	std::queue<std::function<void()>> m_Jobs;
	// ���� job ť�� ���� cv �� m.
	std::condition_variable m_CV_Jobs;
	std::mutex m_MutexJob;


	bool stop_all;

	// Worker ������
	void WorkerThread();


private:
	mutex			_lock;
	vector<thread>	_threads;

	map<int, thread::id> m_ThreadInfo;
};

END