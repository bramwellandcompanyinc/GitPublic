#ifndef geometry_line_h
#define geometry_line_h

#include "math.h"
#include "GenericConstants.h"
#include "Assert.h"

template <class FPType> class Point2;
template <class FPType> class Point3;
template <class FPType> class Point4;
template <class FPType> class Vector2;
template <class FPType> class Vector3;
template <class FPType> class Vector4;
template <class FPType> class Plane;
enum PlaneIntersectionConstants;

template <class FPType> class Line2
{
public:
	float m_fpSlope;
	float m_fpSlopeInverse;
	float m_fpMySlopeMinusPerpendicularSlopeInverse;
	float m_fpOffset;
	bool m_blnVertical;

	inline Line2<FPType>()
	{
		m_fpSlope = 0;
		m_fpSlopeInverse = 0;
		m_fpMySlopeMinusPerpendicularSlopeInverse = 0;
		m_fpOffset = 0;
		m_blnVertical = false;
	}

	inline Line2(const Point2<FPType>& a_pt, const Vector2<FPType>& a_vec)
	{
		Init(a_pt, a_vec);
	}

	inline void Init(const Point2<FPType>& a_pt, const Vector2<FPType>& a_vec)
	{
		//Calculate the formula of the line given a direction vector and point that the line goes through.
		//For point (x, y) and vector [a, b], y = (b/a)x + d; therefore the offset d = y-(b/a)x

		if (a_vec.X != 0)
		{
			m_fpSlope = a_vec.Y/a_vec.X;
			m_fpSlopeInverse = 1/m_fpSlope;
			FPType fpSlopePerpendicular = -(a_vec.X/a_vec.Y);

			m_fpMySlopeMinusPerpendicularSlopeInverse = 1/(m_fpSlope - fpSlopePerpendicular);

			m_fpOffset = a_pt.Y - (m_fpSlope*a_pt.X);
			m_blnVertical = false;
		}
		else
		{
			m_blnVertical = true;
		}
	}

	//Reposition a line to a new point, assuming the slope remains initialised from a previous init call for this instance.  This has the benefit of saving a divide
	//because we know even if the line moves, the slope remains the same.

	inline void Reposition(const Point2<FPType>& a_pt)
	{
		//Calculate the formula of the line given a direction vector and point that the line goes through.
		//For point (x, y) and vector [a, b], y = (b/a)x + d; therefore the offset d = y-(b/a)x

		if (!m_blnVertical)
		{
			m_fpOffset = a_pt.Y - (m_fpSlope*a_pt.X);
		}
		else
		{
			//cry for now
			//m_fpOffset = X;?
		}
	}

	inline FPType SolveForX(const FPType a_fpX)
	{
		//y = ax + b

		FPType fpY = 0;
		if (!m_blnVertical)
		{
			fpY = (m_fpSlope * a_fpX) + m_fpOffset;
		}
		else
		{
			//fpY = ???
		}
		return fpY;
	}

	inline FPType SolveForY(const FPType a_fpY)
	{
		//y = ax+b, therefore x = (y-b)/a

		FPType fpX = (a_fpY - m_fpOffset) * m_fpSlopeInverse;
		return fpX;
	}

	//Intersect the passed line with this line

	void IntersectWithThrow(const Line2<FPType>& l2WavePerpendicular, Point2<FPType>* ap_p2Intersection)
	{
		if (!Intersect(l2wPerpendicular, ap_p2Intersection))
		{
			throw -1;
		}
	}

	bool Intersect(const Line2<FPType>& l2WavePerpendicular, Point2<FPType>* ap_p2Intersection)
	{
		//assert(pp_p2Intersection != nullptr);

		bool blnRes = false;
		/*
		If y1 = a1*x1 + b1 and y2 = a2*x2 + b2, and we know that x and y will be the same because we're intersecting the lines, then
		a1*x + b1 = a2*x + b2, so a1*x - a2*x = b2 - b1
		Now, (a1-a2)x = b2-b1, so x = (b2 - b1) / (a1 - a2)
		We then solve for x from either line equation to get y
		*/

		//Make sure the slopes are not the same - if they are then they don't intersect, so return false

		if (this->m_fpSlope != l2WavePerpendicular.m_fpSlope)
		{
			FPType fpX = (l2WavePerpendicular.m_fpOffset - this->m_fpOffset)/(this->m_fpSlope - l2WavePerpendicular.m_fpSlope);
			FPType fpY = SolveForX(fpX);

			ap_p2Intersection->Init(fpX, fpY);

			blnRes = true;
		}
		else
		{
			//Force a null pointer so if the calling code uses it we'll get an error right away, instead of letting it slide...

			blnRes = false;
		}

		return blnRes;
	}

	inline bool IntersectPerpendicular(const Line2<FPType>& l2WavePerpendicular, Point2<FPType>* ap_p2Intersection)
	{
		//assert(pp_p2Intersection != nullptr);

		bool blnRes = false;
		/*
		If y1 = a1*x1 + b1 and y2 = a2*x2 + b2, and we know that x and y will be the same because we're intersecting the lines, then
		a1*x + b1 = a2*x + b2, so a1*x - a2*x = b2 - b1
		Now, (a1-a2)x = b2-b1, so x = (b2 - b1) / (a1 - a2)
		We then solve for x from either line equation to get y
		*/

		//Make sure the slopes are not the same - if they are then they don't intersect, so return false

		//float fltX = (l2dWavePerpendicular.m_fpOffset - this->m_fpOffset)/(this->m_fpSlope - l2dWavePerpendicular.m_fpSlope);
		//In the case of perp lines, m_fpSlope is the inverse of the other line's slope, and we already know that.  So, we can calculate that
		//to start with when we create the line and rather than divide by the difference, multiply by the inverse.  So, the above line becomes:
		FPType fpX = (l2WavePerpendicular.m_fpOffset - this->m_fpOffset) * m_fpMySlopeMinusPerpendicularSlopeInverse;
		FPType fpY = SolveForX(fpX);

		ap_p2Intersection->Init(fpX, fpY);

		blnRes = true;

		return blnRes;
	}
};
typedef Line2<double> Line2d;
typedef Line2<float> Line2f;



	enum LineIntersectionConstants {Intersects=1, Coincident=2, Parallel=4};


