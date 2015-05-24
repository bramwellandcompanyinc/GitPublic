#include "IThreadPoolQueueable.h"

struct ThreadPoolRequest
{
public:
//	IThreadPoolQueueable* mp_tpqFunctionPointer;
	Int64 m_i64NumThreads;
	Int64 m_i64InterlockedCounter;	//Initially stores the num threads but is decremented in an interlocked fashion by each thread on completion
//	ConditionVariable* mp_cvWakeOnComplete;
//	Mutex* mp_mtxWakeOnComplete;
};