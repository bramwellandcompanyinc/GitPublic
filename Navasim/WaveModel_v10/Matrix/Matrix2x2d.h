#pragma once
#include <Geometry/Primitives.h>
#include "LookupTables.h"

class Matrix2x2d
{
	double m_fpElements[4];

public:

	Matrix2x2d();

	Matrix2x2d(double fp00, double fp01, double fp10, double fp11);
	
	inline void Init(double fp00, double fp01, double fp10, double fp11);

	inline double CalculateDeterminant();

	static Matrix2x2d& Multiply(Matrix2x2d& m22Left, Matrix2x2d& m22Right, Matrix2x2d& m22Out);

	static Matrix2x2d& CalculateInverse(Matrix2x2d& m22In, Matrix2x2d& m22Out);

	Matrix2x2d& CalculateInverse(Matrix2x2d& m22Out);
};

