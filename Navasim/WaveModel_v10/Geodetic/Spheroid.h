#ifndef spheroid_h
#define spheroid_h

#include "GenericConstants.h"
#include "Assert.h"
#include "math.h"
#include <Geometry/Primitives.h>

template <class FPType> class Point3;
template <class FPType> class Point4;
template <class FPType> class Vector3;
template <class FPType> class Vector4;
template <class FPType> class Line3;
template <class FPType> class Line4;

class Spheroid;
/*
class SpheroidSurfaceMesh;


//a class to encapsulate a lat, long, and maybe altitude/height relative to the surface of a spheroid at that point?
//this will probably just be a struct, as the data that is in here is really very highly dependent on the spheroid's shape
//this is a geodetic coordinate and works relative to the "reference ellipsoid" that GPS works on.  The latitude is not the angle
//from the point to the centre of the earth, rather it is the angle between the surface normal at the point and the equatorial plane.
class GeodeticCoordinate
{
//normal vector
//east-west
//north-south
//offset from surface along normal vector
	
	metre m_mtrHeight;
	Vector3d m_v3dNormal;
	double m_dblLatitude;
	double m_dblLongitude;

	//The next two parameters we'll gleam from the spheroid object itself.  We don't really need to store a reference to the spheroid once we've got
	//the basic information we need from it.

	double m_dblSemiMajorAxis;
	double m_dblFlatteningFactor;

	//so we need to store a reference to the spheroid too?
public:	
//	GeodeticCoordinate(Spheroid& a_sph, double a_dblLatitude, double a_dblLongitude, metre a_mtrHeight);
GeodeticCoordinate::GeodeticCoordinate(Spheroid& a_sph, double a_dblLatitude, double a_dblLongitude, metre a_mtrSurfaceOffset)
{}

	//Formulate a geodetic coordinate from a cartesian coordinate

	GeodeticCoordinate(Spheroid& a_sph, Point3d a_p3d);

	//the latitude given the line from the point through to the centre of the spheroid

	float GetGeocentricLatitude();

	//Return the current location as an X, Y, Z - we only do the double here because we know that for a spheroid the size of the earth, we need to
	//have double precision to represent any position within less than .5m accuracy (ideally down to the mm or cm).  Single precision floating point only
	//gives us .5m accuracy.

	Point3d& GetCartesianCoordinate(Point3d& a_p3dOut);
};
*/
enum SpheroidSurfaceFlag {IsBelowSurface = 1, IsAtSurface = 2, IsAboveSurface = 4};

class Spheroid
{
private:
	bool m_blnSpherical;
	double m_dblEquatorialRadius;
	double m_dblPolarRadius;
	Vector3d m_v3dNorthPoleVector;

public:

	static const double EARTH_AVE_RADIUS;

	//pole vector must be the +ve y axis for now
	Spheroid(double a_dblEquatorialRadius, double a_dblPolarRadius, Vector3d& a_v3dNorthPoleVector)
	{
		Vector3d v3dAxisY(0.0, 1.0, 0.0);
		Assert (a_v3dNorthPoleVector == v3dAxisY);

		m_dblEquatorialRadius = a_dblEquatorialRadius;
		m_dblPolarRadius = a_dblPolarRadius;
		m_blnSpherical = (m_dblEquatorialRadius == m_dblPolarRadius);
	}

	//construct a straight sphere with a coordinate system to convert between lats and longs and R3

	Spheroid(double& a_dblSphereRadius, Vector3d& a_v3dNorthPoleVector)
	{
		Vector3d v3dAxisY(0.0, 1.0, 0.0);
		Assert (a_v3dNorthPoleVector == v3dAxisY);

		m_dblEquatorialRadius = a_dblSphereRadius;
		m_dblPolarRadius = a_dblSphereRadius;
		m_blnSpherical = true;

	}

	//HorizonDepressionFromEye = angle from POE to horizon (ie, tangent to spheroid surface)
	//HorizonDepressionFromSurface = angle taken through the spheroid from the point on the surface directly below POE

	void GetHorizonParameters(Point3d& a_p3dPointOfEye, float& a_fltHorizonDistanceFromPOE, float& a_fltHorizonDistanceFromSPP, float& a_fltHorizonDepressionFromEye, float& a_fltHorizonDepressionBelowSurface);


	//return false if no intersection

	bool FindClosestIntersectionPoint(Line3d& a_l3d, Point3d& a_p3dOut)
	{
		bool blnRes = false;

		return blnRes;
	}
/*
	void ConvertCartesianToSurfaceCoords(Point3D& a_p3dIn, SpheroidSurfaceCoordinate& a_sscOut)
	{
	}

	void ConvertSurfaceToCartesianCoords(SpheroidSurfaceCoordinate& a_sscIn, Point3D& a_p4dOut)
	{

	}
*/
	//we'll define this enumeration later, but a_ssf will be below, at, and above
	double GetHeightOfEye(Point3d& a_p3d, SpheroidSurfaceFlag& a_ssf)
	{
		a_ssf = IsAboveSurface;
		//return 0;
	}

	double GetHeightOfEye(Point3d& a_p3d)
	{
		//For now we'll just return the Y value of POE until we've got the spheroid worked out

		return a_p3d.Y;
	}
	/*
	//another one to define later on
	double GetHeightOfEye(SpheroidSurfaceCoordinate& a_ssc, SpheroidSurfaceFlag& a_ssf)
	{
		a_ssf = IsAboveSurface;
		//return 0;
	}
*/
	bool PointIsAboveSurface(Point3d& a_p3d)
	{
		bool blnRes = true;
		return blnRes;
	}

private:

};

#endif