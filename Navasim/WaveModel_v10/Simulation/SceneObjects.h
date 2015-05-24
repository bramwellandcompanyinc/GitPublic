//#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <VirtualSpace/ISceneObject.h>
#include <Geometry/Primitives.h>
#include <Wave/WaveModel.h>
/*
	PFNGLBINDBUFFERARBPROC glBindBufferARB;
	PFNGLGENBUFFERSARBPROC glGenBuffersARB;
	PFNGLBUFFERDATAARBPROC glBufferDataARB;
*/
//we want to declare the sub, terrain tile, water, sphere here

class SubmarineSceneObject : public ISceneObject
{
private:
	bool m_blnSubMade;
	int m_intSubRenderList;
	GLUquadricObj* gluQ;

public:
	virtual ~SubmarineSceneObject(){}
	void OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Render();
	void Deallocate();
};


class HonoluluSceneObject : public ISceneObject
{
private:
	PFNGLBINDBUFFERARBPROC glBindBufferARB;// = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
	PFNGLGENBUFFERSARBPROC glGenBuffersARB;// = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
	PFNGLBUFFERDATAARBPROC glBufferDataARB;// = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
	Point3f* mp_p3fPoints;
	float* mp_fltNormals;
	Point3d m_p3dAnchor;
	int* mp_i64Primitives;
	Int64 m_i64Rows;
	Int64 m_i64Cols;
	int m_intNumPrims;

public:
	virtual ~HonoluluSceneObject(){}
	void OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Render();
	void Deallocate();
};


class WaterSceneObject : public ISceneObject
{
private:
	Point2f p2fWaveOrigin;

	//This combination of waves is as good as I've gotten so far...

	WaveForm wavPrimary1;
	WaveForm wavPrimary2;
	WaveForm wavPrimary3;
	WaveForm wavPrimary4;
	WaveForm wavPrimary5;
	WaveForm wavPrimary6;

	static const long gc_MeshWidth = 200; //metres
	static const long gc_MeshWidthHalf = gc_MeshWidth/2;
	float fltWaveData[gc_MeshWidth][gc_MeshWidth];
	Vector3f v3fNormals[gc_MeshWidth][gc_MeshWidth];
	float fltVertices[gc_MeshWidth][gc_MeshWidth][3];
	float fltNormals[gc_MeshWidth][gc_MeshWidth][3];
	int intVertexIndices[(gc_MeshWidth-1)][(gc_MeshWidth-1)][4];
	float m_fltScaleFactor;

	void AdvanceWaveFronts();
	void ModulateSurfaceMesh();
	void DrawWaves();

public:
	virtual ~WaterSceneObject(){}
	void OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Render();
	void Deallocate();
};


class ReferenceSphereSceneObject : public ISceneObject
{
private:
	GLUquadricObj* gluQ;

public:
	virtual ~ReferenceSphereSceneObject(){}
	void OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Render();
	void Deallocate();
};

class ReferenceSquareSceneObject : public ISceneObject
{
private:

public:
	virtual ~ReferenceSquareSceneObject(){}
	void OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf);
	void Render();
	void Deallocate();
};
