#include "stdafx.h"
#include <Matrix/Matrix4x4f.h>

Matrix4x4f::Matrix4x4f(float fp00, float fp01, float fp02, float fp03,
					   float fp10, float fp11, float fp12, float fp13,
					   float fp20, float fp21, float fp22, float fp23,
					   float fp30, float fp31, float fp32, float fp33)
{
	Init(fp00, fp01, fp02, fp03,
		 fp10, fp11, fp12, fp13,
		 fp20, fp21, fp22, fp23,
		 fp30, fp31, fp32, fp33);
}

void Matrix4x4f::Init(float fp00, float fp01, float fp02, float fp03,
					  float fp10, float fp11, float fp12, float fp13,
					  float fp20, float fp21, float fp22, float fp23,
					  float fp30, float fp31, float fp32, float fp33)
{
	m_fpElements[0] = fp00;
	m_fpElements[1] = fp01;
	m_fpElements[2] = fp02;
	m_fpElements[3] = fp03;

	m_fpElements[4] = fp10;
	m_fpElements[5] = fp11;
	m_fpElements[6] = fp12;
	m_fpElements[7] = fp13;

	m_fpElements[8] = fp20;
	m_fpElements[9] = fp21;
	m_fpElements[10] = fp22;
	m_fpElements[11] = fp23;

	m_fpElements[12] = fp30;
	m_fpElements[13] = fp31;
	m_fpElements[14] = fp32;
	m_fpElements[15] = fp33;
}

