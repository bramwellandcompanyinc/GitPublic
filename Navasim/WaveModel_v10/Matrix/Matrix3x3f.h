#pragma once
#include <Geometry/Primitives.h>
#include <Geometry/Point.h>
#include "LookupTables.h"

class Matrix3x3f
{
public:

	//We use a pre-sized array here because a) we know how big it needs to be, and b) we'd like to avoid having to use NEW because it can slow things down
	//significantly due to the fact that it requires the OS to allocate the memory, and memory allocation/deallocation is a synchronised process.  Therefore,
	//potentially the program could be bottlenecked by allocations for these (what are very small afterall) objects.

	//The methods of this class are designed with this in mind, and with a specific emphasis on not copying data between objects unless we really have to,

	float m_fpElements[9];

	Matrix3x3f();

	Matrix3x3f(float fp00, float fp01, float fp02,
			  float fp10, float fp11, float fp12,
			  float fp20, float fp21, float fp22);

	void Init(float fp00, float fp01, float fp02,
			  float fp10, float fp11, float fp12,
			  float fp20, float fp21, float fp22);

	inline float CalculateDeterminant();

	//This is basically the same as init, but whatever for the time being...

	static inline Matrix3x3f& Copy(Matrix3x3f* ap_m33In, Matrix3x3f* ap_m33Out);

	Matrix3x3f& Transpose();

	void SetIdentity();

	static void Multiply(Matrix3x3f& a_m33Left, Matrix3x3f& a_m33Right, Matrix3x3f& a_m33Out);

	float operator[](Int32 i32Index);

	//the far end of the z-axis dips down, the top end of the y-axis dips away
	Matrix3x3f& RotateInWorldX(float fpAngle);

	Matrix3x3f& RotateInWorldY(float fpAngle);

	Matrix3x3f& RotateInWorldZ(float fpAngle);

	//Apply the rotation stored in the current matrix to the point or vector provided

	Point3f& Transform(Point3f& p3In, Point3f& p3Out);

	Matrix3x3f& Transpose(Matrix3x3f& p3In, Matrix3x3f& p3Out);

	//Given three arrays of coordinates, apply the rotation/transformation this current matrix is set with.  We will ultimately and very definitely do this
	//using simd at some stage

	void Transform(float* p_fpXIn, float* p_fpYIn, float* p_fpZIn, float* p_fpXOut, float* p_fpYOut, float* p_fpZOut, Int64 i64CountPoints);
};
