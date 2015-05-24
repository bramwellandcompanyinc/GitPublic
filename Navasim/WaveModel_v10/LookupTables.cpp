#include "stdafx.h"
//#include "Assert.h"
#include <Assert.h>
#include "GenericConstants.h"
#include "LookupTables.h"
#include "math.h"

//we need to do the cleanup for the arrays too yet

//float* p_fltSquareRoots = nullptr;
double* p_dblSines = nullptr;
double* p_dblCosines = nullptr;
double* p_dblTangents = nullptr;
double* p_dblMeshAngularSeparations = nullptr;

#define ANGLE_DELTA 100
#define ONE_OVER_ANGLE_DELTA = 1/ANGLE_DELTA

#define NOT_POPULATED 999999.00

bool LookupTables::Init()
{
	bool blnRes = false;
	//Set up the sine and cosine tables.  We expect to get a number in degrees between 0 <= a < 360, and we're going to populate the for axes manually after
	//doing the rest, because we've found that - at least in the case of the cosine function, we don't get an accurate cos for 90 degress and possibly more

	p_dblSines = new double[360 * ANGLE_DELTA];
	p_dblCosines = new double[360 * ANGLE_DELTA];
	p_dblTangents = new double[360 * ANGLE_DELTA];
	
	//Fill the arrays with values so we can check that we have no holes in the wave forms

	for (Int64 i64=0; i64<360*ANGLE_DELTA; i64++)
	{
		p_dblSines[i64]=NOT_POPULATED;
		p_dblCosines[i64]=NOT_POPULATED;
		p_dblTangents[i64]=NOT_POPULATED;
	}

	if (p_dblSines && p_dblCosines)
	{
		Int64 i64Index = 0;
/*
		for (float f=0; f<360.0; f+=0.01f)
		{
			*(p_dblSines+i64Index) = sin(dblD2R*f);
			*(p_dblCosines+i64Index) = cos(dblD2R*f);
			i64Index++;
		}
		*/

		for (i64Index=0; i64Index<360 * ANGLE_DELTA; i64Index+=1)
		{
			float f = ((float)(i64Index))/(float)(ANGLE_DELTA) * dblD2R;
			float fltSin = sin(f);
			float fltCos = cos(f);
			double dblTan = tan(f);
			p_dblSines[i64Index] = fltSin;
			p_dblCosines[i64Index] = fltCos;
			p_dblTangents[i64Index] = dblTan;
			if (i64Index == 27000)
			{
				bool blnStop = true;
			}
		}

		//Make sure we over-write the main points with accurate entries, to avoid getting small negatives near zero, which can invert the model's orientation

		p_dblSines[0] = 0.0f;
		p_dblCosines[0] = 1.0f;

		p_dblSines[90*ANGLE_DELTA] = 1.0f;
		p_dblCosines[90*ANGLE_DELTA] = 0.0f;

		p_dblSines[180*ANGLE_DELTA] = 0.0f;
		p_dblCosines[180*ANGLE_DELTA] = -1.0f;

		p_dblSines[270*ANGLE_DELTA] = -1.0f;
		p_dblCosines[270*ANGLE_DELTA] = 0.0f;

		//Now do a quick check to make sure we have got no holes
/*
		for (Int64 i64=0; i64<360*ANGLE_DELTA; i64++)
		{
			if ( (p_dblSines[i64]==9999.00) || (p_dblCosines[i64]==9999.00) )
			{
				bool blnBad=true;
			}
		}
*/

		//Populate the angular separation lookup table that is indexed upon the height of eye and returns the angle that should be used to calculate how
		//granular an ocean mesh should be in order to represent the most reasonable trade-off between performance and surface accuracy.  As we approach the
		//water surface this angular separation will decrease because we want to capture more detail.  As we gain height we would not see that detail anyway
		//so we can safely increase the size of the mesh's polygons.

	//WE DON'T HAVE THE LOGIC FOR THIS WORKED OUT YET

		//Populate a 2D lookup table that will contain distances from the point on the surface below a point of eye at a given height, to the point on the
		//surface described by the view angle towards the surface.  This will be used to interpolate the wave mesh from far to near.  This might not actually
		//be a 2D lookup afterall - because this result needs to be accurate and cannot suffer the lack of granularity we'll get at higher altitudes.  Instead,
		//We should do this using the inverse tangent of the height/angle of eye.


		blnRes = true;
	}
	else
	{
		//we'll need to take care of this later on
		throw;
	}

	return blnRes;
}

