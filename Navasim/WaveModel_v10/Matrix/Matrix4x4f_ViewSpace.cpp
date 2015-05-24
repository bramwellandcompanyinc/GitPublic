#include "stdafx.h"
#include <Matrix/Matrix4x4f.h>

//mimics the gluLookat function

Matrix4x4f& Matrix4x4f::ComputeViewMatrix(Vector3f& a_v3ViewVector, Vector3f& a_v3UpVector, Matrix4x4f& a_m44Out)
{
	//We're going to "mostly" replicate the glLookAt function, except rather than providing two points and then calculating a vector, we're going to
	//provide a single point to which we'll ultimately translate the matrix and a view direction vector, and additionally, we're not going to map down
	//the negative z-axis - we'll map down the positive one instead.

	//Firstly, lets normalise the vectors, but we don't want to mess with the originals - we don't want to unnecessarily cause issues by doing that, at
	//least not at this point anyway.  This does cause a copy between vectors, but for the sake of 3 floats per copy, it isn't really a big deal.  And
	//besides, this is going to be done about once per frame anyway, so let's take the hit and have less impact on naive caller routines.

	Vector3f& v3VV = a_v3ViewVector;
	Vector3f& v3UV = a_v3UpVector;
	v3VV.MakeUnitVector();
	v3UV.MakeUnitVector();
/*
	http://www.devmaster.net/forums/archive/index.php/t-3145.html
	First, calculate forward, right, and up vectors. The forward vector should be a unit vector pointing in the direction that your camera is looking.
	The right and up vectors must be unit vectors, and perpendicular to each other and the forward vector. How to come up with these vectors is a matter
	of how your camera works, so I won't go into more detail (unless you ask).

	Now, make a 3x3 matrix that has as its first row, the right vector; as its second row, the up vector; and as its third row, the NEGATIVE of the
	forward vector (this is so the forward vector maps onto the negative Z axis, which is the usual behavior for gluLookAt).

	Finally, expand this matrix into a 4x4 (by adding a 1 in the lower-right corner and zeros elsewhere), and multiply it by a translation matrix that
	takes your camera location to the origin (i.e. the translation is by the negative of the camera location). And you've got a look-at matrix.
*/
	//we have the forward vector, we cross that with the up vector to get S

	//then cross S with F to get U - but doesn't this give us a vector we've already got?
	/*
		then,:
      |	s[0]   s[1]   s[2]  0  |
      |	u[0]   u[1]   u[2]  0  |
  M = |			               |
      |-f[0]  -f[1]  -f[2]  0  |
      |	 0	   0       0    1  |

  and gluLookAt	is equivalent to glMultMatrixf(M);
  glTranslated (-eyex, -eyey, -eyez);
*/

	Vector3f v3S = v3UV * v3VV; //v3dVV * v3dUV;
	v3S.MakeUnitVector();
	Vector3f v3U = v3VV * v3S;
	//v3dS.MakeUnitVector();
	v3U.MakeUnitVector();

	//it does seem that in order to make this work properly we do need to invert the view vector (map it down the negative axis) - i don't know why,
	//but things don't come out as expected if we don't - even if we hard code the view matrix, we need to invert the view vector.

	//Based on receiving truly perpendicular view and up vectors, this implementation produces the correct right vector and produces the correct
	//visual result:
	a_m44Out.Init(v3S.X, v3S.Y, v3S.Z, 0.0f,
				  v3U.X, v3U.Y, v3U.Z, 0.0f,
				  v3VV.X, v3VV.Y, v3VV.Z, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);

/*
	a_m44Out.Init(v3dS.X(), v3dS.Y(), v3dS.Z(), 0.0f,
				  v3dUV.X(), v3dUV.Y(), v3dUV.Z(), 0.0f,
				  -v3dVV.X(), -v3dVV.Y(), -v3dVV.Z(), 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
*/
	return a_m44Out;
}

void Matrix4x4f::SetViewMatrix(Matrix3x3f& m33Axes)
{
	Init(m33Axes[0],m33Axes[1],m33Axes[2],0.0f,
		 m33Axes[3],m33Axes[4],m33Axes[5],0.0f,
		 m33Axes[6],m33Axes[7],m33Axes[8],0.0f,
		 0.0f,0.0f,0.0f,1.0f);

/*
	Init(m33Axes[0],	m33Axes[3],		m33Axes[6],		0,
		 m33Axes[1],	m33Axes[4],		m33Axes[7],		0,
		 -m33Axes[2],	-m33Axes[5],	-m33Axes[8],	0,
		 0,				0,				0,				1);
*/
}


