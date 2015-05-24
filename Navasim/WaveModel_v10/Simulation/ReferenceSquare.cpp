#include "stdafx.h"
#include <GL/gl.h>
#include <Simulation/SceneObjects.h>

void ReferenceSquareSceneObject::OneTimeInit(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{

}

void ReferenceSquareSceneObject::Generate(IViewSpace* ap_vs, IViewFrustum* ap_vf)
{
	GLfloat mat_shininess[] = {35};
	GLfloat mat_specular[] = {0.8f, 0.8f, 0.8f, 1};
	GLfloat mat_sphere[] = {0.0f, 0.6f, .6f, 1.0f};

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_sphere);
}

void ReferenceSquareSceneObject::Render()
{
	glPushMatrix();

		glBegin(GL_LINES);
		{
			glVertex3f(-.1f, .1f, .25);
			glVertex3f(.1f, .1f, .25);

			glVertex3f(.1f, .1f, .25);
			glVertex3f(.1f, -.1f, .25);

			glVertex3f(.1f, -.1f, .25);
			glVertex3f(-.1f, -.1f, .25);

			glVertex3f(-.1f, -.1f, .25);
			glVertex3f(-.1f, .1f, .25);
		}
		glEnd();

	glPopMatrix();
}

void ReferenceSquareSceneObject::Deallocate()
{

}