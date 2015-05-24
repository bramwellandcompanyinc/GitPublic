#include "stdafx.h"
#include "GenericConstants.h"
#include <GL/gl.h>
#include <Simulation/SceneObjects.h>
#include <Wave/WaveModel.h>

void WaterSceneObject::Deallocate()
{
}

void WaterSceneObject::OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{
	p2fWaveOrigin.Init(0.0f, 0.0f);

	//This combination of waves is as good as I've gotten so far...
	//With the peaking factors below, these waves look pretty good too.  If we increase peaking by 0.1, we start to witness aliasing along the peak of the
	//wave.  This is still a workable input set though, and once we start to bump-map the surface we might not even notice the aliasing.

	wavPrimary1.Init(1.0f, 0.5f, 40.0f, 5, 5.0f, p2fWaveOrigin);
	wavPrimary2.Init(0.7f, 0.5f, 41.0f, 35, 6.0f, p2fWaveOrigin);
	wavPrimary3.Init(0.2f, 0.4f, 18.0f, 245, 7.0f, p2fWaveOrigin);
	wavPrimary4.Init(0.9f, 0.45f, 39.0f, 8, 5.3f, p2fWaveOrigin);
	wavPrimary5.Init(0.6f, 0.40f, 42.0f, 38, 6.3f, p2fWaveOrigin);
	wavPrimary6.Init(0.15f, 0.35f, 17.0f, 242, 6.1f, p2fWaveOrigin);

	wavPrimary1.ResetGenesisTime();
	wavPrimary2.ResetGenesisTime();
	wavPrimary3.ResetGenesisTime();
	wavPrimary4.ResetGenesisTime();
	wavPrimary5.ResetGenesisTime();
	wavPrimary6.ResetGenesisTime();

	//Generate trochoid wave form data

	wavPrimary1.GenerateWaveformHeightData();
	wavPrimary2.GenerateWaveformHeightData();
	wavPrimary3.GenerateWaveformHeightData();
	wavPrimary4.GenerateWaveformHeightData();
	wavPrimary5.GenerateWaveformHeightData();
	wavPrimary6.GenerateWaveformHeightData();

	for (int i = 0; i < gc_MeshWidth-1; i++)
	{
		for (int j = 0; j < gc_MeshWidth-1; j++)
		{
			intVertexIndices[i][j][0] = i*gc_MeshWidth + j;
			intVertexIndices[i][j][1] = (i+1)*gc_MeshWidth + j;
			intVertexIndices[i][j][2] = (i+1)*gc_MeshWidth + j + 1;
			intVertexIndices[i][j][3] = i*gc_MeshWidth + j + 1;
		}
	}

	m_fltScaleFactor = 0.01f;
}

void WaterSceneObject::Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{
	GLfloat mat_shininess[] = {35};
	GLfloat mat_specular[] = {0.8f, 0.8f, 0.8f, 1};
	GLfloat mat_blue[] = {0.0f, 0.0f, .6f, 1.0f};

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_blue);
}

void WaterSceneObject::Render()
{


	//Draw the "horizon" square

	GLfloat mat_water[] = {0.2f, 0.2f, 0.9f, 1.0f};
	GLfloat mat_blue[] = {0.0f, 0.0f, .6f, 1.0f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_water);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_blue);

	glBegin(GL_QUADS);
		glVertex3f(-20.0f,0.001f,20.0f);
		glVertex3f(20.0f,0.001f,20.0f);
		glVertex3f(20.0f,0.001f,-20.0f);
		glVertex3f(-20.0f,0.001f,-20.0f);
	glEnd();

	AdvanceWaveFronts();
	ModulateSurfaceMesh();
	DrawWaves();
}


