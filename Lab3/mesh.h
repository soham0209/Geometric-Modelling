// ------------------------------------------------------------
// Mesh.h
// ------------------------------------------------------------
//
// ------------------------------------------------------------

#ifndef MESHCLASS
#define	MESHCLASS

#include <stdio.h>
#include <vector>
#include <cmath>
#include <set>


// Change to double for more precision
typedef		float	datatype;


using namespace std;


// ------------------------------------------------------------
// GeomVert: this class holds the geometric coordinates of a vertex
// ------------------------------------------------------------
class GeomVert {
public:
	GeomVert(datatype x, datatype y, datatype z) { mCo[0] = x; mCo[1] = y; mCo[2] = z; }

	datatype      GetCo(int axis)                { return mCo[axis]; }

	bool operator == (GeomVert &A)               { 
		return ( (mCo[0] == A.GetCo(0)) && (mCo[1] == A.GetCo(1)) && (mCo[2] == A.GetCo(2)) );		
	}
	datatype dotProduct(GeomVert y) {
		return (mCo[0] * y.GetCo(0) + mCo[1] * y.GetCo(1) + mCo[2] * y.GetCo(2));
	}
	float length() {
		return (float)sqrt(mCo[0] * mCo[0] + mCo[1] * mCo[1] + mCo[2] * mCo[2]);
	}
	
private:
	datatype	mCo[3];
};
// ------------------------------------------------------------





// ------------------------------------------------------------
// TopoVert: this class holds all of a vertex's topological (connectivity) information
// ------------------------------------------------------------
class TopoVert {
public:
	TopoVert()                      { };
	~TopoVert()                     { mIncVerts.clear(); mIncEdges.clear(); mIncFacets.clear(); }
	void AddIncVert (int vert_ind ) { mIncVerts.insert( vert_ind ); }
	//void AddIncVert (int vert_ind ) { mIncVerts.push_back( vert_ind ); }
	void AddIncEdge (int edge_ind ) { mIncEdges.push_back( edge_ind ); }
	void AddIncFacet(int facet_ind) { mIncFacets.push_back( facet_ind ); }
	
    int GetNumberIncVertices()		{ return (int)mIncVerts.size(); }
	int GetIncVertex(int vert_ind)  { 
		set<int>::iterator sit = mIncVerts.begin(); for (int i = 0; i < vert_ind; i++) sit++;		
		return *sit;
	}
    int GetNumberIncEdges()			{ return (int)mIncEdges.size(); }
    int GetIncEdge(int edge_ind)    { return mIncEdges[edge_ind]; }
    int GetNumberIncFacets()	    { return (int)mIncFacets.size(); }
	int GetIncFacet(int facet_ind)  { return mIncFacets[facet_ind]; }

private:
	set<int>    mIncVerts;
	vector<int> mIncEdges;
	vector<int> mIncFacets;
	
};
// ------------------------------------------------------------





// ------------------------------------------------------------
// TopoEdge
// ------------------------------------------------------------
class TopoEdge {
public:
	TopoEdge()                      { v1 = v2 = -1; }
	~TopoEdge()                     { mIncFacets.clear(); }

	bool operator == (TopoEdge &A)  {
		return (  ((v1 == A.GetVertex(0)) && (v2 == A.GetVertex(1))) || ((v2 == A.GetVertex(0)) && (v1 == A.GetVertex(1))) );
	}

	int  GetVertex(int ind)         { if (ind == 0) return v1;  return v2; }
	void SetVertex(int ind, int v)  { if (ind == 0) { v1 = v; } else { v2 = v; } }
	
	void AddIncFacet(int facet_ind) { mIncFacets.push_back(facet_ind); }
    int  GetNumberIncFacets()       { return (int)mIncFacets.size(); }
	int  GetIncFacet(int facet_ind) { return mIncFacets[facet_ind]; }

private:
	int v1, v2;
	vector<int> mIncFacets;
};
// ------------------------------------------------------------





