#include "stdafx.h"
#include <GL/gl.h>
#include <Simulation/SceneObjects.h>

void ReferenceSphereSceneObject::OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{
	gluQ = gluNewQuadric();
}

void ReferenceSphereSceneObject::Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{
	GLfloat mat_shininess[] = {35};
	GLfloat mat_specular[] = {0.8f, 0.8f, 0.8f, 1};
	GLfloat mat_sphere[] = {0.0f, 0.6f, .6f, 1.0f};

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_sphere);
}

void ReferenceSphereSceneObject::Render()
{
	glPushMatrix();
		glTranslatef(10,0,10);
		gluSphere(gluQ, 0.2, 24, 24);
	glPopMatrix();
}

void ReferenceSphereSceneObject::Deallocate()
{
	gluDeleteQuadric(gluQ);
}