template <class FPType> class Line3
{
	//equation of a line:
	//(x-a)/d1 = (y-b)/d2 = (z-c)/d3 = k
	//or
	//x = k*d1+a; y = k*d2+b; z = k*d3+c3 

public:


	Point3<FPType> m_p3_1;
	Vector3<FPType> m_v3Direction;

	Line3(){}

	Line3(Vector3<FPType>& a_v3)
	{
		Init(a_v3);
	}

	Line3(Point3<FPType>& a_p3_1, Point3<FPType>& a_p3_2)
	{
		Init(a_p3_1, a_p3_2);
	}

	Line3(Vector3<FPType>& a_v3Direction, Point3<FPType>& a_p3Reference)
	{
		Init(a_v3Direction, a_p3Reference);
	}

	void Init(Vector3<FPType>& a_v3Direction, Point3<FPType>& a_p3Reference)
	{
		m_v3Direction = a_v3Direction;
		m_p3_1 = a_p3Reference;
	}

	void Init(Vector3<FPType>& a_v3Direction)
	{
		m_v3Direction = a_v3Direction;
		m_p3_1.Init(0.0f, 0.0f, 0.0f);
	}

	void Init(Point3<FPType>& a_p3_1, Point3<FPType>& a_p3_2)
	{
		//Get our direction vector first, in the direction of p1 to p2

		m_v3Direction.Init(a_p3_1, a_p3_2);
		m_p3_1 = a_p3_1;
		//m_p3d2 = a_p3d2;
	}

	//Given a value of K, find the corresponding point on the line

	Point3<FPType>& SolveForK(FPType a_fpK, Point3<FPType>& a_p3Out)
	{
		Point3<FPType>& p = m_p31;
		Vector3<FPType>& v = m_v3Direction;
		a_p3Out.Init(a_fpK*v.X + p.X, a_fpK*v.Y + p.Y, a_fpK*v.Z + p.Z);

		return a_p3Out;
	}

	PlaneIntersectionConstants Intersect(Plane<FPType>& a_pln, Point3<FPType>& a_p3Out);

	//THIS HAPPENS TO BE A PROBLEM BECAUSE INTERSECTING 3D LINES IS INHERENTLY UNSAFE.
	//INTERSECTING LINES OF VARIOUS SLOPES COULD CAUSE INFINITY-SLOPE SITUATIONS, SO WE
	//NEED TO ENSURE WE COVER FOR THOSE CASES.  THIS CODE BELOW IS NOT CLOSE TO ACHIEVING THIS.
	LineIntersectionConstants IntersectOnXZPlane(Line3<FPType>& a_l3, Point3<FPType>& a_p3Out)
	{
		Point2<FPType> p2This(m_p3_1.X, m_p3_1.Z);


		//scratch this entire function, it's all fucked up

		Line2<FPType> l2This(m_p3_1.X, m_p3_1.Z);
		Line2<FPType> l2Intersect(a_l3.m_p3_1.X, a_l3.m_p3_1.Z);
		Point2<FPType> p2Out;
		LineIntersectionConstants lic = l2This.Intersect(l2Intersect, p2Out);
		a_p3Out.Init(p2Out.X, 0.0f, p2Out.Y, 0.0f);

		return lic;
	}
};
typedef Line3<double> Line3d;
typedef Line3<float> Line3f;

//Essentially the same as a regular line, except bounded (and defined by) two Point3D's
//will provide methods to work out whether a primitive (sphere, plane, line, etc) intersects this line between these bounds
//we still need to provide methods for intersecting with the line in an unbounded fashion - maybe this class should derive from Line3D???
/*
class LineSegment3D
{
public:
	bool IntersectPlane(Int64 a_i64BoundedFlag, Plane& a_pln, Point3D& a_p3dIntersection);
};
*/

#endif