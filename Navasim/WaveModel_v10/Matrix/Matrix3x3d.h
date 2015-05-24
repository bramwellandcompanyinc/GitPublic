#pragma once
#include <Geometry/Primitives.h>
#include "LookupTables.h"

class Matrix3x3d
{
public:

	//We use a pre-sized array here because a) we know how big it needs to be, and b) we'd like to avoid having to use NEW because it can slow things down
	//significantly due to the fact that it requires the OS to allocate the memory, and memory allocation/deallocation is a synchronised process.  Therefore,
	//potentially the program could be bottlenecked by allocations for these (what are very small afterall) objects.

	//The methods of this class are designed with this in mind, and with a specific emphasis on not copying data between objects unless we really have to,

	double m_fpElements[9];

	Matrix3x3d();

	Matrix3x3d(double fp00, double fp01, double fp02,
			  double fp10, double fp11, double fp12,
			  double fp20, double fp21, double fp22);

	void Init(double fp00, double fp01, double fp02,
			  double fp10, double fp11, double fp12,
			  double fp20, double fp21, double fp22);

	inline double CalculateDeterminant();

	//This is basically the same as init, but whatever for the time being...

	static inline Matrix3x3d& Copy(Matrix3x3d* ap_m33In, Matrix3x3d* ap_m33Out);

	Matrix3x3d& Transpose();

	void SetIdentity();

	static void Multiply(Matrix3x3d& a_m33Left, Matrix3x3d& a_m33Right, Matrix3x3d& a_m33Out);

	double operator[](Int32 i32Index);

	//the far end of the z-axis dips down, the top end of the y-axis dips away
	Matrix3x3d& RotateInWorldX(double fpAngle);

	Matrix3x3d& RotateInWorldY(double fpAngle);

	Matrix3x3d& RotateInWorldZ(double fpAngle);

	//Apply the rotation stored in the current matrix to the point or vector provided

	Point3d& Transform(Point3d& p3dIn, Point3d& p3dOut);

	Matrix3x3d& Transpose(Matrix3x3d& p3dIn, Matrix3x3d& p3dOut);

	//Given three arrays of coordinates, apply the rotation/transformation this current matrix is set with.  We will ultimately and very definitely do this
	//using simd at some stage

	void Transform(double* p_fpXIn, double* p_fpYIn, double* p_fpZIn, double* p_fpXOut, double* p_fpYOut, double* p_fpZOut, Int64 i64CountPoints);
};
