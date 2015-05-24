#include "StdAfx.h"
//#include "Assert.h"
#include <Assert.h>
#include <GL/gl.h>
#include "LookupTables.h"
#include "HighResTimer.h"
#include <Geometry/Primitives.h>
#include <Simulation/Simulation.h>
#include <Factory.h>


Simulation::Simulation(IWindowEventSource* ap_wes)
{
	//Assert(ap_wes != nullptr);
	if (ap_wes == nullptr)
		throw;

	mp_wes = ap_wes;
	m_blnTerminateSimulation = false;
	m_blnShiftKeyDown = false;

	mp_vw = Factory::new_ViewSpace();
	mp_vfFrustum = Factory::new_ViewFrustum();

	m_fltHeadingVelocity = 25.0f;
	m_fltDeltaNodVelocity = 15.0f;
	m_fltDeltaTiltVelocity = 15.0f;
	m_fltTranslateVelocity = 0.5f;

	m_dirTranslateNorthSouth = TranslateNone;
	m_dirTranslateEastWest = TranslateNone;
	m_dirTranslateAlongView = TranslateNone;
	m_dirTranslateVertical = TranslateNone;

	m_dirHeading = HeadingNoDelta;
	m_dirUpDown = Level;
	m_dirTilt = TiltLevel;

	LookupTables::Init();
	HighResTimer::Init();

	this->EstablishVirtualSpace();

	//Create space for the sub, land, water, sphere

	this->OneTimeInitSceneObjects();

	m_dblLastFrameCompleteSeconds = 0;
}

void Simulation::EstablishVirtualSpace()
{
	UInt16 ui16WindowWidth = 0;
	UInt16 ui16WindowHeight = 0;
	mp_wes->GetWindowSize(ui16WindowWidth, ui16WindowHeight);

	//We want to specify a position behind the window to help determine the field of view in x and y.
	//for example, to work with a 45 degree field of view in X, we might want to be placing the view point
	//behind the window the same distance as the screen is wide.  just depends on how we want to specify it.
	//maybe we just deal with fov in x and work it out from there.
	//but then, seeing as the gluPerspective function takes the fov in Y, let's use that instead.
	float fltAspectRatio = (float)ui16WindowWidth/(float)ui16WindowHeight;

	float fltFieldOfViewInY = 25.0;
	float fltNearPlane = .001f;
	float fltFarPlane = 100.0f;

	glClearDepth(fltFarPlane);
	glEnable(GL_DEPTH_TEST);

	glViewport(0,0,ui16WindowWidth, ui16WindowHeight);

	mp_vfFrustum->Init(fltNearPlane, fltFarPlane, fltFieldOfViewInY, fltAspectRatio);
	mp_vfFrustum->GenerateFrustum();

	float fltDiag = mp_vfFrustum->GetDiagonalFieldOfView();

	Point3d p3dEye(0.0, 0.1, -2.0);
	mp_vw->Init(p3dEye, 0.0f, 0.0f, 0.0f);
}

void Simulation::OneTimeInitSceneObjects()
{
	m_i16SceneObjectCount = 5;
	mpp_scobObjects = new ISceneObject*[m_i16SceneObjectCount];

	mpp_scobObjects[0] = Factory::new_ReferenceSphereSceneObject();
	mpp_scobObjects[1] = Factory::new_SubmarineSceneObject();
	mpp_scobObjects[2] = Factory::new_WaterSceneObject();
	mpp_scobObjects[3] = Factory::new_ReferenceSquareSceneObject();
	mpp_scobObjects[4] = Factory::new_HonoluluSceneObject();
	
	for (UInt16 i16SceneObjectIndex=0; i16SceneObjectIndex<m_i16SceneObjectCount; i16SceneObjectIndex++)
	{
		ISceneObject* p_scob = mpp_scobObjects[i16SceneObjectIndex];
		p_scob->OneTimeInit(mp_vw, mp_vfFrustum);
	}

}

Simulation::~Simulation()
{
	ReleaseResources();
}

void Simulation::SetWindowCallback(IWindowEventSource* ap_wes)
{

}


