#include "Loop.h"
#include "mesh.h"
#include "geometry.h"
using namespace std;

Mesh Loop::createLoop(Mesh m, int level)
{
	Mesh outmesh;
	int num_edges = m.GetNumberEdges();
	int num_vert = m.GetNumberVertices();
	for (int edgid = 0; edgid < num_edges; edgid++) {
		TopoEdge e = m.GetEdge(edgid);
		int r_index = e.GetVertex(0);
		int s_index = e.GetVertex(1);
		GeomVert R = m.GetGeomVertex(r_index);
		GeomVert S = m.GetGeomVertex(s_index);
		Vec3f r(R.GetCo(0), R.GetCo(1), R.GetCo(2));
		Vec3f s(S.GetCo(0), S.GetCo(1), S.GetCo(2));
		int bd_pt = e.GetNumberIncFacets();
		Vec3f E;
		if (bd_pt > 1) {
			TopoFacet t1 = m.GetFacet(e.GetIncFacet(0));
			TopoFacet t2 = m.GetFacet(e.GetIncFacet(1));
			int count = 0;
			int p_index = -1, q_index = -1;
			while (count < t1.GetNumberVertices()) {
				if (t1.GetVertexInd(count) != r_index && t1.GetVertexInd(count) != s_index) {
					p_index = t1.GetVertexInd(count);
					break;
				}
				count++;
			}
			count = 0;
			while (count < t2.GetNumberVertices()) {
				if (t2.GetVertexInd(count) != r_index && t2.GetVertexInd(count) != s_index) {
					q_index = t2.GetVertexInd(count);
					break;
				}
				count++;
			}
			if (p_index == -1 || q_index == -1) {
				cout << "CHECK HERE.NO P and Q is found" << endl;
				getchar();
			}
			GeomVert P = m.GetGeomVertex(p_index);
			GeomVert Q = m.GetGeomVertex(q_index);

			Vec3f p(P.GetCo(0), P.GetCo(1), P.GetCo(2));
			Vec3f q(Q.GetCo(0), Q.GetCo(1), Q.GetCo(2));

			E = (p + 3.0 * r + 3.0 * s + q) / 8.0;
		}
		else {
			int p = e.GetVertex(0);
			int q = e.GetVertex(1);
			GeomVert P = m.GetGeomVertex(p);
			GeomVert Q = m.GetGeomVertex(q);
			E = Vec3f((P.GetCo(0) + Q.GetCo(0)) / 2.0, (P.GetCo(1) + Q.GetCo(1)) / 2.0, (P.GetCo(1) + Q.GetCo(1)) / 2.0);
		}
		edge_map[edgid] = E;
	}
	for (int vert_id = 0; vert_id < num_vert; vert_id++) {
		TopoVert v = m.GetVertex(vert_id);
		Vec3f vavg(0, 0, 0);
		
		GeomVert vp = m.GetGeomVertex(vert_id);
		Vec3f V(vp.GetCo(0), vp.GetCo(1), vp.GetCo(2));

		int inc_edge_num = v.GetNumberIncEdges();
		for (int inc_edge = 0; inc_edge < inc_edge_num; inc_edge++) {
			int incedge_id = v.GetIncEdge(inc_edge);
			TopoEdge e = m.GetEdge(incedge_id);
			Vec3f pi;
			if (e.GetVertex(0) == vert_id) {
				GeomVert p(m.GetGeomVertex(e.GetVertex(1)));
				pi = Vec3f(p.GetCo(0), p.GetCo(1), p.GetCo(2));
			}
			else if (e.GetVertex(1) == vert_id) {
				GeomVert p(m.GetGeomVertex(e.GetVertex(0)));
				pi = Vec3f(p.GetCo(0), p.GetCo(1), p.GetCo(2));
			}
			else {
				cout << "Adjacent Vertex are not getting found" << endl;
				getchar();
			}
			vavg += pi;
		}
		//vavg /= (float)inc_edge_num;
		if (inc_edge_num > 2) {
			float beta;
			if (inc_edge_num > 3)
				beta = 3.0 / (8.0 * inc_edge_num);
			else
				beta = 3.0 / 16.0;



			Vec3f v_dot = ((1 - inc_edge_num * beta) * V) + (beta * vavg);
			vert_map[vert_id] = v_dot;
		}
		else {
			float beta = 1.0 / 8.0;
			Vec3f v_dot = beta * vavg + (3.0 / 4.0) * V;
			vert_map[vert_id] = v_dot;
		}
	}
	int num_facets = m.GetNumberFacets();
	for (int facet_id = 0; facet_id < num_facets; facet_id++) {
		TopoFacet f = m.GetFacet(facet_id);
		int num_edg_facet = f.GetNumberEdges();
		vector<GeomVert>meshpts;
		/*for (int edgid = 0; edgid < num_edg_facet; edgid++) {
			int eid = f.GetIncEdge(edgid);
			Vec3f ep1 = edge_map[eid];
			meshpts.push_back(GeomVert(ep1.x, ep1.y, ep1.z));
		}
		outmesh.AddFacet(util.OrderPoints(meshpts));
		meshpts.clear();
		*/
		int vert_facet_num = f.GetNumberVertices();
		for (int vid = 0; vid < vert_facet_num; vid++) {
			int vertex = f.GetVertexInd(vid);
			TopoVert v = m.GetVertex(vertex);
			int count = 0, match = 0;
			int edge_v[2];
			while (count < f.GetNumberEdges()) {
				TopoEdge e = m.GetEdge(f.GetIncEdge(count));
				if (e.GetVertex(0) == vertex || e.GetVertex(1) == vertex) {
					edge_v[match] = f.GetIncEdge(count);
					match++;
				}
				int eid = f.GetIncEdge(count);
				Vec3f edg_pt = edge_map[eid];
				meshpts.push_back(GeomVert(edg_pt.x, edg_pt.y, edg_pt.z));
				count++;
			}
			if (match != 2) {
				cout << "Facets do not contain 2 edges that have vertex common!!" << endl;
				getchar();
			}
			outmesh.AddFacet(meshpts);
			meshpts.clear();
			Vec3f vprime = vert_map[vertex];
			Vec3f ep1 = edge_map[edge_v[0]];
			Vec3f ep2 = edge_map[edge_v[1]];
			meshpts.push_back(GeomVert(vprime.x, vprime.y, vprime.z));
			//meshpts.push_back(m.GetGeomVertex(vertex));
			meshpts.push_back(GeomVert(ep1.x, ep1.y, ep1.z));
			meshpts.push_back(GeomVert(ep2.x, ep2.y, ep2.z));
			outmesh.AddFacet(util.OrderPoints(meshpts));
			meshpts.clear();
			//cout << "vp: "<< vprime.x << " " << vprime.y <<" "<< vprime.z << endl;
			//cout << "ep1: " << ep1.x << " " << ep1.y << " " << ep1.z << endl;
			//cout << "ep2: " << ep2.x << " " << ep2.y << " " << ep2.z << endl;
			//getchar();
		}
	}
	if (level == 1) {

		cout << "Level 1 completed" << endl;
		return outmesh;
	}
	cout << "Level " << level << " completed" << endl;
	return createLoop(outmesh, level - 1);
}
