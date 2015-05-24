#pragma once

typedef short Int16;
typedef int Int32;
typedef long Int64;
typedef unsigned short UInt16;
typedef unsigned int UInt32;
typedef unsigned long UInt64;

const double PI = 3.14159265358979323846;
const double dblD2R = PI/180.0;
const double dblR2D = 180.0/PI;

#define _Align16_ __declspec(align(16))

typedef double metre;
typedef float degree;