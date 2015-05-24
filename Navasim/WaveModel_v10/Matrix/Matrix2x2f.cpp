#pragma once
#include "stdafx.h"
#include <Geometry/Primitives.h>
#include <Matrix/Matrix2x2f.h>

Matrix2x2f::Matrix2x2f()
{
}

Matrix2x2f::Matrix2x2f(float fp00, float fp01, float fp10, float fp11)
{
	Init(fp00, fp01, fp10, fp11);
}

void Matrix2x2f::Init(float fp00, float fp01, float fp10, float fp11)
{
	m_fpElements[0] = fp00;
	m_fpElements[1] = fp01;
	m_fpElements[2] = fp10;
	m_fpElements[3] = fp11;
}

float Matrix2x2f::CalculateDeterminant()
{
	float* E = m_fpElements;
	//float fpRes = E[0]*E[1] - E[2]*E[3];
	float fpRes = E[0]*E[3] - E[1]*E[2];
	return fpRes;
}

Matrix2x2f& Matrix2x2f::Multiply(Matrix2x2f& m22Left, Matrix2x2f& m22Right, Matrix2x2f& m22Out)
{
	float* L = m22Left.m_fpElements;
	float* R = m22Right.m_fpElements;
	float* O = m22Out.m_fpElements;

	//This checks out well.

	O[0] = L[0]*R[0] + L[1]*R[2];	O[1] = L[0]*R[1] + L[1]*R[3];
	O[2] = L[2]*R[0] + L[3]*R[2];	O[3] = L[2]*R[1] + L[3]*R[3];

	return m22Out;
}

Matrix2x2f& Matrix2x2f::CalculateInverse(Matrix2x2f& m22In, Matrix2x2f& m22Out)
{
	float fpDeterminant = m22In.CalculateDeterminant();
	float fp1D = 1/fpDeterminant; //1 over D

	float* I = m22In.m_fpElements;

	m22Out.Init(fp1D*I[3], -fp1D*I[1], -fp1D*I[2], fp1D*I[0]);

	return m22Out;
}

Matrix2x2f& Matrix2x2f::CalculateInverse(Matrix2x2f& m22Out)
{
	return Matrix2x2f::CalculateInverse(*this, m22Out);
}


