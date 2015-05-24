#pragma once
#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "HighResTimer.h"
#include "math.h"
#include "resource.h"
#include <Geometry/Primitives.h>
#include <Geometry/Converters.h>

#define NUM_SAMPLES 1024

class WaveForm
{
public:

	float* mp_fltHeights;
	Vector3f* mp_v3fNormals;
	Vector2f m_v2fDirection;
	Vector2f m_v2fPerpendicular;
	Point2d m_p2dOrigin; //must be DP because it is world-centric
	Line2f m_l2fDirection;
	Line2f m_l2fPerpendicular;
	Int32 m_i32SignDirectionVectorInX;
	Int32 m_i32SignDirectionVectorInY;

	Int32 m_i32HeadingDegrees;
	float m_fltHeadingRadians;
	float m_fltAmplitude;
	//float m_fltFlattening;
	float m_fltAmplitudeMultiplier;
	float m_fltPeakingFactor;
	//float m_fltResolution;
	float m_fltWaveLength;
	float m_fltWaveLengthReciprocal;

	float m_fltTrochoidCircleRadius;
	float m_fltTrochoidCentreOffset;

	float m_fltSpeed; //m/s
	Vector2f m_v2fVelocityNormalised; //the direction and distance the wave will travel in 1s.
	float m_fltGenesisTime; //based on high performance timer or system clock, but the system clock is only 20 ticks/sec.

private:

			float cosine;
			float sine;
			float xx, xy, xz;
			float yx, yy, yz;
			float zx, zy, zz;
			float xxt, xyt, xzt;


public:

	//The most natural model for describing a wave is as follows:
	//amplitude - height in metres, which will cause us to divide/multiply the normal result to get the desired height (what ever roundess is, the height will
	//be as specified by amplitude
	//roundness/peaking factor becomes another 0.0-1.0 value that gives us inner circle diameter

	WaveForm()
	{
	}

	WaveForm(const float a_fltAmplitude, const float a_fltPeakingFactor, const float a_fltWaveLength, const Int32 a_i32Heading, float fltVelocity, Point2f& a_p2fOrigin)
	{
		Init(a_fltAmplitude, a_fltPeakingFactor, a_fltWaveLength, a_i32Heading, fltVelocity, a_p2fOrigin);
	}

	void Init(const float a_fltAmplitude, const float a_fltPeakingFactor, const float a_fltWaveLength, const Int32 a_i32Heading, float fltVelocity, Point2f& a_p2fOrigin)
	{
		//We're changing the amplitude to be a value between 0.0 and 1.0, which will be a percentage of wave radius.  1.0 will render a perfect cycloid, and
		//0.0 will render a flat line.  Anything in between will render a trochoid, which is what we want.
		mp_fltHeights = new float[NUM_SAMPLES];
		mp_v3fNormals = new Vector3f[NUM_SAMPLES];

		m_i32HeadingDegrees = a_i32Heading;
		m_fltHeadingRadians = a_i32Heading * static_cast<float>(PI/180);

		//We really need to get this to work - we cannot be skipping points in the curve.
		//if we set the resolution of the curve high enough when we generate it, we won't get
		//issues with the rendering.

		for (int i=0; i<NUM_SAMPLES; i++)
			*(mp_fltHeights+i) = 0;

		m_fltWaveLength = a_fltWaveLength;

		//Initialise direction vector and line equation for the wave

		Point2f p2fOrigin;
		Geometry::Point2::ConvertD2F(m_p2dOrigin, p2fOrigin);
		m_v2fDirection.Init(a_i32Heading, a_fltWaveLength);
		m_l2fDirection.Init(p2fOrigin, m_v2fDirection); //the origin is world-space and must be double precision
		m_i32SignDirectionVectorInX = m_v2fDirection.X > 0? 1:-1;
		m_i32SignDirectionVectorInY = m_v2fDirection.Y > 0? 1:-1;

		//Initialise the wave-front (perpendicular) vector and line equation

		m_v2fPerpendicular.Init(-m_v2fDirection.Y,m_v2fDirection.X);
		m_l2fPerpendicular.Init(p2fOrigin, m_v2fPerpendicular);

		//Determine the reciprocal of the wavelength for use in clamping sample distances into an offset in a single wavelength

		m_fltWaveLengthReciprocal = 1/m_fltWaveLength;

		//For the trochoid wave, determine the circumference of the circle required to generate the requested wavelength
		//Now, circumference of the circle will have to be equal to the wavelength.  As Circumference = 2 * PI * R
		//so:

		m_fltAmplitude = a_fltAmplitude; //from top to bottom, the number of metres height
		m_fltPeakingFactor = a_fltPeakingFactor; //0.0 = flat (curtate trochoid), 1.0 = full peak (cycloid)
		m_fltTrochoidCircleRadius = m_fltWaveLength / static_cast<float>(2 * PI);
		m_fltTrochoidCentreOffset = m_fltTrochoidCircleRadius * m_fltPeakingFactor;

		//The amplitude multiplier is what we're going to multiply y by once it has been 0-based.  The natural (un-modified) maximum height of the wave
		//will be given by the 
		m_fltAmplitudeMultiplier = m_fltAmplitude/m_fltTrochoidCentreOffset; //What we need to multiply the result by to get the desired wave height

		/*
		Determine the normalised velocity vector.  We know the wavelength, and we know the number of metres per second the wave will travel.  So, if a wave is 10m long
		and travels at 0.1m/s, the time it will take to travel its length is 100s.  We know the length of the vector has to be 0.1m long.  So, we need to work out how
		many times .1 goes into the wave length.  How do we take the vector from, say [10,0] to [0.1,0]?  We know the vector represents the wavelength, so we divide the
		wavelength by the time the wave will take to travel that long, and then divide the direction vector (which is wavelength long) by that number:
		*/

		m_fltSpeed = fltVelocity;
		float fltSecondsPerWaveLength = m_fltWaveLength/fltVelocity;
		m_v2fVelocityNormalised = m_v2fDirection/fltSecondsPerWaveLength;

	}

