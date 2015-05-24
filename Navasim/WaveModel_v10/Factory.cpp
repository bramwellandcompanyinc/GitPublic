#include "stdafx.h"
#include "Factory.h"
#include <VirtualSpace/ViewFrustum.h>
#include <VirtualSpace/ViewSpace.h>
#include <Simulation/SceneObjects.h>

IViewSpace* Factory::new_ViewSpace()
{
	return (IViewSpace*)(new ViewSpace());
}

IViewFrustum* Factory::new_ViewFrustum()
{
	return (IViewFrustum*)(new ViewFrustum());
}

ISceneObject* Factory::new_ReferenceSphereSceneObject()
{
	return (ISceneObject*)(new ReferenceSphereSceneObject());
}

ISceneObject* Factory::new_ReferenceSquareSceneObject()
{
	return (ISceneObject*)(new ReferenceSquareSceneObject());
}

ISceneObject* Factory::new_WaterSceneObject()
{
	return (ISceneObject*)(new WaterSceneObject());
}

ISceneObject* Factory::new_SubmarineSceneObject()
{
	return (ISceneObject*)(new SubmarineSceneObject());
}

ISceneObject* Factory::new_HonoluluSceneObject()
{
	return (ISceneObject*)(new HonoluluSceneObject());
}

