#pragma once

#include <Geometry/Primitives.h>
#include <VirtualSpace/ISceneObject.h>
#include <Geodetic/SpheroidSurfaceMesh.h>

class OceanSurface: public SpheroidSurfaceMesh
{
private:
	Spheroid*			mp_sphSubject;
	ISceneObject*		mp_p3fMesh;

public:
	OceanSurface(void);
	~OceanSurface(void);

	//We're going to use the interface definition here instead of the concrete class because at some point we may choose to handle the mesh implementation
	//differently, by (for example) pushing points out into a spherical section.

	void Init(Spheroid* ap_sphSubject, ISceneObject* ap_p3fm);
	
	void Render(HGLRC* ap_glcContexts);

	Int64* GetPolygonList();
};
