/*
#include "stdafx.h"
#include "GenericConstants.h"
#include "Stack.h"
#include "ThreadPoolThread.h"
#include "ThreadPoolRequest.h"

//Defines a thread pool for managing mega-threaded tasks

class ThreadPool
{
private:
	Stack* mp_stkAvailableThreads;	//Threads will be removed from this stack by the scheduling (calling) thread
	ThreadPoolThread* mp_tptThreads;	//The actual thread objects.  The thread index value indexes into this array.
	Int64* mp_i64SchedulerThreadIDs;
	Int64* mp_i64LogicalThreadIDs;
	ConditionVariable* mp_cvMain;	//The condition variable that might be used to wake the calling thread back up again on completion of a task
	Mutex* mp_mtxMain;	//The mutex to protect the main condition variable
	ConditionVariable* mp_cvThreads;	//The condition variables used to wake worker threads
	Mutex* mp_mtxThreads;	//The mutex used to protect the condition variables

public:
	ThreadPool();
	ThreadPool(Int64 a_i64NumThreads);
	void Init(Int64 a_i64NumThreads);
	virtual ~ThreadPool();
	void Destruct();
	void QueueRequest(ThreadPoolRequest* ap_tpr);

};
*/