void WaterSceneObject::ModulateSurfaceMesh(/*OceanSurface& a_os, Spheroid& a_sph, SpheroidSurfaceMesh& a_ssm*/)
{

	//If our HOE is within 10m or so of our last calculated mesh, just return that mesh....

	/* TO BE IMPLEMENTED */

	//Take the spheroid earth and ask it to provide us a spheroid surface mesh based on the current view space and frustum representing our position
	//on earth and how wide our fov is

	//If we're constructing the default space trapezoid we don't really need the view space, just the frustum
//	sphEarth.ConstructDefaultSurfaceMeshTrapezoid(ssmMesh /*, vsViewSpace*/, vsViewFrustum);

	//Rotate the points into world space (ultimately this will be on the gpu but it will need to return a vertex buffer.  We can already do vertex buffers
	//on the GPU without needing to use CUDA for it... lets get the single cpu idea working first though.  But if we do, can we get the vertex buffer back
	//from the GPU without CUDA?

	//Firstly lets construct an ocean surface using the spheroid surface, so that we can start using a more abstract approach to wave generation.

//	a_os.Init(&a_sph, &a_ssm);

	//Get the collection of regular surface waves we have active for our current region

	//SwellWave* p_sw; //etc...

	//Get the collection of local waves that are active for our region

	//LocalWave* p_lw; //this will be refracted waves, etc

	//WakeWaves* p_ww;	//any waves active for boat wakes in our region

	//TerrainMap tm;	//the terrain mesh providing depth and elevation data, which will affect the points we do wave height calcs for, and possibly provides for
					//the modulation of wave height in locality to shallow water


	//os.ApplyShoreLineAndDepthEffects(tm);
	// or
	//os.ResolveShoreline(tm);
	//and when we do our wave sampling we modulate wave height then using the terrain map as well.

	//Sample the wave heights at each point on the mesh (and ultimately, apply the influence of any local phenomena) and push the mesh points out from the
	//centre of the spheroid appropriately

	//When it comes to determining whether a polygon is under land, we could provide a 2D texture (black and white).  If, for our current position we get a
	//white texel from a lookup then we're under land, or something similar??  dunno yet, maybe, it has issues.






	register Int32 i, j;
	Vector3f v3f1;
	Vector3f v3f2;
	Vector3f v3f3;
	Vector3f v3f4;
	Vector3f v3f5;
	Vector3f v3f6;

	
	for (i = 0; i < gc_MeshWidth; i++)
	{
		float x = (i-gc_MeshWidthHalf);
		float xScaled = x * m_fltScaleFactor;

		for (j = 0; j < gc_MeshWidth; j++)
		{
			float z = (j-gc_MeshWidthHalf);
			fltVertices[i][j][0] = xScaled;
			fltVertices[i][j][2] = z * m_fltScaleFactor;

			//with the sampling commented out we get 58 fps vs 21.7 fps
			Point2f pt(x, z);
			float fltHeight = 0;

			fltHeight = wavPrimary1.SampleWaveHeight(pt, v3f1);
			fltHeight += wavPrimary2.SampleWaveHeight(pt, v3f2);
			fltHeight += wavPrimary3.SampleWaveHeight(pt, v3f3);
			fltHeight += wavPrimary4.SampleWaveHeight(pt, v3f4);
			fltHeight += wavPrimary5.SampleWaveHeight(pt, v3f5);
			fltHeight += wavPrimary6.SampleWaveHeight(pt, v3f6);
			
			fltVertices[i][j][1] = fltHeight * m_fltScaleFactor;

						
			Vector3f v3fNormal;
			v3fNormal += v3f1;
			v3fNormal += v3f2;
			v3fNormal += v3f3;
			v3fNormal += v3f4;
			v3fNormal += v3f5;
			v3fNormal += v3f6;


//			v3fNormal.MakeUnitVector(); //it is preferable to use glNormalize and have the gpu do this instead

			fltNormals[i][j][0] = v3fNormal.X;
			fltNormals[i][j][1] = v3fNormal.Y;
			fltNormals[i][j][2] = v3fNormal.Z;
/*
			if (i%8 == 0 && j%8 == 0)
			{
			glBegin(GL_LINES);
				glVertex3f(fltVertices[i][j][0], fltVertices[i][j][1], fltVertices[i][j][2]);
				glVertex3f(fltVertices[i][j][0] + (v3fNormal.X * m_fltScaleFactor), fltVertices[i][j][1] + (v3fNormal.Y * m_fltScaleFactor), fltVertices[i][j][2] + (v3fNormal.Z * m_fltScaleFactor));
			glEnd();
			}
*/
		}
	}

	glVertexPointer(3, GL_FLOAT, 0, fltVertices);
	glNormalPointer(GL_FLOAT, 0, fltNormals);
}


void WaterSceneObject::DrawWaves()
{
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_NORMAL_ARRAY);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glDrawElements(GL_QUADS, (gc_MeshWidth-1)*(gc_MeshWidth-1)*4, GL_UNSIGNED_INT, intVertexIndices);
}

void WaterSceneObject::AdvanceWaveFronts(/*bool blnPause*/)
{
	//if (!blnPause)
	//{
		wavPrimary1.AdvanceWave();
		wavPrimary2.AdvanceWave();
		wavPrimary3.AdvanceWave();
		wavPrimary4.AdvanceWave();
		wavPrimary5.AdvanceWave();
		wavPrimary6.AdvanceWave();
	//}
}