Matrix4x4f& Matrix4x4f::RotateInWorldX(float fpDegrees)
{
	//0 degrees means horizontal, therefore we must use cos(0) = 1 as fpSinZ
	
	//float fpSinX = sin(fpD2R*fpDegrees);
	//float fpCosX = cos(fpD2R*fpDegrees);

	//Now that we've reversed the order in which these rotations happen, it seems we need to reverse the angle of rotation in X to make the visual
	//output correct.  as long as the points of the view frustum work out in the right spot, this is ok.  if not, we're going to have to revisit this.
	
	//this works out to be the case - an angle of 31 degrees came up with y being around +5, and z being around +8 - perfect!

	float fpSinX, fpCosX;
	float fpNegDeg = -fpDegrees;
	LookupTables::SineAndCosine(fpNegDeg, fpSinX, fpCosX);

	//We could eventually define a static rotation matrix for the 4 types - x, y, z, & vector rotations

	Matrix4x4f m44Out;
	Matrix4x4f m44RotX(1,0,0,0,  0,fpCosX, -fpSinX,0,  0,fpSinX,fpCosX,0,  0,0,0,1);
	Multiply(*this, m44RotX, m44Out);
	Copy(&m44Out, this);

	return *this;
}


Matrix4x4f& Matrix4x4f::RotateInWorldY(float fpDegrees)
{
	//We're going to invert the angle here because in our particular case, we want a rotation to be clockwise instead of counter-clockwise.  The rotation
	//matrix below gives us a counter-clockwise rotation about world Y.  It just happens that in the case of rotation around the x-axis, an increasing
	//angle coupled with a counter-clockwise rotation is actually want we want - as we increase our angle, we look up - but by definition, we want our
	//increasing angle in rotation about Y (0-359) to happen in a clockwise direction - so we have to invert the angle.

	float fpSinY=0;
	float fpCosY=0;
	LookupTables::SineAndCosine(fpDegrees, fpSinY, fpCosY);

	Matrix4x4f m44Out;
	Matrix4x4f m44RotY(fpCosY,0,fpSinY,0,  0,1,0,0,  -fpSinY,0,fpCosY,0,  0,0,0,1);
	Multiply(*this, m44RotY, m44Out);
	Copy(&m44Out, this);

	return *this;
}

Matrix4x4f& Matrix4x4f::RotateInLocalZ(float fpDegrees)
{
	float fpSinZ = 0;
	float fpCosZ = 0;
	float fpNegDeg = -fpDegrees;
	LookupTables::SineAndCosine(fpNegDeg, fpSinZ, fpCosZ);

	Matrix4x4f m44Out;
	Matrix4x4f m44RotZ(fpCosZ,-fpSinZ,0,0,  fpSinZ,fpCosZ,0,0,  0,0,1,0,  0,0,0,1);
	//Matrix4x4f::Multiply(m44RotZ, *this, m44Out);
	Matrix4x4f::Multiply(*this, m44RotZ, m44Out);
	Copy(&m44Out, this);

	return *this;
}
/*

Matrix3x3& Matrix4x4f::RotateInWorldZ(float& fpAngle)
{
	float fpSinZ = sin(fpAngle);
	float fpCosZ = cos(fpAngle);

	Matrix3x3 m33Out;
	Matrix3x3 m33RotZ(fpCosZ,-fpSinZ,0,  fpSinZ,fpCosZ,0,  0,0,1);
	Matrix3x3::Multiply(*this, m33RotZ, m33Out);
	Copy(&m33Out, this);

	return *this;
}

void Matrix4x4f::Transform(float* p_fpXIn, float* p_fpYIn, float* p_fpZIn, float* p_fpXOut, float* p_fpYOut, float* p_fpZOut, Int64 i64CountPoints)
{
	for (Int64 i64=0; i64<i64CountPoints; i64++)
	{
		p_fpXOut[i64] = m_fpElements[0] * p_fpXIn[i64] + m_fpElements[1] * p_fpYIn[i64] + m_fpElements[2] * p_fpZIn[i64];
		p_fpYOut[i64] = m_fpElements[3] * p_fpXIn[i64] + m_fpElements[4] * p_fpYIn[i64] + m_fpElements[5] * p_fpZIn[i64];
		p_fpZOut[i64] = m_fpElements[6] * p_fpXIn[i64] + m_fpElements[7] * p_fpYIn[i64] + m_fpElements[8] * p_fpZIn[i64];
	}
}
*/