// ------------------------------------------------------------
// TopoFacet:  this class holds a facet's topological connectivity) information
//             Facets are represented as a list of vertex indices
// ------------------------------------------------------------
class TopoFacet {
public:
	TopoFacet()                     { };
	~TopoFacet()                    { mIncVerts.clear(); mIncEdges.clear();  mIncFacets.clear(); }
	void AddIncVertex(int v_ind)    { mIncVerts.push_back( v_ind ); }
	void AddIncEdge(int e_ind)      { mIncEdges.push_back( e_ind ); }
	void AddIncFacet(int f_ind)     { mIncFacets.insert( f_ind ); }
    int  GetNumberVertices()        { return (int)mIncVerts.size(); }
	int  GetVertexInd(int vert_ind) { return mIncVerts[vert_ind]; }
    int  GetNumberEdges()		    { return (int)mIncEdges.size(); }
	int  GetIncEdge(int edge_ind)   { return mIncEdges[edge_ind]; }
    int  GetNumberFacets()		    { return (int)mIncFacets.size(); }
	int  GetIncFacet(int facet_ind) { 
		set<int>::iterator sit = mIncFacets.begin(); for (int i = 0; i < facet_ind; i++) sit++;		
		return *sit;
	}
	int orientation(GeomVert p, GeomVert q, GeomVert r)
	{
		// Reference: https://www.geeksforgeeks.org/orientation-3-ordered-points/
		// for details of below formula.
		float a = p.GetCo(0);
		float b = p.GetCo(1);
		float c = p.GetCo(2);

		float d = q.GetCo(0);
		float e = q.GetCo(1);
		float f = q.GetCo(2);

		float g = r.GetCo(0);
		float h = r.GetCo(1);
		float k = r.GetCo(2);

		float val = (a*e*k + b * f*g + c * d*h) - (b*d*k + a * f*h + c * e*g);

	
		if (val == 0) return 0;  // colinear

		return (val > 0) ? 1 : 2; // clock or counterclock wise
	}
	bool doIntersect(GeomVert p1, GeomVert q1, GeomVert p2, GeomVert q2)
	{
		// Find the four orientations needed for general and
		// special cases
		int o1 = orientation(p1, q1, p2);
		int o2 = orientation(p1, q1, q2);
		int o3 = orientation(p2, q2, p1);
		int o4 = orientation(p2, q2, q1);

		// General case
		if (o1 != o2 && o3 != o4)
			return true;

		return false;
	}
private:
	vector<int> mIncVerts;	
	vector<int> mIncEdges;
	set<int>    mIncFacets;
};
// ------------------------------------------------------------






// ------------------------------------------------------------
// Mesh:  This class uses all the preceding classes to represent a mesh with
//        adjacency.connectivity information
// ------------------------------------------------------------
class Mesh {
private:
	void Erase();
public:
	Mesh()  { };
	~Mesh() { Erase(); };

	void      AddFacet(datatype x1, datatype y1, datatype z1, datatype x2, datatype y2, datatype z2, datatype x3, datatype y3, datatype z3);
	void      AddFacet(vector<GeomVert> geomfacet);
	
    int		  GetNumberVertices()		  { return (int)mGeomVerts.size(); }
    int		  GetNumberEdges()			  { return (int)mTopoEdges.size(); }
    int       GetNumberFacets()           { return (int)mTopoFacets.size(); }

	TopoVert  GetVertex(int vert_ind)     { return mTopoVerts[vert_ind]; }
	TopoEdge  GetEdge(int edge_ind)       { return mTopoEdges[edge_ind]; }
	TopoFacet GetFacet(int facet_ind)     { return mTopoFacets[facet_ind]; }
	
	GeomVert  GetGeomVertex(int vert_ind) { return mGeomVerts[vert_ind]; }
    int       FindGeomVertex(GeomVert v);


private:

	int		  FindTopoEdge(TopoEdge e);

	vector<GeomVert>  mGeomVerts;
	vector<TopoVert>  mTopoVerts;
	vector<TopoEdge>  mTopoEdges;
	vector<TopoFacet> mTopoFacets;
};
// ------------------------------------------------------------

#endif
