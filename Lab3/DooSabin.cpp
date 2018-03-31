#include "mesh.h"
#include "geometry.h"
#include "Utils.h"
#include "DooSabin.h"
#include <vector>
#include <algorithm>
#include<iostream>
#include <stack>
#include <map>
using namespace std;

Mesh DooSabin::createDooSabin(Mesh m, int level) {
	Mesh outmesh;
	vector<GeomVert> newpoints;
	int num_facets = m.GetNumberFacets();
	for (int facetind = 0; facetind < num_facets; facetind++) {
		TopoFacet f = m.GetFacet(facetind);
		int vert_num = f.GetNumberVertices();
		int edge_num = f.GetNumberEdges();
		newpoint* n = new newpoint;
		float fpx = 0;
		float fpy = 0;
		float fpz = 0;
		//Compute face point
		for (int vertid = 0; vertid < vert_num; vertid++) {
			int vindex = f.GetVertexInd(vertid);
			GeomVert v = m.GetGeomVertex(vindex);
			fpx += v.GetCo(0);
			fpy += v.GetCo(1);
			fpz += v.GetCo(2);
		}
		fpx /= (float)vert_num;
		fpy /= (float)vert_num;
		fpz /= (float)vert_num;
		GeomVert face_point(fpx, fpy, fpz);
		for (int vertid = 0; vertid < vert_num; vertid++) {
			int vindex = f.GetVertexInd(vertid);
			TopoVert v = m.GetVertex(vindex);
			GeomVert p = m.GetGeomVertex(vindex);
			GeomVert* adjPoints[2];
			int match = 0;
			int edgenum = 0;
			while (match != 2 && edgenum <= edge_num) {
				int eind = f.GetIncEdge(edgenum);
				TopoEdge e = m.GetEdge(eind);
				if (e.GetVertex(0) == vindex || e.GetVertex(1) == vindex) {
					adjPoints[match] = &(m.GetGeomVertex(vindex));
					match++;
				}
				edgenum++;
			}
			if (match != 2) {
				cout << "Check edgepoint section" << endl;
			}
			fpx = (p.GetCo(0) + adjPoints[0]->GetCo(0)) / 2;
			fpy = (p.GetCo(1) + adjPoints[0]->GetCo(1)) / 2;
			fpz = (p.GetCo(2) + adjPoints[0]->GetCo(2)) / 2;
			GeomVert edge_point1(fpx, fpy, fpz);

			fpx = (p.GetCo(0) + adjPoints[1]->GetCo(0)) / 2;
			fpy = (p.GetCo(1) + adjPoints[1]->GetCo(1)) / 2;
			fpz = (p.GetCo(2) + adjPoints[1]->GetCo(2)) / 2;
			GeomVert edge_point2(fpx, fpy, fpz);

			fpx = (face_point.GetCo(0) + edge_point1.GetCo(0) + edge_point2.GetCo(0) + p.GetCo(0)) / 4;
			fpy = (face_point.GetCo(1) + edge_point1.GetCo(1) + edge_point2.GetCo(1) + p.GetCo(1)) / 4;
			fpz = (face_point.GetCo(2) + edge_point1.GetCo(2) + edge_point2.GetCo(2) + p.GetCo(2)) / 4;
			GeomVert pprime(fpx, fpy, fpz);
			newpoints.push_back(pprime);
			n->vert.push_back(pprime);
			n->parent[vindex] = (n->vert.size() - 1);
		}//For all vertex in a face

		outmesh.AddFacet(newpoints);
		newpoints.clear();
		list[facetind] = *n;

	}//For all faces

	int v_num = m.GetNumberVertices();
	for (int vertid = 0; vertid < v_num; vertid++) {
		vector<GeomVert> vpoints;
		TopoVert v = m.GetVertex(vertid);
		int adjfacets_num = v.GetNumberIncFacets();
        if(adjfacets_num>2){
		for (int findex = 0; findex < adjfacets_num; findex++) {
			int face_index = v.GetIncFacet(findex);
			newpoint fp = list[face_index];
			int p = fp.parent[vertid];
			vpoints.push_back(fp.vert.at(p));

		}
		outmesh.AddFacet(util.OrderPoints(vpoints));
		vpoints.clear();
        }
	}
	int e_num = m.GetNumberEdges();
	for (int edgeid = 0; edgeid < e_num; edgeid++) {
		vector<GeomVert> epoints;
		int f1, f2;
		TopoEdge e = m.GetEdge(edgeid);
		if (e.GetNumberIncFacets() > 1) {
			f1 = e.GetIncFacet(0);
			f2 = e.GetIncFacet(1);
			cout << f1 << " " << f2 << endl;

			TopoFacet fo = m.GetFacet(f1);
			TopoFacet ft = m.GetFacet(f2);
			int v1 = e.GetVertex(0);
			int v2 = e.GetVertex(1);
			newpoint n1 = list[f1];
			epoints.push_back(n1.vert.at(n1.parent[v2]));
			epoints.push_back(n1.vert.at(n1.parent[v1]));
			newpoint n2 = list[f2];
			epoints.push_back(n2.vert.at(n2.parent[v1]));
			epoints.push_back(n2.vert.at(n2.parent[v2]));
			outmesh.AddFacet(epoints);
			epoints.clear();
		}

	}
	if (level == 1)
		return outmesh;
	return createDooSabin(outmesh, level - 1);
}


