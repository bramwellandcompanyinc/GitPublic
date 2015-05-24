#pragma once
#include <Geometry/Primitives.h>
#include "LookupTables.h"

class Matrix2x2f
{
	float m_fpElements[4];

public:

	Matrix2x2f();

	Matrix2x2f(float fp00, float fp01, float fp10, float fp11);
	
	inline void Init(float fp00, float fp01, float fp10, float fp11);

	inline float CalculateDeterminant();

	static Matrix2x2f& Multiply(Matrix2x2f& m22Left, Matrix2x2f& m22Right, Matrix2x2f& m22Out);

	static Matrix2x2f& CalculateInverse(Matrix2x2f& m22In, Matrix2x2f& m22Out);

	Matrix2x2f& CalculateInverse(Matrix2x2f& m22Out);
};

