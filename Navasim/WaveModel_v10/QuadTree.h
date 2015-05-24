#include <Geometry/Primitives.h>

class IQuadTreeNode
{

};

//we can implement this class later on to make the parameter passing for the quadtreenode constructor a little cleaner and limit the amount of
//stuff we need to pass on the stack.  it will basically have left col, right col, top row, bottom row.
class QuadTreeNodeExtents
{
public:
	Int32 L;
	Int32 R;
	Int32 T;
	Int32 B;

	QuadTreeNodeExtents& operator = (QuadTreeNodeExtents& qtne)
	{
		L = qtne.L;
		R = qtne.R;
		T = qtne.T;
		B = qtne.B;

		return *this;
	}
};

//implements a vertex-centric quad tree.  need not be a square quadtree, but should have an odd number of vertices in either dimension.

/*FOR THE TIME BEING WE'RE GOING TO IMPLEMENT A SQUARE QUADTREE (2^n + 1) BUT WE MUST BE ABLE TO HANDLE NO-DATA SITUATIONS */

class QuadTreeRoot
{
public:
	Int32 m_i32Width;
	Int32 m_i32Height;
	Point3f* mp_p3fVertices;

	QuadTreeRoot(Int32 a_i32Width, Int32 a_i32Height, Point3f* ap_p3fVertices):m_i32Width(a_i32Width), m_i32Height(a_i32Height), mp_p3fVertices(ap_p3fVertices){}

	inline Int32 Width(){return m_i32Width;}

	inline Int32 Height(){return m_i32Height;}

	inline Point3f* Vertices(){return mp_p3fVertices;}
};

class QuadTreeNode : public IQuadTreeNode
{
private:
	bool m_blnHasChildNodes;
//	bool m_blnDividesHorizontally;
//	bool m_blnDividesVertically;
	Int32 m_i32Depth;

	QuadTreeRoot* mp_qtrRoot;
	//IQuadTreeNode* mp_qtnParent;	//for convenience - but lets not store this now so that we keep the size of the qt structure down.
	IQuadTreeNode*[4] mp_qtnChildren; //stored clockwise from TL, TR, BR, BL
	QuadTreeNodeExtents m_qtne;

	//We may even want to map this floating point value into a range of 0..255 to save space in memory - that will probably give sufficient
	//accuracy for this purpose.

	float m_fltFlatnessFactor;

public:

	//we even may be able to get away with loading a flatness quad tree as a staight data struct into memory, and instead of having links
	//between a parent and its child nodes, we actually lay it out in memory in a 4-branch extending tree, and based upon our knowledge of which
	//node we're dealing with, we can work out its address and just go straight to finding its value there (or the struct that represents the node).
	//this will be the approach we'll take when loading a quadtree into memory.  addressing would work as follows:

	//level 0 (root) = sizeof(the struct)
	//level 1 = sizeof(level 0) + (sizeof(the struct) * 4)
	//level 2 = sizeof(level 1) + (sizeof(the struct) * 16)

	//we can work out how big the structure will be...
	//struct size * ((2^n)*(2^n) + (2^(n-1))*(2^(n-1)) + (2^(n-2))*(2^(n-2)).. + 1 )

	//so, for a size of 8 polys wide, we'd have n being 3
	//so it would be: 8*8 + 4*4 + 2*2 + 1 = 64+16+4+1 = 85*struct size

	//if it is 128 polys wide (approx 1280m):
	//128*128 + 64*64 + 32*32 + 16*16 + 8*8 + 4*4 + 2*2 + 1
	//16384 + 4096 + 1024 + 256 + 64 + 16 + 4 + 1
	//= 21845 * struct size

	//it would be nice to have a class that can walk the struct, for example: starting from the root node, give me the address of the south-east
	//corner at the next level down, and then the address of the north-west corner at the next level down again, and then give me the address of its
	//3 sibling nodes, etc.  This way we avoid the whole struct of pointers to structs.

	//we could lay them out in memory such that each level is contiguous.  To navigate around at the root node's children, we start with the address
	//of the 2*2 grid, and it will be laid out in column major format.

	//but for the sake of generating the initial structure we'll proceed along this path.

