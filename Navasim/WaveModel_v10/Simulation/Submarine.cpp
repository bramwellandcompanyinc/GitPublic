#include "stdafx.h"
#include <GL/gl.h>
#include <Simulation/SceneObjects.h>

void SubmarineSceneObject::OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{
	m_blnSubMade = false;
	gluQ = gluNewQuadric();
}

void SubmarineSceneObject::Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{
	GLfloat mat_shininess[] = {35};
	GLfloat mat_specular[] = {0.8f, 0.8f, 0.8f, 1};
	GLfloat mat_blue[] = {0.0f, 0.0f, .6f, 1.0f};

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_blue);
}

void SubmarineSceneObject::Render()
{
	if (m_blnSubMade)
	{
		glCallList(m_intSubRenderList);
	}
	else
	{
		glPushMatrix();
		m_intSubRenderList = glGenLists(1);
		glNewList(m_intSubRenderList, GL_COMPILE_AND_EXECUTE);
		glTranslatef(-0.1f, 0, 0);

		GLfloat mat_grey[] = {.5f, .5f, .5f, .5f};
		GLfloat mat_black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_grey);
		glScalef(.1f, .1f, .1f);

		glTranslatef(0, 0.08f, 0);
		gluCylinder(gluQ, 0.2f, 0.2f, 1.2f, 24, 1);
		//glRotatef(5*rx, 0.0f, 0.0f, 1.0f);
		glPushMatrix();
		glScalef(1.0f, 1.0f, 2.0f);
		gluSphere(gluQ, 0.2f, 24, 24);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, +.5f);
		glRotatef(-90, 1, 0, 0);
		glScalef(0.4f, 1.0f, 1.0f);
		gluCylinder(gluQ, .2f, .2f, .4f, 24, 1);
		glPopMatrix();
		
		glTranslatef(0.0f, 0.0f, +1.2f);
		glPushMatrix();
		glScalef(1.0f, 1.0f, 6.0f);
		gluSphere(gluQ, 0.2f, 24, 24);
		glPopMatrix();
		
		glEndList();
		m_blnSubMade = true;
	}
}

void SubmarineSceneObject::Deallocate()
{
	gluDeleteQuadric(gluQ);
}