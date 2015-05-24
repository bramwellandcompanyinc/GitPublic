#include "StdAfx.h"
#include "Assert.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <Geometry/Primitives.h>
#include <Simulation/simulation.h>
#include <Simulation/SceneObjects.h>

void Simulation::OnExitCommand(UInt32 ui32Param)
{
	m_blnTerminateSimulation = true;
}

void Simulation::OnKeyDown(UInt32 ui32Param)
{
	switch (ui32Param)
	{
	case 16: //shift keys
		m_blnShiftKeyDown = true;
		break;
	case 65: //a
		//align to view in the NE direction
		mp_vw->Look(45);
		break;
	case 79: //o
		//reset eye point to the origin
		mp_vw->ToOrigin();
		break;
	case 76: //l (little L) Set eye to horizontal and level
		mp_vw->LevelHead();
		mp_vw->LevelTilt();
		break;
	case 102: //keypad 6
		m_dirHeading = HeadClockwise;
		break;
	case 100: //keypad 4
		m_dirHeading = HeadAnticlockwise;
		break;
	case 104: //keypad 8
		m_dirUpDown = Up;
		break;
	case 98: //keypad 2
		m_dirUpDown = Down;
		break;
	case 97: //keypad 1 (tilt left)
		m_dirTilt = TiltAnticlockwise;
		break;
	case 99: //keypad 3 (tilt right)
		m_dirTilt = TiltClockwise;
		break;
	case 69: //e
		if (!m_blnShiftKeyDown)
			m_dirTranslateEastWest = TranslatePositive;
		else
			mp_vw->Look(90);
		break;
	case 87: //w
		if (!m_blnShiftKeyDown)
			m_dirTranslateEastWest = TranslateNegative;
		else
			mp_vw->Look(270);
		break;
	case 78: //n
		if (!m_blnShiftKeyDown)
			m_dirTranslateNorthSouth = TranslatePositive;
		else
			mp_vw->Look(0);
		break;
	case 83: //s
		if (!m_blnShiftKeyDown)
			m_dirTranslateNorthSouth = TranslateNegative;
		else
			mp_vw->Look(180);
		break;
	case 85: //u
		if (!m_blnShiftKeyDown)
			m_dirTranslateVertical = TranslatePositive;
		else
			mp_vw->RotateInY(90);
		break;
	case 68: //d
		if (!m_blnShiftKeyDown)
			m_dirTranslateVertical = TranslateNegative;
		else
			mp_vw->RotateInY(-90);
		break;
	case 71: // g - forwards in direction of view vector
		m_dirTranslateAlongView = TranslatePositive;
		break;
	case 66: // b - backwards in direction of view vector
		m_dirTranslateAlongView = TranslateNegative;
		break;
/*
	case 82: //r - look back (and reverse) current view vector to the rear
		blnReverseView = true;
		break;
		*/
	}
}

void Simulation::OnKeyUp(UInt32 ui32Param)
{
	switch (ui32Param)
	{
	case 16:	//shift keys
		m_blnShiftKeyDown = false;
		break;
	case 102: //keypad 6
	case 100: //keypad 4
		m_dirHeading = HeadingNoDelta;
		break;
	case 104: //keypad 8
	case 98: //keypad 2
		m_dirUpDown = Level;
		break;
	case 97: //keypad 1 (tilt left)
	case 99: //keypad 3 (tilt right)
		m_dirTilt = TiltLevel;
		break;
	case 69: //e
	case 87: //w
		m_dirTranslateEastWest = TranslateNone;
		break;
	case 78: //n
	case 83: //s
		m_dirTranslateNorthSouth = TranslateNone;
		break;
	case 85: //u
	case 68: //d
		m_dirTranslateVertical = TranslateNone;
		break;
	case 71: // g - forwards in direction of view vector
	case 66: // b - backwards in direction of view vector
		m_dirTranslateAlongView = TranslateNone;
		break;
	case 82: // r
		//blnReverseView = false;
		break;
	};
}

void Simulation::OnChar(UInt32 ui32Param)
{
	switch (ui32Param)
	{
	case 'p':
//		blnPause = !blnPause;
		break;
	case 27:
	case 13:
		m_blnTerminateSimulation = true;
		break;
	case 'f':
/*
		if (!m_blnRenderWireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
*/
		m_blnRenderWireFrame = !m_blnRenderWireFrame;
		break;
	}
}
void Simulation::OnRightButtonDown()
{
}

void Simulation::OnLeftButtonDown()
{
}


void Simulation::OnLeftButtonDoubleClick()
{
	this->TerminateLoop();
}



