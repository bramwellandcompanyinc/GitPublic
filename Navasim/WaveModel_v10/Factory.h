#include <VirtualSpace/ISceneObject.h>
#include <VirtualSpace/IViewFrustum.h>
#include <VirtualSpace/IViewSpace.h>

class Factory
{
public:
	static IViewSpace* new_ViewSpace();
	static IViewFrustum* new_ViewFrustum();

	static ISceneObject* new_SubmarineSceneObject();
	static ISceneObject* new_ReferenceSquareSceneObject();
	static ISceneObject* new_ReferenceSphereSceneObject();
	static ISceneObject* new_WaterSceneObject();
	static ISceneObject* new_HonoluluSceneObject();
};