#ifndef geometry_point_h
#define geometry_point_h

template <class FPType> class Vector2;
template <class FPType> class Vector3;
template <class FPType> class Vector4;

template <class FPType> class Point2
{
public:
	FPType X;
	FPType Y;

	inline Point2()
	{	
		X = 0;
		Y = 0;
	}

	inline Point2(const FPType a_fpX, const FPType a_fpY):X(a_fpX), Y(a_fpY){}

	inline void Init(const FPType a_fpX, const FPType a_fpY)
	{
		X = a_fpX;
		Y = a_fpY;
	}

	inline FPType DistanceFrom(Point2<FPType>& a_pt)
	{
		FPType fpDeltaX, fpDeltaY;
		fpDeltaX = (X - a_pt.X);
		fpDeltaY = (Y - a_pt.Y);
		FPType fpRes = sqrt((fpDeltaX*fpDeltaX) + (fpDeltaY*fpDeltaY));
		return fpRes;
	}

	inline Point2<FPType> operator+(Vector2<FPType>& a_v2)
	{
		Point2<FPType> p2Res;
		p2Res.X = X + a_v2.X;
		p2Res.Y = Y + a_v2.Y;

		return p2Res;
	}
};
typedef Point2<double> Point2d;
typedef Point2<float> Point2f;


template <class FPType> class Point3
{
public:
	FPType X;
	FPType Y;
	FPType Z;

	inline Point3<FPType>()
	{	
		X = 0;
		Y = 0;
		Z = 0;
	}

	inline Point3<FPType>(const FPType a_fpX, const FPType a_fpY, const FPType a_fpZ):X(a_fpX), Y(a_fpY), Z(a_fpZ){}

	inline void Init(const FPType a_fpX, const FPType a_fpY, const FPType a_fpZ)
	{
		X = a_fpX;
		Y = a_fpY;
		Z = a_fpZ;
	}

	inline FPType DistanceFrom(Point3<FPType>& a_pt)
	{
		FPType fpDeltaX, fpDeltaY, fpDeltaZ;
		fpDeltaX = (X - a_pt.X);
		fpDeltaY = (Y - a_pt.Y);
		fpDeltaZ = (Z - a_pt.Z);
		FPType fpRes = sqrt((fpDeltaX*fpDeltaX) + (fpDeltaY*fpDeltaY) + (fpDeltaZ*fpDeltaZ));
		return fpRes;
	}

	inline FPType DistanceToOrigin()
	{
		FPType fpRes = sqrt((X*X) + (Y*Y) + (Z*Z));
		return fpRes;
	}

	//inline Point3<FPType>& operator+=(Vector3<FPType>& a_v3t);

	inline Vector3<FPType>& GetAsPositionVector(Vector3<FPType>& a_v3t)
	{
		a_v3t.Init(X, Y, Z);
		return a_v3t;
	}

	inline Point3<FPType>& operator+=(Vector3<FPType>& a_v3t)
	{
		X += a_v3t.X;
		Y += a_v3t.Y;
		Z += a_v3t.Z;

		return *this;
	}
};

typedef Point3<double> Point3d;
typedef Point3<float> Point3f;


template <class FPType> class Point4
{
public:
	FPType X;
	FPType Y;
	FPType Z;
	FPType W;

	inline Point4()
	{	
		X = 0;
		Y = 0;
		Z = 0;
		W = 1;
	}

	inline Point4(const FPType a_fpX, const FPType a_fpY, const FPType a_fpZ, const FPType a_fpW):X(a_fpX), Y(a_fpY), Z(a_fpZ), W(a_fpW){}

	inline void Init(const FPType a_fpX, const FPType a_fpY, const FPType a_fpZ, const FPType a_fpW)
	{
		X = a_fpX;
		Y = a_fpY;
		Z = a_fpZ;
		W = a_fpW;
	}

	inline Point3<FPType>& Convert3(Point3<FPType>& a_p3Out)
	{
		a_p3Out.Init(X, Y, Z);
		return a_p3Out;
	}

	//Perform an in-place translation
	inline Point4<FPType>& Translate(Vector4<FPType>& v4)
	{
		(*this) += v4;
		return *this;
	}

	/*
	inline Point4D& operator+(Vector4& v4d, Point4D p4dOut)
	{
		p4dOut.Init(X, Y, Z, W);
		p4dOut += v4d;

		return p4dOut;
	}
*/
	inline Point4<FPType>& operator+=(Vector4<FPType>& v4)
	{
		X += v4.X;
		Y += v4.Y;
		Z += v4.Z;

		return *this;
	}

	Vector4<FPType>& GetAsPositionVector(Vector4<FPType>& v4Out)
	{
		v4Out.Init(X, Y, Z, W);
		return v4Out;
	}
};
typedef Point4<double> Point4d;
typedef Point4<float> Point4f;

#endif