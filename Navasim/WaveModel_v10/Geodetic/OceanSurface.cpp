#include "StdAfx.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include "OceanSurface.h"

OceanSurface::OceanSurface(void)
{
}

OceanSurface::~OceanSurface(void)
{
}

void OceanSurface::Init(Spheroid* ap_sphSubject, ISceneObject* ap_p3fm)
{
	mp_sphSubject = ap_sphSubject;
	mp_p3fMesh = ap_p3fm;
}

void OceanSurface::Render(HGLRC* ap_glcContexts)
{
	Int64 i63NumPoints = mp_p3fMesh->GetNumPoints();

	//I THINK WE NEED TO SWITCH THIS BACK TO BEING POINT3D INSTEAD - IF WE'RE TRANSFERRING A LARGE ARRAY OF POINT DATA TO THE GPU PER FRAME, THEN WE'RE GIVING
	//OURSELVES ABOUT 30% MORE WORK TO DO IF WE SUBMIT POINT4D - AND WE KNOW THAT WE REALLY ONLY CARE ABOUT X, Y, Z, AND NOT W.  THE ONLY REASON GOING TO
	//POINT4D INITIALLY MADE SENSE WAS THAT WE HAD IMPLEMENTED THE POINT ROTATION ALGORITHM IN ASSEMBLY, AND HAD FOUND A WAY TO DO IT IN WHICH 4D MADE FOR
	//BETTER ALIGNMENT AND SMALLER CODE.  BUT IT IS REALLY EFFECTIVELY A WASTE OF CYCLES, MEMORY, AND BUS BANDWIDTH.

	Point3f* p_p3fMesh = mp_p3fMesh->GetMeshPoints();
	Int64* p_i64Polygons = mp_p3fMesh->GetPolygonList();

	//make this function take an array of contexts - one for each cpu thread available to us.  this way, we can use multiple cores to submit the rendering
	//commands to opengl

	if (ap_glcContexts == NULL)
	{
		//Take this path if we want to use a single threaded approach to gl command submission.  We also need to consider getting the normal vectors
		//submitted for rendering, where do we do this ??????????????

		glShadeModel(GL_SMOOTH);
		glEnable(GL_NORMALIZE);
		glDrawElements(GL_QUADS, i63NumPoints, GL_UNSIGNED_INT, p_i64Polygons);
		glDisable(GL_NORMALIZE);
	}
	else
	{
		//submit the mesh to be rendered on multiple threads - as many as there are contexts.  The chances are that the CPU will never be able to push the
		//GPU all the way to its limits, so having multiple threads issuing commands is likely to be no real problem for the GPU.

		//	wglMakeCurrent(hdc, m_hrc);

		//do the good stuff here
	}
}

Int64* OceanSurface::GetPolygonList()
{
	return null;
}