	~WaveForm()
	{
		delete[] mp_fltHeights;
		delete[] mp_v3fNormals;
	}

	void ResetGenesisTime()
	{
		m_fltGenesisTime = (float)HighResTimer::GetSeconds();
	}

	void AdvanceWave()
	{
		float fltSecondsSinceGenesis = (float)HighResTimer::GetSeconds() - m_fltGenesisTime;

		//In a single-peak implementation of this method we might override an abstract base method, because what it will do will be different.  For example, linear
		//wave fronts work differently to circular ones because circular ones don't have a direction vector as such.

		//1 - See how far along the wave's direction vector we've travelled at the wave speed
		//2 - Add this vector to the point of wave origin to get a point along the new wave-front
		//3 - Work out the line equation for the new wavefront.

		Point2f p2fOrigin;
		Geometry::Point2::ConvertD2F(m_p2dOrigin, p2fOrigin);
		Vector2f v2fTranslatedSinceGenesis = m_v2fVelocityNormalised * fltSecondsSinceGenesis;
		Point2f p2fOnWaveFront = p2fOrigin + v2fTranslatedSinceGenesis;
		m_l2fPerpendicular.Reposition(p2fOnWaveFront);
	}

	inline float SampleWaveHeight(Point2f& a_p2fVertex, Vector3f& a_v3fNormal)
	{
		float fltRes = 0;

		//first, intersect the perpendicular vector for the wave, starting from the origin, with the direction vector of the wave
		//through the vertex.  To do this we need to get the equation for each line.
		
		//Work out the line formula for the vertex using the vector of the wave

		//we want to make a line copy that we can just reposition instead of making a new line.  because now we're doing the slope, inverse slope, etc
		//calculations when the line is first made, we can avoid these divisions again if the wave has a "linethruvertextocopy" member.  we have to be a bit
		//careful to not shoot ourselves in the foot for smp tho by accessing the same member in a write fashion, so we need to copy this locally.
		//the reference is 2fps faster than the copy, but we need to somehow move the line without having threads clash with each other.  Maybe we can solve
		//this using a "WaveSampler" class which is spawned by the wave-form on request by each thread.  Each sampler has its own state, but draws from the same
		//array of data.  This would permit one state per thread and would make it impossible to clash. Very nice :)

		//Line2D l2dThroughVertex(a_ptVertex, m_v2dDirection); //the original line - this line is 1 fps slower than the next 2
		//Line2D l2dThroughVertex = m_l2dDirection; //rely on the copy constructor here - we pick up 2 fps if we use the reference instead of a copy
		Line2f& l2fThroughVertex = m_l2fDirection; //rely on the copy constructor here
		l2fThroughVertex.Reposition(a_p2fVertex);	//the new line

		//Find the intersection of this line with the wave perpendicular vector through the wave origin

		Point2f p2fIntersection;
		//bool blnIntersected = l2dThroughVertex.Intersect(this->m_l2dPerpendicular, &p2dIntersection);
		//For linear waves we're going to do this instead because we know we're intersecting perpendicular lines, which permits some optimisation
		bool blnIntersected = l2fThroughVertex.IntersectPerpendicular(this->m_l2fPerpendicular, &p2fIntersection);

//		if (blnIntersected)
//		{
			//Now we've got the point of intersection, find the distance between the vertex and that point.

			bool blnBehindWave = false;

			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			//Instead of doing this using a square root, because we know the slope of the wave front we can use the larger of dx or dy between the points to
			//work out the distance, because the distance will always be directly proportionate to the slope (which might need to be changed a little bit for
			//vertical lines).

			//THIS IS YET TO BE IMPLEMENTED THIS WAY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			float fltDistance = a_p2fVertex.DistanceFrom(p2fIntersection);
			float fltOffsetInPhase = ClampDistanceToWavelength(fltDistance, blnBehindWave);

			//If we're in front of the wave front we need to sample from the other side of the texture, by subtracting this offset from 1.0f

			if (m_l2fDirection.m_fpSlope <= 1.0f)
			{
				//I'd like to see this done without a conditional branch - maybe some inline assembly could do this, seeing as the math library provides
				//no facility for getting the sign (why?)

				Int32 i32SignDeltaX = (a_p2fVertex.X - p2fIntersection.X) > 0.0f? 1.0f:-1.0f;
				if (i32SignDeltaX  == m_i32SignDirectionVectorInX)
					fltOffsetInPhase = 1.0f - fltOffsetInPhase;
			}
			else
			{
				float i32SignDeltaY = (a_p2fVertex.Y - p2fIntersection.Y) > 0.0f? 1.0f:-1.0f;
				if (i32SignDeltaY == m_i32SignDirectionVectorInY)
					fltOffsetInPhase = 1.0f - fltOffsetInPhase;
			}

			Int64 i64Index = Lookup(fltOffsetInPhase);
			fltRes = mp_fltHeights[i64Index];

			//We've got to rotate the normal yet - hey, we can do this once at the start of the program - it is always going to give the same result...
			a_v3fNormal = mp_v3fNormals[i64Index];
/*
		}
		else
		{
			//What happens now?  This shouldn't ever happen because we know that we're intersecting two perpendicular vectors...

			//assert(false, "Vectors did not intersect");

			fltRes = 0;
		}
*/
		//Initialise the normal vector to be vertical for the time being.  We've got to make this into a 3d vector though...

		//a_v3dNormal.Init(0.0f, 1.0f, 0.0f);
	

		return fltRes;
	}


private:

