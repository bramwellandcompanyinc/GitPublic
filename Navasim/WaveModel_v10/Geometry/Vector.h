#include "GenericConstants.h"

template <class FPType> class Vector2
{
public:
	FPType X;
	FPType Y;
	FPType m_fpLength;

	Vector2()
	{
		m_fpLength=0;
		X = 0;
		Y = 0;
	}

	inline Vector2(const FPType a_fpX, const FPType a_fpY):X(a_fpX), Y(a_fpY){m_fpLength=0;}

	inline Vector2(const Int32 a_i32Heading, const FPType a_fpLength)
	{
		//We need to adjust this to make 0 degrees due north, not due east

		Init(a_i32Heading, a_fpLength);
	}

	inline void Init(const FPType a_fpX, const FPType a_fpY)
	{
		X = a_fpX;
		Y = a_fpY;
	}

	inline void Init(const Int32 a_i32Heading, const FPType a_fpLength)
	{
		//We need to adjust this to make 0 degrees due north, not due east

		X = static_cast<float>(cos(a_i32Heading*PI/180) * a_fpLength);
		Y = static_cast<float>(sin(a_i32Heading*PI/180) * a_fpLength);
		m_fpLength = a_fpLength;
	}

	inline FPType Length()
	{
		//I don't normally like having more than one exit point but we want to minimise the number of jumps we do for functions like this.

		if (m_fpLength > 0)
			return m_fpLength;
		else
		{
			/*
			if (X > a distance || Y > a distance)
			{
				//do a lookup - the mesh is going to be detailed closer to the
				//viewer.  The lookup table will be a 1024x1024 texture, where we can do
				//lookups if the integer component is -1024 < x < 1024, including 1024
				//bits of precision for the fractional part. Ie, a 1MB texture.  Even though
				//we may need to do a control transfer after the lookup, I'm sure this would be faster.
				//(Maybe the compiler will put the "else" case after the most common path.)
			}
			else*/
				m_fpLength = sqrt(X*X + Y*Y);
				//m_fpLength = HypotenuseLU(X, Y);

			return m_fpLength;
		}
	}

	inline Vector2<FPType> operator/(FPType a_fpDivisor)
	{
		Vector2<FPType> v2Res;
		v2Res.X = X / a_fpDivisor;
		v2Res.Y = Y / a_fpDivisor;
		if (v2Res.m_fpLength > 0)
			v2Res.Length(); //we have to reset this so it is correct given the new lengths

		return v2Res;
	}

	inline Vector2<FPType> operator*(FPType a_fpDivisor)
	{
		Vector2<FPType> v2Res;
		v2Res.X = X * a_fpDivisor;
		v2Res.Y = Y * a_fpDivisor;
		if (v2Res.m_fpLength > 0)
			v2Res.Length(); //we have to reset this so it is correct given the new lengths

		return v2Res;
	}

	inline void MakeUnitVector()
	{
		//Make sure the current length is set first

		m_fpLength = 0;
		Length();
		
		//if (m_fpLength != 1.0f)<FPType>
		if (m_fpLength != 1)
		{
			X /= m_fpLength;
			Y /= m_fpLength;
		}
		m_fpLength = 1.0f;
	}
};
typedef Vector2<double> Vector2d;
typedef Vector2<float> Vector2f;