double LookupTables::NormaliseHeading(double dblHeading)
{
	if (dblHeading < 0)
		dblHeading = 360.0f+dblHeading;
	else if (dblHeading >= 360.0f)
		dblHeading = dblHeading - 360.0f;

	return dblHeading;
}

float LookupTables::NormaliseHeading(float fltHeading)
{
	if (fltHeading < 0)
		fltHeading = 360.0f+fltHeading;
	else if (fltHeading >= 360.0f)
		fltHeading = fltHeading - 360.0f;

	return fltHeading;
}

double LookupTables::AngleDelta()
{
	return 1.0/ANGLE_DELTA;
}

double LookupTables::Sine(double& dblHeading)
{
	dblHeading = NormaliseHeading(dblHeading);

	Int64 i64LookupIndex = static_cast<Int64>(dblHeading*ANGLE_DELTA);
	float dblRes = p_dblSines[i64LookupIndex];
	if (dblRes != NOT_POPULATED)
		return dblRes;
	else
		throw;
}

double LookupTables::Cosine(double& dblHeading)
{
	dblHeading = NormaliseHeading(dblHeading);

	Int64 i64LookupIndex = static_cast<Int64>(dblHeading*ANGLE_DELTA);
	float dblRes = p_dblCosines[i64LookupIndex];
	if (dblRes != NOT_POPULATED)
		return dblRes;
	else
		throw;
}

double LookupTables::Tangent(double& dblAngle)
{
	dblAngle = NormaliseHeading(dblAngle);	//keep this normalisation local to the function - it need not affect the caller

	Int64 i64LookupIndex = static_cast<Int64>(dblAngle*ANGLE_DELTA);
	double dblRes = p_dblTangents[i64LookupIndex];
	//Assert(dblRes == NOT_POPULATED ? 0:1);
	return dblRes;
}


float LookupTables::Tangent(float& fltAngle)
{
	fltAngle = NormaliseHeading(fltAngle);	//keep this normalisation local to the function - it need not affect the caller

	Int64 i64LookupIndex = static_cast<Int64>(fltAngle*ANGLE_DELTA);
	float fltRes = static_cast<float>(p_dblTangents[i64LookupIndex]);
	//Assert(fltRes == NOT_POPULATED ? 0:1);
	return fltRes;
}


void LookupTables::SineAndCosine(double& dblHeading, double& dblSine, double& dblCosine)
{
	dblHeading = NormaliseHeading(dblHeading);

	Int64 i64LookupIndex = static_cast<Int64>(dblHeading*ANGLE_DELTA);
	dblSine = p_dblSines[i64LookupIndex];
	dblCosine = p_dblCosines[i64LookupIndex];
	if ( (dblSine == NOT_POPULATED) || (dblCosine == NOT_POPULATED) )
		throw;
}

void LookupTables::SineAndCosine(float& fltHeading, float& fltSine, float& fltCosine)
{
	fltHeading = (float)NormaliseHeading(fltHeading);

	Int64 i64LookupIndex = static_cast<Int64>(fltHeading*ANGLE_DELTA);
	fltSine = (float)p_dblSines[i64LookupIndex];
	fltCosine = (float)p_dblCosines[i64LookupIndex];
	if ( (fltSine == NOT_POPULATED) || (fltCosine == NOT_POPULATED) )
		throw;
}

/*
double LookupTables::OceanMeshAngularSeparation(double a_dblHeightOfEye)
{
	double dblSeparation;
	Int64 i64LookupIndex = static_cast<Int64>(a_dblHeightOfEye*ANGLE_DELTA);
	dblSeparation = p_dblMeshAngularSeparations[i64LookupIndex];
	Assert(dblSeparation != NOT_POPULATED);
	return dblSeparation;
}
*/