	//We've got to change the way this works a little - the wave looks like it is reflected around the origin of the wave, as can be seen by only generating
	//the upper samples half of the wave-form; just after the simulation starts, it is possible to see that there is initially a double-wide 0-plane spot
	//around the wave origin, but waves either side of that hvae the regular wave spacing.  We probably have to take into account the direction of the point
	//relative to the wave origin, so that we instead sample backwards down through the array (from the top) instead of just based on distance alone (from
	//the bottom)

	inline float ClampDistanceToWavelength(const float a_fltDistanceFromOrigin, bool& blnBehindWave)
	{
		/*
		Given the distance from the wave origin (or the perpendicular vector for it), work out how far inside the wavelength we are, getting the offset from the
		start of the phase it lands in.
		*/

		const float& fltD = a_fltDistanceFromOrigin;
		float fltWavelengthsFromOrigin = this->m_fltWaveLengthReciprocal * fltD;

		//So now we have the integer part being the wavelength we're in, and the fractional part is the distance inside the wave.  We need to drop the integer part.

		Int32 i32Phase = static_cast<Int32>(fltWavelengthsFromOrigin);
		float fltOffsetInPhase = fltWavelengthsFromOrigin - (float)i32Phase;
		float fltDistanceInPhase = fltOffsetInPhase * m_fltWaveLength;

		//Is it better off to use a metre-based offset or to just stick with the 0.0 <= d < 1.0 clamped portion - it might be easier this way, plus we save ourselves the
		//multiply by wavelength if we don't want the metre-based offset...  Plus, this will work more naturally if/when we move to texture lookups for wave heights, if we
		//end up taking that approach.

		return fltOffsetInPhase;
	}

