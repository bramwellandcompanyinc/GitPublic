#pragma once
#include "stdafx.h"
#include "GenericConstants.h"
#include "math.h"

class HighResTimer
{
public:

	static const Int32 MILLISECONDS_PER_SECOND;

private:
	static LARGE_INTEGER sm_i64CountsPerSecond;
	static double sm_dblSecondsConversion;


public:

	static void Init()
	{
		QueryPerformanceFrequency(&sm_i64CountsPerSecond);
		sm_dblSecondsConversion = 1.0/((double)sm_i64CountsPerSecond.QuadPart);
	}

	static inline double GetSeconds()
	{
		LARGE_INTEGER i64Count;
		QueryPerformanceCounter(&i64Count);
		double dblRes = ((double)i64Count.QuadPart) * sm_dblSecondsConversion;
		return dblRes;
	}

};
