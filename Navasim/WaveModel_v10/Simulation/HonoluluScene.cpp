//#include <Terrain/DigitalElevationModelParser.h>
//#include <Terrain/HonoluluScene.h>

#include "stdafx.h"
#include <GL/gl.h>
#include <Simulation/SceneObjects.h>

using namespace std;
#include <iostream>
#include <fstream>



	
void HonoluluSceneObject::OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{
	//Anchor this tile at the lat/long 0 N 90 E

	m_p3dAnchor.Init(6400000, 0, 0);

	//Load a terrain tile from disk - for now we're going to use a model of O'ahu

	//string strPath("C:\\SourceCode\\_Navasim\\Maps\\oahu10m.asc");
	string strPath("C:\\OnlineData\\Google Drive\\Source\\GitPrivate\\Navasim\\WaveModel_v10\\oahu10m.asc");
	//string strPath("C:\\SourceCode\\_Navasim\\Maps\\shane_cheary16075.asc");
	ifstream ifsDEM(strPath.c_str(), ios::in);

	int cols = 1814;
	int rows = 863;
	int nodata = -9999;
//		int cols = 3425;
//		int rows = 3004;
//		int nodata = -999999;
	m_i64Rows = rows;
	m_i64Cols = cols;
	char chrBuffer[20000];

	if (ifsDEM.is_open())
	{
		//Read thru the first 6 lines where the fie metadata is stored.

		for (int i=0; i<6; i++)
			ifsDEM.getline(chrBuffer, 20000);

		mp_p3fPoints = new Point3f[cols*rows];
		mp_fltNormals = new float[3*cols*rows];
		int p = 0;
		for (int i=0; i<rows; i++)
		{
			for (int j=0; j<cols; j++)
			{
				p = (i*cols) + j;
				mp_p3fPoints[p].X = ((float)j)/100.0;
				mp_p3fPoints[p].Z = -((float)i)/100.0;
				float fltY; 
				ifsDEM >> fltY >> ws;
				if (fltY != nodata)
					mp_p3fPoints[p].Y = fltY/500.0;
				else
					mp_p3fPoints[p].Y = fltY;
			}
		}
		ifsDEM.close();

		//set up the quads

		mp_i64Primitives = new int[cols*rows*sizeof(int)];
		Int64 i64I=0;
		m_intNumPrims=0;
		int j;
		for (int i = 0; i < rows-1; i++)
		{	
			for (j = 0; j < cols-1; j++)
			{
				bool blnNoData = false;

				blnNoData = blnNoData || (mp_p3fPoints[i*cols + j].Y == nodata);
				blnNoData = blnNoData || (mp_p3fPoints[(i+1)*cols + j].Y == nodata);
				blnNoData = blnNoData || (mp_p3fPoints[(i+1)*cols + j + 1].Y == nodata);
				blnNoData = blnNoData || (mp_p3fPoints[i*cols + j + 1].Y == nodata);

				if (!blnNoData)
				{
					*(mp_i64Primitives+i64I) = i*cols + j;
					*(mp_i64Primitives+i64I+1) = (i+1)*cols + j;
					*(mp_i64Primitives+i64I+2) = (i+1)*cols + j + 1;
					*(mp_i64Primitives+i64I+3) = i*cols + j + 1;
					m_intNumPrims++;
					i64I += 4;
				}
			}
		}

		//Setup the normals

		//do the inside points first

		//this may be a yucky way to approach this because we'll be working out all the slopes more than once, but it will do for now
		//until I'm prepared to think about it some more

		for (int i=1; i<rows-1; i++)
		{
			for (int j=1; j<cols-1; j++)
			{
				//Get the differences in Y between each of the point's neighbours.

				float fltNormalSlopeLeft = mp_p3fPoints[i*cols + j-1].Y - mp_p3fPoints[i*cols + j].Y;
				float fltNormalSlopeRight = mp_p3fPoints[i*cols + j].Y - mp_p3fPoints[i*cols + j+1].Y;
				float fltNormalSlopeUp = mp_p3fPoints[(i-1)*cols + j].Y - mp_p3fPoints[i*cols + j].Y;
				float fltNormalSlopeDown = mp_p3fPoints[i*cols + j].Y - mp_p3fPoints[(i+1)*cols + j].Y;

				Vector3f v3f((fltNormalSlopeLeft+fltNormalSlopeRight)/2, .01, (fltNormalSlopeUp+fltNormalSlopeDown)/2);
				v3f.MakeUnitVector();
				mp_fltNormals[3*(i*cols+j)] = v3f.X;
				mp_fltNormals[3*(i*cols+j)+1] = v3f.Y;
				mp_fltNormals[3*(i*cols+j)+2] = v3f.Z;
			}
		}


		glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");

			//			// Generate And Bind The Vertex Buffer
	//GLuint uintVertexBuffer;
	//glGenBuffersARB( 1, &uintVertexBuffer );					// Get A Valid Name
	//glBindBufferARB( GL_ARRAY_BUFFER_ARB, uintVertexBuffer );			// Bind The Buffer
	//glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_i64Rows*m_i64Cols*3*sizeof(float), mp_p3fPoints, GL_STATIC_DRAW_ARB );
	//glVertexPointer(3, GL_FLOAT, 0, 0);
	////delete[] mp_p3fPoints;

	//GLuint uintNormalBuffer;
	//glGenBuffersARB( 1, &uintNormalBuffer );					// Get A Valid Name
	//glBindBufferARB( GL_ARRAY_BUFFER_ARB, uintNormalBuffer );			// Bind The Buffer
	//glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_i64Rows*m_i64Cols*3*sizeof(float), mp_fltNormals, GL_STATIC_DRAW_ARB );
	//glNormalPointer(GL_FLOAT, 0, 0);
	////delete[] mp_fltNormals;

	//GLuint uintIndexBuffer;
	//glGenBuffersARB( 1, &uintIndexBuffer );					// Get A Valid Name
	//glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, uintIndexBuffer );			// Bind The Buffer
	//glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_intNumPrims*4*sizeof(int), mp_i64Primitives, GL_STATIC_DRAW_ARB );
	////delete[] mp_i64Primitives;

		//glBindBuffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
		//glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
		//glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");

	}
	else
		throw;

}


