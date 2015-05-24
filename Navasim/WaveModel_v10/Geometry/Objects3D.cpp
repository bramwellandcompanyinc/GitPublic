#include "stdafx.h"
#include "math.h"
#include <Geometry/Line.h>


/*
inline Point3<FPType>& Point3<FPType>::operator+=(Vector3<FPType>& a_v3t)
{
	X += a_v3t.X;
	Y += a_v3t.Y;
	Z += a_v3t.Z;

	return *this;
}
*/
/*
Point3D& Point3D::operator+=(Vector3D& a_v3d)
{
	X += a_v3d.X;
	Y += a_v3d.Y;
	Z += a_v3d.Z;

	return *this;
}

Vector3D& Point3D::GetAsPositionVector(Vector3D& a_v3t)
{
	a_v3t.Init(X, Y, Z);
	return a_v3t;
}
*/
/*
PlaneIntersectionConstants Line3D::Intersect(Plane& a_pln, Point3D& a_p3dOut)
{
	PlaneIntersectionConstants icRes = PlaneIntersectionConstants::LineIsParallel | PlaneIntersectionConstants::LineIsCoincident;

	//what happens if the line lies on this plane?  Is that an intersection technically, and either way, how do we signal this to the caller, and
	//how should the caller best handle that condition?

	//We need to substitute the parametric form of the line equation into the cartesian formula of the plane to solve for the line's k.  If D is our
	//plane's constant, and a, b, and c are the plane's normal vector elements, V is the direction vector of the line, and P is a point on the line,
	//then:

	//k = (-D-(aPx + bPy + cPz))/(aVx + bVy + cVz)

	//Alias parts of the plane for readability

	float& d = a_pln.m_fltD;
	Vector3D& n = a_pln.m_v3dNormal;

	//Do the same for this line

	Point3D& p = m_p3d1;
	Vector3D& v = m_v3dDirection;
	float fltDivisor = n.X*v.X + n.Y*v.Y + n.Z*v.Z;
	if (fltDivisor != 0.0f)
	{
		float fltK = (-d-(n.X*p.X + n.Y*p.Y + n.Z*p.Z))/fltDivisor;
		icRes = PlaneIntersectionConstants::LineIntersects;
		this->SolveForK(fltK, a_p3dOut);
	}
	else
	{
		//We don't know whether this is coincident or parallel unless we try to work out whether a point is on the plane, or the distance from the line
		//to the plane is the same along the length of the line

		icRes = PlaneIntersectionConstants::LineIsParallel | PlaneIntersectionConstants::LineIsCoincident;
//		a_p3d.Init(NaN, NaN, NaN);
	}

	//Knowing K now, we can try and solve for the intersection point
	return icRes;
}
*/