template <class FPType> class Vector3
{
public:
	FPType X;
	FPType Y;
	FPType Z;
	FPType m_fpLength;

	inline Vector3()
	{
		m_fpLength=0;
		X = 0;
		Y = 0;
		Z = 0;
	}

	inline Vector3(const FPType a_fpX, const FPType a_fpY, const FPType a_fpZ):X(a_fpX), Y(a_fpY), Z(a_fpZ){m_fpLength=0;}

	//Cosntruct a vector using two points - this will go in the direction of p1 to p2

	inline Vector3(Point3<FPType>& a_p3t1, Point3<FPType>& a_p3t2)
	{
		Init(a_p3t1, a_p3t2);	
	}

	inline void Init(Point3<FPType>& a_p3t1, Point3<FPType>& a_p3t2)
	{
		Init(a_p3t2.X - a_p3t1.X, a_p3t2.Y - a_p3t1.Y, a_p3t2.Z - a_p3t1.Z);
	}

	inline void Init(const FPType a_fpX, const FPType a_fpY, const FPType a_fpZ)
	{
		X = a_fpX;
		Y = a_fpY;
		Z = a_fpZ;
	}

	inline FPType Length()
	{
		//I don't normally like having more than one exit point but we want to minimise the number of jumps we do for functions like this.

		if (m_fpLength > 0)
			return m_fpLength;
		else
		{
			m_fpLength = sqrt(X*X + Y*Y + Z*Z);
			return m_fpLength;
		}
	}

	//the copy implementation
	inline Vector3 operator+(Vector3& a_v3t)
	{
		Vector3 v3dRes;
		v3dRes.X = X + a_v3t.X;
		v3dRes.Y = Y + a_v3t.Y;
		v3dRes.Z = Z + a_v3t.Z;

		return v3dRes;
	}

	//the in-place implementation
	inline Vector3& operator+=(Vector3& a_v3t)
	{
		X += a_v3t.X;
		Y += a_v3t.Y;
		Z += a_v3t.Z;

		return *this;
	}

	inline Vector3 operator*=(FPType a_fpMultiplier)
	{
		X *= a_fpMultiplier;
		Y *= a_fpMultiplier;
		Z *= a_fpMultiplier;
		if (m_fpLength > 0)
			Length(); //we have to reset this so it is correct given the new lengths

		return *this;
	}

	inline Vector3 operator* (Vector3& a_v3t)
	{
		Vector3 v3dRes;
		Vector3& a = *this;
		Vector3& b = a_v3t;
		v3dRes.X = a.Y*b.Z - a.Z*b.Y;
		v3dRes.Y = a.Z*b.X - a.X*b.Z;
		v3dRes.Z = a.X*b.Y - a.Y*b.X;

		return v3dRes;
	}

	//scalar divide
	inline Vector3& operator/=(double a_dbl)
	{
		X /= a_dbl;
		Y /= a_dbl;
		Z /= a_dbl;

		return *this;
	}

	inline bool operator==(Vector3& a_v3t)
	{
		return ( (a_v3t.X == X) && (a_v3t.Y == Y) && (a_v3t.Z == Z) );
	}

	inline void MakeUnitVector()
	{
		//Make sure the current length is set first

		m_fpLength = 0;
		Length();

		if (m_fpLength != 1.0f)
		{
			X /= m_fpLength;
			Y /= m_fpLength;
			Z /= m_fpLength;
		}
		m_fpLength = 1.0f;
	}

	//The cross product of two vectors is: a × b = (a2b3 − a3b2, a3b1 − a1b3, a1b2 − a2b1)
	static inline Vector3& Cross(Vector3& a_v3tLeft, Vector3& a_v3tRight, Vector3& a_v3tOut)
	{
		Vector3& a = a_v3tLeft;
		Vector3& b = a_v3tRight;
		a_v3tOut.X = a.Y*b.Z - a.Z*b.Y;
		a_v3tOut.Y = a.Z*b.X - a.X*b.Z;
		a_v3tOut.Z = a.X*b.Y - a.Y*b.X;

		return a_v3tOut;
	}

	inline Vector3& Cross(Vector3& a_v3tWith, Vector3& a_v3tOut)
	{
		return Cross(*this, a_v3tWith, a_v3tOut);
	}

	inline Point3<FPType>& Translate(Point3<FPType>& a_p3tIn, Point3<FPType>& a_p3tOut)
	{
		a_p3tOut.Init(a_v3tIn.X+X, a_v3tIn.Y+Y, a_v3tIn.Z+Z);
		return a_p3tOut;
	}

	static inline Vector3& Multiply(Vector3& a_v3t, FPType a_fpScalar)
	{
		a_v3t.X *= a_fpScalar;
		a_v3t.Y *= a_fpScalar;
		a_v3t.Z *= a_fpScalar;
		return a_v3t;
	}

	inline Vector3& Multiply(FPType a_fpScalar, Vector3& a_v3tOut)
	{
		a_v3tOut.X = X * a_fpScalar;
		a_v3tOut.Y = Y * a_fpScalar;
		a_v3tOut.Z = Z * a_fpScalar;
		return a_v3tOut;
	}

	inline FPType DotProduct(Vector3<FPType>& a_v3tRight)
	{
		FPType fpOut = 0;
		fpOut = X*a_v3tRight.X + Y*a_v3tRight.Y + Z*a_v3tRight.Z;
		return fpOut;
	}
};

typedef Vector3<double> Vector3d;
typedef Vector3<float> Vector3f;


template <class FPType> class Vector4
{
public:
	FPType X;
	FPType Y;
	FPType Z;
	FPType W;

	Vector4()
	{
	}

	Vector4(FPType a_X, FPType a_Y, FPType a_Z, FPType a_W)
	{
		Init(a_X, a_Y, a_Z, a_W);
	}

	Vector4<FPType>& Init(FPType a_X, FPType a_Y, FPType a_Z, FPType a_W)
	{
		X = a_X;
		Y = a_Y;
		Z = a_Z;
		W = a_W;

		return *this;
	}

	inline Vector4<FPType>& operator *=(FPType fpScalar)
	{
		X *= fpScalar;
		Y *= fpScalar;
		Z *= fpScalar;

		return *this;
	}
};
typedef Vector4<double> Vector4d;
typedef Vector4<float> Vector4f;
