#pragma once

#include <Geometry/Primitives.h>

template <class FPType> class Point2;
template <class FPType> class Point3;
template <class FPType> class Point4;
template <class FPType> class Vector3;
template <class FPType> class Vector4;
template <class FPType> class Line3;
template <class FPType> class Line4;
template <class FPType> class Plane;

//This class is needed because we based the vector class on a template, and therefore we cannot code explicit knowledge of the template parameter type
//of the vector to convert between 3d and 3f.  We therefore want some static methods to do this instead.

namespace Geometry
{
	class Point2
	{
	public:
		static void ConvertF2D(Point2f& a_p2fIn, Point2d& a_p2dOut)
		{
			a_p2dOut.Init(a_p2fIn.X, a_p2fIn.Y);
		}

		static void ConvertD2F(Point2d& a_p2dIn, Point2f& a_p2fOut)
		{
			a_p2fOut.Init((float)a_p2dIn.X, (float)a_p2dIn.Y);
		}
	};

	class Point3
	{
	public:
		static void ConvertF2D(Point3f& a_p3fIn, Point3d& a_p3dOut)
		{
			a_p3dOut.Init(a_p3fIn.X, a_p3fIn.Y, a_p3fIn.Z);
		}

		static void ConvertD2F(Point3d& a_p3dIn, Point3f& a_p3fOut)
		{
			a_p3fOut.Init((float)a_p3dIn.X, (float)a_p3dIn.Y, (float)a_p3dIn.Z);
		}
	};

	class Vector3
	{
	public:
		static void ConvertF2D(Vector3f& a_v3fIn, Vector3d& a_v3dOut)
		{
			a_v3dOut.Init(a_v3fIn.X, a_v3fIn.Y, a_v3fIn.Z);
		}

		static void ConvertD2F(Vector3d& a_v3dIn, Vector3f& a_v3fOut)
		{
			a_v3fOut.Init((float)a_v3dIn.X, (float)a_v3dIn.Y, (float)a_v3dIn.Z);
		}
	};
}