void HonoluluSceneObject::Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{
	//glVertexPointer(3, GL_FLOAT, 0, mp_p3fPoints);
	//glNormalPointer(GL_FLOAT, 0, mp_fltNormals);

	//			// Generate And Bind The Vertex Buffer
	//GLuint uintVertexBuffer;
	//glGenBuffersARB( 1, &uintVertexBuffer );					// Get A Valid Name
	//glBindBufferARB( GL_ARRAY_BUFFER_ARB, uintVertexBuffer );			// Bind The Buffer
	//glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_i64Rows*m_i64Cols*3*sizeof(float), mp_p3fPoints, GL_STATIC_DRAW_ARB );
	//glVertexPointer(3, GL_FLOAT, 0, 0);
	////delete[] mp_p3fPoints;

	//GLuint uintNormalBuffer;
	//glGenBuffersARB( 1, &uintNormalBuffer );					// Get A Valid Name
	//glBindBufferARB( GL_ARRAY_BUFFER_ARB, uintNormalBuffer );			// Bind The Buffer
	//glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_i64Rows*m_i64Cols*3*sizeof(float), mp_fltNormals, GL_STATIC_DRAW_ARB );
	//glNormalPointer(GL_FLOAT, 0, 0);
	////delete[] mp_fltNormals;

	//GLuint uintIndexBuffer;
	//glGenBuffersARB( 1, &uintIndexBuffer );					// Get A Valid Name
	//glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, uintIndexBuffer );			// Bind The Buffer
	//glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_intNumPrims*4*sizeof(int), mp_i64Primitives, GL_STATIC_DRAW_ARB );
	////delete[] mp_i64Primitives;
}

void HonoluluSceneObject::Render()
{
//glEnableClientState(GL_VERTEX_ARRAY);
//glEnableClientState(GL_NORMAL_ARRAY);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glVertexPointer(3, GL_FLOAT, 0, mp_p3fPoints);
	glNormalPointer(GL_FLOAT, 0, mp_fltNormals);

	//glDrawElements(GL_QUADS, m_intNumPrims*4, GL_UNSIGNED_INT, 0);//mp_i64Primitives);
	glDrawElements(GL_QUADS, m_intNumPrims*4, GL_UNSIGNED_INT, mp_i64Primitives);

//	glDisableClientState(GL_VERTEX_ARRAY);                // deactivate vertex array
//	glDisableClientState(GL_NORMAL_ARRAY);                // deactivate vertex array

	// bind with 0, so, switch back to normal pointer operation

//	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
//	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}

void HonoluluSceneObject::Deallocate()
{

}