Matrix4x4f& Matrix4x4f::SetIdentity()
{
	Init(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	return *this;
}

inline float Matrix4x4f::CalculateDeterminant()
{
	float fpRes;
	Matrix3x3f m33A, m33B, m33C, m33D;
	float* E = m_fpElements;

	//first generate the 4 3x3 sub-matrices

	m33A.Init(E[5], E[6], E[7],   E[9], E[10], E[11],   E[13], E[14], E[15]);
	m33B.Init(E[4], E[6], E[7],   E[8], E[10], E[11],   E[12], E[14], E[15]);
	m33C.Init(E[4], E[5], E[7],   E[8], E[9],  E[11],   E[12], E[13], E[15]);
	m33D.Init(E[4], E[5], E[6],   E[8], E[9],  E[10],   E[12], E[13], E[14]);

	//Now work out the determinants of those matrices

	float fpDa, fpDb, fpDc, fpDd;
	fpDa = m33A.CalculateDeterminant();
	fpDb = m33B.CalculateDeterminant();
	fpDc = m33C.CalculateDeterminant();
	fpDd = m33D.CalculateDeterminant();

	//Finally, combine the results

	fpRes = E[0]*fpDa - E[1]*fpDb + E[2]*fpDc - E[3]*fpDd;
	return fpRes;
}


Matrix4x4f& Matrix4x4f::CalculateInverse(Matrix4x4f& m44In, Matrix4x4f& m44Out)
{
	/*
	//We're going to be using this as a divisor, so lets divide it into 1 now.
	
	float fp1overD = 1/(m44In.CalculateDeterminant());

	//We're (unfortunately) going to be calculating 16 3x3 sub-matrices, so
	//let's do this in a loop

	Matrix3x3 m33Subs[16];

	//This next array provides the list of indices we should use to generate the
	//sub-matrices - ie, given an input index, this array gives us all the others
	//without needing to do conditional branching...

	//A = shorthand for intAllIndexesBut
	int A[4][3] = {{1, 2, 3},  {0, 2, 3},  {0, 1, 3},  {0, 1, 2}};

	float* E = m44In.m_fpElements; //in matrix
	float* F = m44Out.m_fpElements; //out matrix

	for (int i=0; i<4; i++)
	{
		int I1 = 4*A[i][0];
		int I2 = 4*A[i][1];
		int I3 = 4*A[i][2];

		for (int j=0; j<4; j++)
		{
			int intThisIndex = 4*i + j;

			int J1 = A[j][0];
			int J2 = A[j][1];
			int J3 = A[j][2];

			//Firstly, populate a 3x3 matrix for this position
			
			m33Subs[intThisIndex].Init(E[I1]+E[J1],  E[I1]+E[J2],  E[I1]+E[J3],
									   E[I2]+E[J1],  E[I2]+E[J2],  E[I2]+E[J3],
									   E[I3]+E[J1],  E[I3]+E[J2],  E[I3]+E[J3]);

			//Now, work out its determinant

			float fpD = m33Subs[intThisIndex].CalculateDeterminant();

			//The position for this point is the determinant for this matrix divided by the
			//determinant of the input 4x4 matrix (or multiplied by its inverse)

			float fpValue = fpD * fp1overD;

			//Finally, store it at the Ith,Jth element in the target 4x4 matrix

			F[intThisIndex] = fpValue;
		}
	}

	m44Out.Transpose();
	*/
	return m44Out;
}

Matrix4x4f& Matrix4x4f::Copy(Matrix4x4f* ap_m44In, Matrix4x4f* ap_m44Out)
{
	//We could again implement this with a loop but it seems faster to do this without - just directly copy the data across.  Loops aren't fast.
	
	float* p_fpEi = ap_m44In->m_fpElements;
	float* p_fpEo = ap_m44Out->m_fpElements;

	p_fpEo[0] = p_fpEi[0]; p_fpEo[1] = p_fpEi[1]; p_fpEo[2] = p_fpEi[2]; p_fpEo[3] = p_fpEi[3];
	p_fpEo[4] = p_fpEi[4]; p_fpEo[5] = p_fpEi[5]; p_fpEo[6] = p_fpEi[6]; p_fpEo[7] = p_fpEi[7];
	p_fpEo[8] = p_fpEi[8]; p_fpEo[9] = p_fpEi[9]; p_fpEo[10] = p_fpEi[10]; p_fpEo[11] = p_fpEi[11];
	p_fpEo[12] = p_fpEi[12]; p_fpEo[13] = p_fpEi[13]; p_fpEo[14] = p_fpEi[14]; p_fpEo[15] = p_fpEi[15];

	return *ap_m44Out;
}

void Matrix4x4f::Multiply(Matrix4x4f& a_m44, Point3f& a_p3In, Point3f& a_p3Out)
{
	float* M = a_m44.m_fpElements;
	Point3f& I = a_p3In;
	Point3f& O = a_p3Out;

	O.X = M[0]*I.X + M[1]*I.Y + M[2]*I.Z;
	O.Y = M[4]*I.X + M[5]*I.Y + M[6]*I.Z;
	O.Z = M[8]*I.X + M[9]*I.Y + M[10]*I.Z;
}

void Matrix4x4f::Multiply(Matrix4x4f& a_m44, Vector3f& a_v3In, Vector3f& a_v3Out)
{
	float* M = a_m44.m_fpElements;
	Vector3f& I = a_v3In;
	Vector3f& O = a_v3Out;

	O.X = M[0]*I.X + M[1]*I.Y + M[2]*I.Z;
	O.Y = M[4]*I.X + M[5]*I.Y + M[6]*I.Z;
	O.Z = M[8]*I.X + M[9]*I.Y + M[10]*I.Z;
}


Point3f& Matrix4x4f::Multiply(Point3f& a_p3In, Point3f& a_p3Out)
{

	return a_p3Out;
}

void Matrix4x4f::Multiply(Matrix4x4f& a_m44Left, Matrix4x4f& a_m44Right, Matrix4x4f& a_m44Out)
{
	float* p_fpL = a_m44Left.m_fpElements;
	float* p_fpR = a_m44Right.m_fpElements;
	float* p_fpO = a_m44Out.m_fpElements;

	//We could implement this multiply using a loop very easily and the code would be smaller, but the looping logic would slow down what should be a
	//very fast algorithm.  Ultimately, we'd like to be doing this using SIMD.

	p_fpO[0] = p_fpL[0]*p_fpR[0] + p_fpL[1]*p_fpR[4] + p_fpL[2]*p_fpR[8] + p_fpL[3]*p_fpR[12];
	p_fpO[1] = p_fpL[0]*p_fpR[1] + p_fpL[1]*p_fpR[5] + p_fpL[2]*p_fpR[9] + p_fpL[3]*p_fpR[13];
	p_fpO[2] = p_fpL[0]*p_fpR[2] + p_fpL[1]*p_fpR[6] + p_fpL[2]*p_fpR[10] + p_fpL[3]*p_fpR[14];
	p_fpO[3] = p_fpL[0]*p_fpR[3] + p_fpL[1]*p_fpR[7] + p_fpL[2]*p_fpR[11] + p_fpL[3]*p_fpR[15];

	p_fpO[4] = p_fpL[4]*p_fpR[0] + p_fpL[5]*p_fpR[4] + p_fpL[6]*p_fpR[8] + p_fpL[7]*p_fpR[12];
	p_fpO[5] = p_fpL[4]*p_fpR[1] + p_fpL[5]*p_fpR[5] + p_fpL[6]*p_fpR[9] + p_fpL[7]*p_fpR[13];
	p_fpO[6] = p_fpL[4]*p_fpR[2] + p_fpL[5]*p_fpR[6] + p_fpL[6]*p_fpR[10] + p_fpL[7]*p_fpR[14];
	p_fpO[7] = p_fpL[4]*p_fpR[3] + p_fpL[5]*p_fpR[7] + p_fpL[6]*p_fpR[11] + p_fpL[7]*p_fpR[15];

	p_fpO[8] = p_fpL[8]*p_fpR[0] + p_fpL[9]*p_fpR[4] + p_fpL[10]*p_fpR[8] + p_fpL[11]*p_fpR[12];
	p_fpO[9] = p_fpL[8]*p_fpR[1] + p_fpL[9]*p_fpR[5] + p_fpL[10]*p_fpR[9] + p_fpL[11]*p_fpR[13];
	p_fpO[10] = p_fpL[8]*p_fpR[2] + p_fpL[9]*p_fpR[6] + p_fpL[10]*p_fpR[10] + p_fpL[11]*p_fpR[14];
	p_fpO[11] = p_fpL[8]*p_fpR[3] + p_fpL[9]*p_fpR[7] + p_fpL[10]*p_fpR[11] + p_fpL[11]*p_fpR[15];

	p_fpO[12] = p_fpL[12]*p_fpR[0] + p_fpL[13]*p_fpR[4] + p_fpL[14]*p_fpR[8] + p_fpL[15]*p_fpR[12];
	p_fpO[13] = p_fpL[12]*p_fpR[1] + p_fpL[13]*p_fpR[5] + p_fpL[14]*p_fpR[9] + p_fpL[15]*p_fpR[13];
	p_fpO[14] = p_fpL[12]*p_fpR[2] + p_fpL[13]*p_fpR[6] + p_fpL[14]*p_fpR[10] + p_fpL[15]*p_fpR[14];
	p_fpO[15] = p_fpL[12]*p_fpR[3] + p_fpL[13]*p_fpR[7] + p_fpL[14]*p_fpR[11] + p_fpL[15]*p_fpR[15];
}


Matrix4x4f& Matrix4x4f::Multiply(float fpScalar, Matrix4x4f& m44Out)
{
	float* p_fpElements = m44Out.m_fpElements;
	for (Int64 i=0; i<16; i++)
		p_fpElements[i] = m_fpElements[i] * fpScalar;

	return m44Out;
}


void Matrix4x4f::Transform(Point3f* ap_p3PointsIn, Point3f* ap_p3PointsOut, Int64 i64CountPoints)
{
	float* E = m_fpElements;
	//Vector4D v4dIn
	for (Int64 i64=0; i64<i64CountPoints; i64++)
	{
		Matrix4x4f::Multiply(*this, *(ap_p3PointsIn+i64), *(ap_p3PointsOut+i64));
	}
}

void Matrix4x4f::Transform(float* p_fpXIn, float* p_fpYIn, float* p_fpZIn, float* p_fpXOut, float* p_fpYOut, float* p_fpZOut, Int64 i64CountPoints)
{
	float* E = m_fpElements;
	for (Int64 i64=0; i64<i64CountPoints; i64++)
	{
		p_fpXOut[i64] = E[0] * p_fpXIn[i64] + E[1] * p_fpYIn[i64] + E[2] * p_fpZIn[i64];
		p_fpYOut[i64] = E[3] * p_fpXIn[i64] + E[4] * p_fpYIn[i64] + E[5] * p_fpZIn[i64];
		p_fpZOut[i64] = E[6] * p_fpXIn[i64] + E[7] * p_fpYIn[i64] + E[8] * p_fpZIn[i64];
	}
}

Matrix4x4f& Matrix4x4f::Translate(float fpDeltaX, float fpDeltaY, float fpDeltaZ)
{
	m_fpElements[12] += fpDeltaX;
	m_fpElements[13] += fpDeltaY;
	m_fpElements[14] += fpDeltaZ;

	return *this;
}

Matrix4x4f& Matrix4x4f::Transpose(Matrix4x4f& m44In, Matrix4x4f& m44Out)
{
	float* E = m44In.m_fpElements;
	float* F = m44Out.m_fpElements;

	F[0]=E[0];	F[1]=E[4];	F[2]=E[8];		F[3]=E[12];
	F[4]=E[1];	F[5]=E[5];	F[6]=E[9];		F[7]=E[13];
	F[8]=E[2];	F[9]=E[6];	F[10]=E[10];	F[11]=E[14];
	F[12]=E[3];	F[13]=E[7];	F[14]=E[11];	F[15]=E[15];

	return m44Out;
}
