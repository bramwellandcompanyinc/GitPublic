#pragma once
#include "stdafx.h"
#include <Geometry/Point.h>
#include <Matrix/Matrix2x2d.h>
#include <Matrix/Matrix3x3d.h>
#include "LookupTables.h"

Matrix3x3d::Matrix3x3d()
{
	//for (int i=0; i<9; i++)
	//	m_fpElements[i] = 0.0f;
}

Matrix3x3d::Matrix3x3d(double fp00, double fp01, double fp02,
					   double fp10, double fp11, double fp12,
					   double fp20, double fp21, double fp22)
{
	Init(fp00, fp01, fp02,
		 fp10, fp11, fp12,
		 fp20, fp21, fp22);
}

void Matrix3x3d::Init(double fp00, double fp01, double fp02,
					  double fp10, double fp11, double fp12,
					  double fp20, double fp21, double fp22)
{
	m_fpElements[0] = fp00;
	m_fpElements[1] = fp01;
	m_fpElements[2] = fp02;

	m_fpElements[3] = fp10;
	m_fpElements[4] = fp11;
	m_fpElements[5] = fp12;

	m_fpElements[6] = fp20;
	m_fpElements[7] = fp21;
	m_fpElements[8] = fp22;
}

double Matrix3x3d::CalculateDeterminant()
{
	double fpRes;
	Matrix2x2d m22A, m22B, m22C;
	double* E = m_fpElements;

	//first generate the 3 2x2 sub-matrices

	m22A.Init(E[4],E[5],E[7],E[8]);
	m22B.Init(E[3],E[5],E[6],E[8]);
	m22C.Init(E[3],E[4],E[6],E[7]);

	//Now work out the determinants of those matrices

	double fpDa, fpDb, fpDc;
	fpDa = m22A.CalculateDeterminant();
	fpDb = m22B.CalculateDeterminant();
	fpDc = m22C.CalculateDeterminant();

	//Finally, combine the results

	fpRes = E[0]*fpDa - E[1]*fpDb + E[2]*fpDc;
	return fpRes;
}

//This is basically the same as init, but whatever for the time being...

Matrix3x3d& Matrix3x3d::Copy(Matrix3x3d* ap_m33In, Matrix3x3d* ap_m33Out)
{
	//We could again implement this with a loop but it seems faster to do this without - just directly copy the data across.  Loops aren't fast.
	
	//It would be nice to implement this copy as a set of compiler intrinsics... REP STOSQW (=8 bytes = 2 doubles so count 4) and then a etc...
	//but they suck,  i'd rather use inline asm instead.  they have wierd things going on and seem like a better idea than they really are so far...


	double* p_fpEi = ap_m33In->m_fpElements;
	double* p_fpEo = ap_m33Out->m_fpElements;

//		__movsq(p_fpEo, p_fpEi, 4);
//		__movsd(p_fpEo+8, p_fpEi+8, 1);


	p_fpEo[0] = p_fpEi[0]; p_fpEo[1] = p_fpEi[1]; p_fpEo[2] = p_fpEi[2];
	p_fpEo[3] = p_fpEi[3]; p_fpEo[4] = p_fpEi[4]; p_fpEo[5] = p_fpEi[5];
	p_fpEo[6] = p_fpEi[6]; p_fpEo[7] = p_fpEi[7]; p_fpEo[8] = p_fpEi[8];

	return *ap_m33Out;
}

Matrix3x3d& Matrix3x3d::Transpose()
{
	//Convert from a column-major format to row-major (or vice-versa)

	double* fpE = m_fpElements;
	Matrix3x3d m33(fpE[0], fpE[3], fpE[6],  fpE[1], fpE[5], fpE[7],  fpE[2], fpE[6], fpE[8]);

	Copy(&m33, this);

	return *this;
}

void Matrix3x3d::SetIdentity()
{
	Init(1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f);
}