	//Used during wave-form generation and wave height sampling

	inline Int64 Lookup(float fltX)
	{
		return (Int64)(fltX * NUM_SAMPLES);
	}

public:

	//instead of taking the data like this, we determine radius a using circumference, and we determine the b value based on a and the amplitude.
//			x = a.t - b.sin(t)
//			y = a - b.cos(t)

	void GenerateWaveformHeightData()
	{
			//Do the math to work out the rotation amounts for this wave direction

			
			cosine = cos(m_fltHeadingRadians);
			sine = sin(m_fltHeadingRadians);
			xx = 1.0f; xy = 0.0f; xz = 0.0f;
			yx = 0.0f; yy = 1.0f; yz = 0.0f;
			zx = 0.0f; zy = 0.0f; zz = 1.0f;
			xxt = .0f; xyt = .0f; xzt = .0f;

			xxt = cosine * xx + sine * zx; xyt = cosine * xy + sine * zy; xzt = cosine * xz + sine * zz;
			zx = cosine * zx - sine * xx; zy = cosine * zy - sine * xy; zz = cosine * zz - sine * xz;
			xx = xxt; xy = xyt; xz = xzt;

		bool blnMinStored = false;
		bool blnMaxStored = false;
		for (float fltT=0; fltT<=2*PI; fltT+=.002f)
		{
			//circle radius = a, distance of point p from centre is b

			float x = (m_fltTrochoidCircleRadius * fltT) - (m_fltTrochoidCentreOffset * sin(fltT));

			//Work out the index of the value that will be stored for this x by dividing x by the circumference,
			//thereby getting a value between 0 and 1.

			float y = m_fltTrochoidCircleRadius + (m_fltTrochoidCentreOffset*cos(fltT));

			float fltPartA = 2*x;
			float fltPartB = sqrt( (-2*x*-2*x) - 4*(x*x + (m_fltTrochoidCircleRadius-y)*(m_fltTrochoidCircleRadius-y) - ((m_fltTrochoidCentreOffset)*(m_fltTrochoidCentreOffset))) );
			float fltXonZeroPlane1 = (fltPartA + fltPartB)/2;
			float fltXonZeroPlane2 = (fltPartA - fltPartB)/2;
			float fltXofT;
			Int64 i64LookupInX = Lookup(x * m_fltWaveLengthReciprocal);

			if (i64LookupInX > NUM_SAMPLES/2)
				fltXofT = fltXonZeroPlane1;
			else
				fltXofT = fltXonZeroPlane2;
						
			//Now determine the offset of x in the array and store x:

			if (i64LookupInX >= 0 && i64LookupInX < NUM_SAMPLES)
					mp_fltHeights[i64LookupInX] = m_fltAmplitudeMultiplier*(y-(m_fltTrochoidCircleRadius-m_fltTrochoidCentreOffset));

			//Now store the normal vector for this location:

			Vector3f v3fNormal(fltXofT-x, (2*m_fltTrochoidCircleRadius-y)/m_fltAmplitudeMultiplier, 0.0f);

			if (i64LookupInX == 10 || i64LookupInX == 1013)
			{
				//breakpoint to check normal values
				float f = 0;
			}

			//We're going to try to make the wave normals combine more correctly by making the length of the normal a function of the wave height.  This way,
			//a large wave combined with a small wave should cause the larger wave's normal to be more influential.

			v3fNormal.MakeUnitVector();
			//v3fNormal *= m_fltAmplitude; //this actually seems to adversely affect the outcome - the shading is much flatter....

			//Now rotate the surface normals to agree with the direction of the wave

			if (i64LookupInX >= 0 && i64LookupInX < NUM_SAMPLES)
			{
				float fltX = xx * v3fNormal.X + yx * v3fNormal.Y + zx * v3fNormal.Z;
				float fltZ = xz * v3fNormal.X + yz * v3fNormal.Y + zz * v3fNormal.Z;
				mp_v3fNormals[i64LookupInX].Init(fltX, v3fNormal.Y, fltZ);
			}
		}

		//Finally, store a vertical normal vector for x=0
		mp_v3fNormals[0].Init(0, 1.0f, 0);
		//mp_v3dNormals[NUM_SAMPLES-1].Init(0, 1.0f, 0);

		m_fltGenesisTime = (float)HighResTimer::GetSeconds();
	}
};