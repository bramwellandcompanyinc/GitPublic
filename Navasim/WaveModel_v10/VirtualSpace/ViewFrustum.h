//Nicknames for each point array index

#pragma once
#include <VirtualSpace/IViewFrustum.h>
#include <VirtualSpace/IViewSpace.h>

//These are defined in clock-wise order from the north-western most point (top left) in all cases

#define FTL 0
#define FTR 1
#define FBR 2
#define FBL 3
#define NTL 4
#define NTR 5
#define NBR 6
#define NBL 7

#define TL 0
#define TR 1
#define BR 2
#define BL 3

//Indexes for the far/near face lines

#define FT 0
#define FR 1
#define FB 2 
#define FL 3
#define NT 4
#define NR 5
#define NB 6
#define NL 7

#define NUM_PTS 8
#define NUM_CORNERS 4
#define NUM_FACE_EDGES 8
#define EDGES_PER_FACE 4;

class ViewFrustumIntersectionSet;

class ViewFrustum : public IViewFrustum
{
private:
	float m_fltNearPlaneZ;
	float m_fltFarPlaneZ;
	float m_fltAspectRatio;
	float m_fltFieldOfViewInY;

	Point3d m_p3dEye;
	IViewSpace* mp_vwViewSpace;

	//This will hold all 8 points of the frustum as seen looking down the +Z axis using the identity view matrix

	Point3f m_p3fFrustumDefault[NUM_PTS];

	//This will hold all 8 points after rotation into world space

	Point3f m_p3fFrustumWorld[NUM_PTS];

	//This will hold the lines connecting the far 4 corners to their corresponding near 4 corners

	//LineSegment3D m_ls3dCorners[NUM_CORNERS];
	Line3f m_ls3fCorners[NUM_CORNERS];

	//This will hold the 8 lines that describe the edges of each of the near and far faces

	Line3f m_l3fFaceEdges[NUM_FACE_EDGES];

	//Populates the lines that describe the frustum corners and frustum near/far edges

	void CalculateFrustumLineEquations();


public:
	ViewFrustum();

	ViewFrustum(float a_fltNearPlaneZ, float a_fltFarPlaneZ, float a_fltFieldOfViewInY, float a_fltAspectRatio);

	void Init(float a_fltNearPlaneZ, float a_fltFarPlaneZ, float a_fltFieldOfViewInY, float a_fltAspectRatio);

	//Takes the data provided in the constuctor and populates the default frustum points array, which we'll use as the source for rotation into world space

	IViewFrustum& GenerateFrustum();

	//IViewFrustum& PositionInWorldSpace(Point3d& a_p3dEye, Matrix4x4& m44WorldAxes);
	IViewFrustum& PositionInWorldSpace(IViewSpace* ap_vsViewSpace);

	void Render();

	float GetDiagonalFieldOfView();
};
