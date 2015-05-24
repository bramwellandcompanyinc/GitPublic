#pragma once
#include "GenericConstants.h"
#include "Assert.h"
#include "math.h"

template <class FPType> class Point3;
template <class FPType> class Point4;
template <class FPType> class Vector3;
template <class FPType> class Line3;

enum PlaneIntersectionConstants {LineIntersects=1, LineIsCoincident=2, LineIsParallel=4};

template <class FPType> class Plane
{
public:

	//These are defined on the basis of: ax + by + cz + d = 0;
	//lets do it this way instead, given n is the normal vector, and p is a known point on the plane:
	//nx(x-px) + ny(x-py) + nz(x-pz) = 0

	Vector3<FPType> m_v3Normal;
	Point3<FPType> m_p3Reference;
	FPType m_fpD;

	Plane()
	{}

	//Construct a plane given any three non-colinear points on the plane

	Plane(Point3<FPType>& a_p3_1, Point3<FPType>& a_p3_2, Point3<FPType>& a_p3_3)
	{
		Init(a_p3_1, a_p3_2, a_p3_3);
	}

	//Construct a plane given the normal vector to the plane, and a reference point on the plane

	Plane(Vector3<FPType>& a_v3Normal, Point3<FPType>& a_p3Reference)
	{
		Init(a_v3Normal, a_p3Reference);
	}

	//Construct a plane given the places where it intercepts the x, y, and z axes

	Plane(FPType& a_fpXIntercept, FPType& a_fpYIntercept, FPType& a_fpZIntercept)
	{
		//Construct 3 points - one for each intercept, and we know they lie on the plane

		Point3<FPType> p3Ix(a_fpXIntercept, 0, 0);
		Point3<FPType> p3Iy(0, a_fpYIntercept, 0);
		Point3<FPType> p3Iz(0, 0, a_fpZIntercept);

		Init(p3Ix, p3Iy, p3Iz);
	}

	void Init(Point3<FPType>& a_p3_1, Point3<FPType>& a_p3_2, Point3<FPType>& a_p3_3)
	{
		m_p3Reference = a_p3_1;

		//Find two vectors between two sets of two points, with one point being common to both vectors

		Vector3<FPType> v31(a_p3_1, a_p3_2);
		Vector3<FPType> v32(a_p3_1, a_p3_3);
		
		//Find the cross product of the two vectors, and we have the normal vector

		v3_1.Cross(v3_2, m_v3Normal);

		//Finally, plug the reference point into the equation to find the constant D

		GenerateD();
	}

	void Init(Vector3<FPType>& a_v3Normal, Point3<FPType>& a_p3Reference)
	{
		m_v3Normal = a_v3Normal;
		m_p3Reference = a_p3Reference;
		GenerateD();
	}

	PlaneIntersectionConstants Intersect(Line3<FPType>& a_l3, Point3<FPType>& a_p3Out)
	{

		return a_l3.Intersect(*this, a_p3Out);
	}

private:

	void GenerateD()
	{
		Vector3<FPType>& n = m_v3Normal;
		Point3<FPType>& r = m_p3Reference;
		m_fpD = -(n.X*r.X + n.Y*r.Y + n.Z*r.Z);
		//m_fltD = n.X*r.X + n.Y*r.Y + n.Z*r.Z;
	}
};
typedef Plane<double> PlaneD;
typedef Plane<float> PlaneF;