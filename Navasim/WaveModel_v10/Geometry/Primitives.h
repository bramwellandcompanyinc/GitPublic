#ifndef geometry_primitives_h
#define geometry_primitives_h

//We're going to provide 0-parm constructors for each of these objects because they are small and we don't want to be concerning ourselves with new/delete (and all the
//overhead that entails) nor with pooling objects that we will potentially have so many of.  Therefore, we're going to take the chance that we use an uninitialised object
//as a trade for the performance gain.  This is a little dangerous admittedly but we don't want to be having too much overhead with creating a simple struct.  This will
//permit us to have local instances on the stack instead of on the heap.  We'll just have to initialise them with an init method instead of a constructor in most cases.

#include <Geometry/Point.h>
#include <Geometry/Vector.h>
#include <Geometry/Line.h>
#include <Geometry/Plane.h>
#include <Geometry/Converters.h>

#endif