void Matrix3x3d::Multiply(Matrix3x3d& a_m33Left, Matrix3x3d& a_m33Right, Matrix3x3d& a_m33Out)
{
	double* p_fpL = a_m33Left.m_fpElements;
	double* p_fpR = a_m33Right.m_fpElements;
	double* p_fpO = a_m33Out.m_fpElements;

	//We could implement this multiply using a loop very easily and the code would be smaller, but the looping logic would slow down what should be a
	//very fast algorithm.  Ultimately, we'd like to be doing this using SIMD.  To do this with simd we might want to use a 4x4 matrix so that we get
	//operand alignment, but use 0 as the last operand, meaning that its value won't affect the outcome, and then we only return the 9 parts we want.
	//This would mean, however, that we'd need to implement a 3x3 matrix using a 4x4 array, and this will affect copy time... hmm... do we spend more time
	//multiplying or copying?  There are 27 muls below... we could reduce this to maybe 9 using simd... well if we only manually copy 9 elements and know
	//the other 6 are 0 we don't need to copy them, but then if we use rep stos instructions to do the copy we'd have to do the entire chunk... i'm not sure
	//it is worth it - once we're finished doing the muls for each row/column combination we've then got to add them together into one value - and this may
	//take more instructions and that would not be good.  Nah, save simd for a better cause.

	p_fpO[0] = p_fpL[0]*p_fpR[0] + p_fpL[1]*p_fpR[3] + p_fpL[2]*p_fpR[6];
	p_fpO[1] = p_fpL[0]*p_fpR[1] + p_fpL[1]*p_fpR[4] + p_fpL[2]*p_fpR[7];
	p_fpO[2] = p_fpL[0]*p_fpR[2] + p_fpL[1]*p_fpR[5] + p_fpL[2]*p_fpR[8];

	p_fpO[3] = p_fpL[3]*p_fpR[0] + p_fpL[4]*p_fpR[3] + p_fpL[5]*p_fpR[6];
	p_fpO[4] = p_fpL[3]*p_fpR[1] + p_fpL[4]*p_fpR[4] + p_fpL[5]*p_fpR[7];
	p_fpO[5] = p_fpL[3]*p_fpR[2] + p_fpL[4]*p_fpR[5] + p_fpL[5]*p_fpR[8];

	p_fpO[6] = p_fpL[6]*p_fpR[0] + p_fpL[7]*p_fpR[3] + p_fpL[8]*p_fpR[6];
	p_fpO[7] = p_fpL[6]*p_fpR[1] + p_fpL[7]*p_fpR[4] + p_fpL[8]*p_fpR[7];
	p_fpO[8] = p_fpL[6]*p_fpR[2] + p_fpL[7]*p_fpR[5] + p_fpL[8]*p_fpR[8];
}

double Matrix3x3d::operator[](Int32 i32Index)
{
	return m_fpElements[i32Index];
}

//the far end of the z-axis dips down, the top end of the y-axis dips away
Matrix3x3d& Matrix3x3d::RotateInWorldX(double fpAngle)
{
	//0 degrees means horizontal, therefore we must use cos(0) = 1 as fpSinZ
	
	//We're going to negate the rotation around the x-axis here because it seems to be the only way that the rotate in x up/down seems to work properly,
	//even though based on math it should be the other way around

	double fpSinX = sin(fpAngle);
	double fpCosX = cos(fpAngle);

	//We could eventually define a static rotation matrix for the 4 types - x, y, z, & vector rotations

	Matrix3x3d m33Out;
	Matrix3x3d m33RotX(1,0,0,  0,fpCosX, -fpSinX,  0,fpSinX,fpCosX);
	Multiply(*this, m33RotX, m33Out);
	Copy(&m33Out, this);

	return *this;
}

Matrix3x3d& Matrix3x3d::RotateInWorldY(double fpAngle)
{
	double fpSinY = sin(fpAngle);
	double fpCosY = cos(fpAngle);

	Matrix3x3d m33Out;
	Matrix3x3d m33RotY(fpCosY,0,fpSinY,  0,1,0,  -fpSinY,0,fpCosY);
	Multiply(*this, m33RotY, m33Out);
	Copy(&m33Out, this);

	return *this;
}

Matrix3x3d& Matrix3x3d::RotateInWorldZ(double fpAngle)
{
	double fpSinZ = sin(fpAngle);
	double fpCosZ = cos(fpAngle);

	Matrix3x3d m33Out;
	Matrix3x3d m33RotZ(fpCosZ,-fpSinZ,0,  fpSinZ,fpCosZ,0,  0,0,1);
	Matrix3x3d::Multiply(*this, m33RotZ, m33Out);
	Copy(&m33Out, this);

	return *this;
}

//Apply the rotation stored in the current matrix to the point or vector provided

Point3d& Matrix3x3d::Transform(Point3d& p3In, Point3d& p3Out)
{
	p3Out.X = m_fpElements[0] * p3In.X + m_fpElements[1] * p3In.Y + m_fpElements[2] * p3In.Z;
	p3Out.Y = m_fpElements[3] * p3In.X + m_fpElements[4] * p3In.Y + m_fpElements[5] * p3In.Z;
	p3Out.Z = m_fpElements[6] * p3In.X + m_fpElements[7] * p3In.Y + m_fpElements[8] * p3In.Z;

	return p3Out;
}

//Given three arrays of coordinates, apply the rotation/transformation this current matrix is set with.  We will ultimately and very definitely do this
//using simd at some stage

void Matrix3x3d::Transform(double* p_fpXIn, double* p_fpYIn, double* p_fpZIn, double* p_fpXOut, double* p_fpYOut, double* p_fpZOut, Int64 i64CountPoints)
{
	for (Int64 i64=0; i64<i64CountPoints; i64++)
	{
		p_fpXOut[i64] = m_fpElements[0] * p_fpXIn[i64] + m_fpElements[1] * p_fpYIn[i64] + m_fpElements[2] * p_fpZIn[i64];
		p_fpYOut[i64] = m_fpElements[3] * p_fpXIn[i64] + m_fpElements[4] * p_fpYIn[i64] + m_fpElements[5] * p_fpZIn[i64];
		p_fpZOut[i64] = m_fpElements[6] * p_fpXIn[i64] + m_fpElements[7] * p_fpYIn[i64] + m_fpElements[8] * p_fpZIn[i64];
	}
}
