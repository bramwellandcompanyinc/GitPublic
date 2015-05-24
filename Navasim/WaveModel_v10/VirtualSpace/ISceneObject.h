#pragma once
#include <VirtualSpace/IViewSpace.h>
#include <VirtualSpace/IViewFrustum.h>
//#include <Geometry/Primitives.h>

class ISceneObject
{
public:
	/*
	virtual Int64 GetNumPoints() = 0;
	virtual Point3f* GetMeshPoints() = 0;
	virtual Int64* GetPolygonList() = 0;
	*/

	virtual void OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf) = 0;
	virtual void Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf) = 0;
	virtual void Render() = 0;
	virtual void Deallocate() = 0;
};

//a class that has three arrays of x, y, and z - that provides better alignment of doubles (we don't have 8 spare unaligned bytes ref a 16-byte boundary)
//and may make it faster for doing simd math, and also we could potentially submit the array to gl in this form - but may be not.  if we cannot submit in this
//form then the class may be useless.  maybe we can use a stride argument to indicate how the data are structured.
//well, potentially - internally we could use the struct of arrays approach but when we're ready to submit to gl we change it to array of structs... nah
//it is easier to convert to a gpu approach if we keep the data closer together, side-by-side in memory.  how do we then deal with the alignment issue?
/*
class PointMesh3D
{


};
*/

//a templatised vector class definition for floats and doubles.

