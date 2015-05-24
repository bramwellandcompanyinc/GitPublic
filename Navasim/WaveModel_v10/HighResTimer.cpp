#include "stdafx.h"
#include "HighResTimer.h"

const Int32 HighResTimer::MILLISECONDS_PER_SECOND = 1000;
LARGE_INTEGER HighResTimer::sm_i64CountsPerSecond;
double HighResTimer::sm_dblSecondsConversion;