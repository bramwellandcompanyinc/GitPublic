#include "stdafx.h"

#include <Geodetic/Spheroid.h>


const double Spheroid::EARTH_AVE_RADIUS = 6375000; //m


void Spheroid::GetHorizonParameters(Point3d& a_p3dPointOfEye, float& a_fltHorizonDistanceFromPOE, float& a_fltHorizonDistanceFromSPP, float& a_fltHorizonDepressionFromEye, float& a_fltHorizonDepressionBelowSurface)
{
	//For now, return the HOE as if the poe is on a flat plane in the cartesian space.

	//The distance to the horizon will be that from the SPP to the actual tangent from the POE to the horizon

	//We're going to use the following names:
	//S represents the surface projected position
	//H represents the horizon point
	//C represents the point at the centre of the spheroid
	//P represents the point of eye above the spheroid surface
	//X represents a point on the line PH that the tanget at S intersects (just to give us the horizon dip below horizontal)
	//r represents the spheroid radius
	//h represents the height of eye above the surface

	//First get PH (distance directly between POE and H)

	double r = m_dblEquatorialRadius;
	double h = a_p3dPointOfEye.Y;
	double PH = sqrt(2*r*h + h*h);
	a_fltHorizonDistanceFromPOE = static_cast<float>(PH);
	
	//We need to get the distance SH because that is the basis for the surface mesh.  To do this, we need the angle
	//HCS

	double HCS = acos(r/(r+h)); //stay in radians for now

	//We now know how far the horizon is from S (directly, not along the surface) = HS

	a_fltHorizonDistanceFromSPP = static_cast<float>(2*r*r*(1-cos(HCS)));

	//We want to know how far the horizon dips from the tangent (horizonal) at eye level, which we can work out from 90-HPC

	double HPC = PI/2 - HCS; //90 degrees - HCS

	a_fltHorizonDepressionFromEye = static_cast<float>((PI/2 - HPC) * dblR2D);

	//We need to get the angle HSX to get the dip of the horizon plane below the tangent at S (a_fltHorizonDepressionBelowSurface).
	//We can get this by working out the two identical angles SHC and HSC that are at the surface of the circle. We already know
	//the angle HCS, so:

	double HSC = (PI-HCS)/2;
	a_fltHorizonDepressionBelowSurface = static_cast<float>(((PI/2) - HSC) * dblR2D);
}