	QuadTreeNode(QuadTreeRoot* ap_qtrRoot, IQuadTreeNode* ap_qtnParent, QuadTreeNodeExtents& a_qtne);
	{
		mp_qtrRoot = ap_qtrRoot;
		//mp_qtnParent = ap_qtnParent;
		mp_p3dVertexArray = ap_p3dVertexArray;

		if (ap_qtnParent != nullptr)
			m_i32Depth = ap_qtnParent.GetDepth()+1;
		else
			m_i32Depth = 0;

		m_qtne = a_qtne;

		//???? Check the extents given to us - if any of the points explained by these extents indicates no data, we won't subdivide.

	
		//Create a new quadtree node now until we get to the point that the vertex array indices are next to each other

		//if our extents passed in are 0 to 8 (1 to 9) our midpoint is going to be 4 (5) - ie, look at the difference between incoming indices
		//and then halve it.  If we have 5 points left, our midpoint is going to be 4-0 = 4, /2 = 2.
		//if we have 3 points left, our midpoint is going to be 2-0 = 2, /2 = 1.  At this point we stop subdividing.

		QuadTreeNodeExtents& e = m_qtne;

		//As we're using a square mesh these will always be the same.

		Int32 i32DeltaHoriz = e.R-e.L;
		//Int32 i32DeltaVert = e.B-e.T;
		blnDividesHorizontally = (i32DeltaHoriz>1);
		//blnDividesVertically = (i32DeltaVert>1);

		m_blnHasChildNodes = !(blnDividesHorizontally);// && blnDividesVertically);

		if (m_blnHasChildNodes)
		{
			mp_qtnChildren[0] = new QuadTreeNode(this, mp_p3dVertexArray, e.L,				 e.L+i32DeltaHoriz, e.T,			  e.T+i32DeltaVert)
			mp_qtnChildren[1] = new QuadTreeNode(this, mp_p3dVertexArray, e.L+i32DeltaHoriz, a_i32colR,			e.T,			  e.T+i32DeltaVert)
			mp_qtnChildren[2] = new QuadTreeNode(this, mp_p3dVertexArray, e.L+i32DeltaHoriz, a_i32ColR,			e.T+i32DeltaVert, e.B)
			mp_qtnChildren[3] = new QuadTreeNode(this, mp_p3dVertexArray, e.L,				 e.L+i32DeltaHoriz, e.T+i32DeltaVert, e.B)
		}

	}

	//we somehow need to be aware of where the edge of the coastline is - or do we want to include depth data in each tile?  we could keep
	//it flexible for the time being.

	//if we do this, we should also mark tiles as being all above-water, mixed, or all-below water, so we know that given our current HOE,
	//we can exclude submarine tiles from the rendering pipeline if we're above water.  this doesn't mean to say that they shouldn't be loaded
	//in shallow water conditions, because we will need the depth data to influence water colour and such.  deeper tiles that don't affect won't
	//affect water colour and may not need to be loaded (unless we're simulating submarine travel).

	inline Int32 GetDepth()
	{
		return m_i32Depth;
	}

	float CalculateFlatnessFactor()
	{
		if (m_blnHasChildNodes)
		{
			//If we have child nodes, recurse down and retrieve their flatness factors, accumulating the results

			for (int i=0; i<4; i++)
				m_fltFlatnessFactor += mp_qtnChildren[i]->CalculateFlatnessFactor();
		}
		else
		{
			//We're going to determine the surface normals for the north-west and south-east triangles, and determine the angle between those
			//two normals. (nw = tl, se = br)

			Vector3f v3fTL, v3fTR;
			FindTopLeftNormalVector(v3fTL);
			FindBottomRightNormalVector(v3fTR);

			//Knowing that the dot product of the two vectors is the cosine of the
			//angle between them, find the angle between the normal vectors.
			
			m_fltFlatnessFactor = acos(v3fTL.DotProduct(v3fTR));
		}

		return m_fltFlatnessFactor;
	}

	void FindTopLeftNormalVector(Vector3f& a_v3fTL)
	{
		//Get our top left, top right, and bottom left points.

		Point3f& p3fTL;
		Point3f& p3fTR;
		Point3f& p3fBL;

		//Make a plane from them

		Plane pln(p3fTL, p3fTR, p3fBL);

		//Return the normal vector

		a_v3fTL = pln.m_v3dNormal;
	}

	void FindBottomRightNormalVector(Vector3f& a_v3fBR)
	{
		Point3f& p3fTR;
		Point3f& p3fBR;
		Point3f& p3fBL;

		//Make a plane from them

		Plane pln(p3fTR, p3fBR, p3fBL);

		//Return the normal vector

		a_v3fBR = pln.m_v3dNormal;
	}
};

//when we build our run-time dynamic quadtree for determining visible vertices and polygons, the tree will only grow as deep as the flatness
//factor dictates.  this will be worked out once every x-frames, or even better once the POE has moved far enough away from the point of last
//generation that it is worthwhile re-doing it.  Even if we re-gen the quadtree for flatness, we will still need to regen the clipped one because
//the view vector may change considerably, even given the same POE.