void Simulation::RenderScene()
{
	mp_wes->StartRenderCycle();

	this->SetInitialSceneState();

	if (m_dblLastFrameCompleteSeconds == 0)
		m_dblLastFrameCompleteSeconds = HighResTimer::GetSeconds();

	//adjust view location and orientation accordingly

	this->ApplyRotations();
	mp_vfFrustum->PositionInWorldSpace(mp_vw);
	mp_vw->SetGlIdentLeftHand();
	mp_vw->Look();

	//capture rendering completion time

	m_dblLastFrameCompleteSeconds = HighResTimer::GetSeconds();

	//Render the objects

	for (UInt16 i16SceneObjectIndex=0; i16SceneObjectIndex<m_i16SceneObjectCount; i16SceneObjectIndex++)
	{
		ISceneObject* p_scob = mpp_scobObjects[i16SceneObjectIndex];
		this->SetDefaultSceneState();
		glPushMatrix();
			p_scob->Generate(mp_vw, mp_vfFrustum);
			p_scob->Render();
		glPopMatrix();
	}

	GLfloat mat_opaque[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_opaque);
	
	mp_vfFrustum->Render();
	this->DrawReferenceAxes();



	mp_wes->CompleteRenderCycle();
}

void Simulation::SetInitialSceneState()
{
	GLfloat fltLight[4];
	fltLight[0] = mp_vw->Eye().X;
	fltLight[1] = mp_vw->Eye().Y;
	fltLight[2] = mp_vw->Eye().Z;
	fltLight[3] = 1;
	GLfloat light0_white[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lmodel_ambient[] = {1, 1, 1, 1};

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, fltLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_white);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	//Sky colours

	GLfloat fogColor[4] = {.1f, .1f, .35f, 1.0f};
	GLfloat skyColor[4] = {.5f, .5f, 1.0f, 1.0f};

	glClearDepth(100.0f);
	glClearColor(skyColor[0], skyColor[1],skyColor[2],skyColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Fog section

	/*
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, skyColor);
	glFogf(GL_FOG_DENSITY, 0.1f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, -2);
	glFogf(GL_FOG_END, -7);
*/

	if (!m_blnRenderWireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Simulation::SetDefaultSceneState()
{
	//Shading model

	glShadeModel(GL_SMOOTH);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //best blend
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_NORMALIZE);
	//glPolygonMode(GL_FRONT, GL_FILL);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
}

void Simulation::ApplyRotations()
{
	//Determine the deltas given the coded linear and angular velocities and time since last rendering

	float fltHeadingDelta = 0;
	float fltDeltaEyeAngle = 0;
	float fltDeltaTiltAngle = 0;
	float fltTranslateDelta = 0;

	this->DetermineFrameMotionDeltas(fltHeadingDelta, fltDeltaEyeAngle, fltDeltaTiltAngle, fltTranslateDelta);

	//we need to change the way we think about heading because as we approach the poles the current model breaks badly.
	//additionally, we need to change the way that translation works - because now we need to be able to translate around the surface of the spheroid,
	//not just in a straight line, as in the flat-space model.

	if (m_dirHeading != HeadingNoDelta)
		mp_vw->RotateInY(fltHeadingDelta * (float)m_dirHeading);

	if (m_dirUpDown != Level)
		mp_vw->RotateInX(fltDeltaEyeAngle * (float)m_dirUpDown);

	if (m_dirTilt != TiltLevel)
		mp_vw->RotateInZ(fltDeltaTiltAngle * (float)m_dirTilt);


	//Translations

	if (m_dirTranslateEastWest != TranslateNone)
		mp_vw->TranslateInX(fltTranslateDelta * (float)m_dirTranslateEastWest);

	if (m_dirTranslateNorthSouth != TranslateNone)
		mp_vw->TranslateInZ(fltTranslateDelta * (float)m_dirTranslateNorthSouth);

	if (m_dirTranslateVertical != TranslateNone)
		mp_vw->TranslateInY(fltTranslateDelta * (float)m_dirTranslateVertical);

	mp_vw->ApplyRotations();

/*
	Matrix4x4d m44Reverse;

	if (blnReverseView)
	{
		m44Reverse.SetIdentity();
		m44Reverse.RotateInWorldY(fltHeadingView+180);
		m44Reverse.RotateInWorldX(fltVerticalViewAngleView);
		m44Reverse.RotateInLocalZ(fltTiltAngleView);
	}
*/
	if (m_dirTranslateAlongView != TranslateNone)
		mp_vw->TranslateAlongViewVector(fltTranslateDelta * (float)m_dirTranslateAlongView);

//	if (blnReverseView)
//		vsViewSpace.m_m44Axes.Copy(&m44Reverse, &(vsViewSpace.m_m44Axes));

}

void Simulation::DrawReferenceAxes()
{
	
	float s = 2;
	glBegin(GL_LINES);
		glPushMatrix();
		glVertex3f(s*0.0f,s*0.0f,s*0.0f);
		glVertex3f(s*0.1f,s*0.0f,s*0.0f);

		glVertex3f(s*0.1f,s*0.0f,s*0.0f);
		glVertex3f(s*0.095f,s*0.01f,s*0.0f);
		
		glVertex3f(s*0.0f,s*0.0f,s*0.0f);
		glVertex3f(s*0.0f,s*0.1f,s*0.0f);

		glVertex3f(s*0.0f,s*0.0f,s*0.0f);
		glVertex3f(s*0.0f,s*0.0f,s*0.1f);
		glPopMatrix();
	glEnd();
}



void Simulation::CommenceInputLoop()
{
	double dblStartTime = HighResTimer::GetSeconds();
	double dblFrameTime = 0;
	double dblFrameStart = 0;
	Int32 i32Frames = 0;

	AllocateResources();

	while (!m_blnTerminateSimulation)
	{
		double dblFrameTimeStart = HighResTimer::GetSeconds();

		this->RenderScene();

		dblFrameTime += (HighResTimer::GetSeconds() - dblFrameTimeStart);
		i32Frames++;

		mp_wes->YieldForEvents();
	}

	double dblStopTime = HighResTimer::GetSeconds();
	double dblFramesPerSec = ((double)i32Frames)/(dblStopTime-dblStartTime);
	dblFrameTime = dblFrameTime/(double)i32Frames;
	char chrBuffer[512];

	sprintf(chrBuffer, "Average frames per second %f, seconds per frame %f", (float)dblFramesPerSec, (float)dblFrameTime);
	LPCSTR strTitle = "Average frames per second";

	::MessageBoxA(NULL, chrBuffer, strTitle, 0);
}

void Simulation::TerminateLoop()
{
	m_blnTerminateSimulation = true;
}

void Simulation::AllocateResources()
{
}

void Simulation::ReleaseResources()
{
	for (UInt16 i16SceneObjectIndex=0; i16SceneObjectIndex<m_i16SceneObjectCount; i16SceneObjectIndex++)
	{
		ISceneObject* p_scob = mpp_scobObjects[i16SceneObjectIndex];
		p_scob->Deallocate();
		//delete p_scob;
	}

	delete mpp_scobObjects;
}

void Simulation::OnWindowResize()
{

}

void Simulation::DetermineFrameMotionDeltas(float& fltHeadingDelta, float& fltDeltaEyeAngle, float& fltDeltaTiltAngle, float& fltTranslateDelta)
{
	double dblCurrentSeconds = HighResTimer::GetSeconds();
	double dblDeltaSeconds = dblCurrentSeconds-m_dblLastFrameCompleteSeconds;

	fltHeadingDelta = m_fltHeadingVelocity * static_cast<float>(dblDeltaSeconds);
	fltDeltaEyeAngle = m_fltDeltaNodVelocity * static_cast<float>(dblDeltaSeconds);
	fltDeltaTiltAngle = m_fltDeltaTiltVelocity * static_cast<float>(dblDeltaSeconds);
	fltTranslateDelta = m_fltTranslateVelocity * static_cast<float>(dblDeltaSeconds);
}

