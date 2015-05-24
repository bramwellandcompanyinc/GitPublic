#pragma once
#include <Geometry/Primitives.h>
#include <Matrix/Matrix2x2f.h>
#include <Matrix/Matrix3x3f.h>
#include "LookupTables.h"

extern "C" void Matrix4x4_SimdMultiplyVectorF(void*, void*, void*);
extern "C" void Matrix4x4_SimdMultiplyVectorD(void*, void*, void*);

class Matrix4x4f
{
public:

	//We use a pre-sized array here because a) we know how big it needs to be, and b) we'd like to avoid having to use NEW because it can slow things down
	//significantly due to the fact that it requires the OS to allocate the memory, and memory allocation/deallocation is a synchronised process.  Therefore,
	//potentially the program could be bottlenecked by allocations for these (what are very small afterall) objects.

	//The methods of this class are designed with this in mind, and with a specific emphasis on not copying data between objects unless we really have to,

	float m_fpElements[16];

	Matrix4x4f(){}

	Matrix4x4f(float fp00, float fp01, float fp02, float fp03,
			  float fp10, float fp11, float fp12, float fp13,
			  float fp20, float fp21, float fp22, float fp23,
			  float fp30, float fp31, float fp32, float fp33);


	void Init(float fp00, float fp01, float fp02, float fp03,
			  float fp10, float fp11, float fp12, float fp13,
			  float fp20, float fp21, float fp22, float fp23,
			  float fp30, float fp31, float fp32, float fp33);

	Matrix4x4f& SetIdentity();

	//This is basically the same as init, but whatever for the time being...

	inline float operator[](Int64 i64Index)
	{
		return m_fpElements[i64Index];
	}

	static Matrix4x4f& Copy(Matrix4x4f* ap_m44In, Matrix4x4f* ap_m44Out);

/*
	Vector4f& Multiply(Vector4f& v4fIn, Vector4f& v4fOut)
	{
		Matrix4x4_SimdMultiplyVectorF(this, &v4fIn, &v4fOut);
		return v4fOut;
	}

	Point4f& Multiply(Point4f& p4fIn, Point4f& p4fOut)
	{
		Matrix4x4_SimdMultiplyVectorF(this, &p4fIn, &p4fOut);
		return p4fOut;
	}
*/
	Point3f& Multiply(Point3f& a_p3In, Point3f& a_p3Out);

	//Do a scalar multiplication of the array elements.
	//For now we'll do this using c++, but in future we want to change this to use SIMD.
	//This function is implemented initially for translating the eye point a particular fraction of the current unitised view vector.
	Matrix4x4f& Multiply(float fpScalar, Matrix4x4f& m44Out);

	static void Multiply(Matrix4x4f& a_m44, Point3f& a_p3In, Point3f& a_p3Out);

	//loss of precision here but we need this to compile for now till we get the template matrix implemented

	static void Multiply(Matrix4x4f& a_m44, Vector3f& a_v3In, Vector3f& a_v3Out);

	static void Multiply(Matrix4x4f& a_m44Left, Matrix4x4f& a_m44Right, Matrix4x4f& a_m44Out);

	//For the time being we'll do this the same way the 3x3 matrix does it, and we'll ignore the W part of the vector.

	void Transform(float* p_fpXIn, float* p_fpYIn, float* p_fpZIn, float* p_fpXOut, float* p_fpYOut, float* p_fpZOut, Int64 i64CountPoints);

	void Transform(Point3f* ap_p3PointsIn, Point3f* ap_p3PointsOut, Int64 i64CountPoints);

/*
	void Transform(Point4D* ap_p4dPointsIn, Point4D* ap_p4dPointsOut, Int64 i64CountPoints)
	{
		float* E = m_fpElements;
		//Vector4D v4dIn
		for (Int64 i64=0; i64<i64CountPoints; i64++)
		{
			Matrix4x4::Multiply(*(ap_p4dPointsIn+i64), *(ap_p4dPointsOut+i64));
		}
	}
*/
	Matrix4x4f& Translate(float fpDeltaX, float fpDeltaY, float fpDeltaZ);

	static Matrix4x4f& Transpose(Matrix4x4f& m44In, Matrix4x4f& m44Out);

	//Perform an in-place transposition: otherwise known as an in-situ matrix transposition
/*
	Matrix4x4& Transpose()
	{
		Matrix4x4 m44Res;
		return this->Transpose(*this, m44Res);
	}
*/
	//is a rotation in world coords given by : R = W x A and a local given by R = A x W?  maybe...


	float CalculateDeterminant();

	static Matrix4x4f& CalculateInverse(Matrix4x4f& m44In, Matrix4x4f& m44Out);

	static Matrix4x4f& ComputeViewMatrix(Vector3f& a_v3ViewVector, Vector3f& a_v3UpVector, Matrix4x4f& a_m44Out);

	void SetViewMatrix(Matrix3x3f& m33Axes);

	Matrix4x4f& RotateInWorldX(float fpDegrees);

	Matrix4x4f& RotateInWorldY(float fpDegrees);

	Matrix4x4f& RotateInLocalZ(float fpDegrees);
};