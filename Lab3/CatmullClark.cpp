#include "CatmullClark.h"

CatmullClark::CatmullClark()
{
}

Mesh CatmullClark::createCatmull(Mesh m, int level)
{
		Mesh outmesh;
		int count = 0;
		int num_facets = m.GetNumberFacets();
		for (int facetind = 0; facetind < num_facets; facetind++) {
			TopoFacet f = m.GetFacet(facetind);
			int vert_num = f.GetNumberVertices();
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
			Vec3f face_point(fpx, fpy, fpz);
			face_map[facetind] = face_point;
			
		}//For each face compute face_point

		
		int edge_num = m.GetNumberEdges();
		for (int eindex = 0; eindex < edge_num; eindex++) {
			TopoEdge e = m.GetEdge(eindex);
			int  v1 = e.GetVertex(0);
			int v2 = e.GetVertex(1);
			Vec3f m1(m.GetGeomVertex(v1).GetCo(0), m.GetGeomVertex(v1).GetCo(1), m.GetGeomVertex(v1).GetCo(2));
			Vec3f m2(m.GetGeomVertex(v2).GetCo(0), m.GetGeomVertex(v2).GetCo(1), m.GetGeomVertex(v2).GetCo(2));
			int adj_face = e.GetNumberIncFacets();
			Vec3f F(0, 0, 0);
            if (adj_face == 1)
                F = face_map[e.GetIncFacet(0)];
            else
            {
				F = face_map[e.GetIncFacet(0)] + face_map[e.GetIncFacet(1)];
			}
			Vec3f E = (m1 + m2 + F) / 4.0;
			edge_map[eindex] = E;
		}
		int vert_num = m.GetNumberVertices();
		vector<GeomVert> meshpts;
		for (int vert_id = 0; vert_id < vert_num; vert_id++) {
			TopoVert v = m.GetVertex(vert_id);
			int inc_faces = v.GetNumberIncFacets();
			Vec3f Q(0, 0, 0);
			Vec3f R(0, 0, 0);
			Vec3f E[2]; 

			for (int inc = 0; inc < inc_faces; inc++) {
				int facet_id = v.GetIncFacet(inc);
					Q += face_map[facet_id];
			}
			Q /= (float)inc_faces;
			int inc_edg = v.GetNumberIncEdges();
			for (int eid = 0; eid < inc_edg; eid++) {
				int edg = v.GetIncEdge(eid);
				TopoEdge e = m.GetEdge(edg);
				GeomVert v0(m.GetGeomVertex(e.GetVertex(0)));
				GeomVert v1(m.GetGeomVertex(e.GetVertex(1)));
				Vec3f edgpt((v0.GetCo(0) + v1.GetCo(0)) / 2.0, (v0.GetCo(1) + v1.GetCo(1)) / 2.0, (v0.GetCo(2) + v1.GetCo(2)) / 2.0);
				R += edgpt;
			}
			R /= (float)inc_edg;

			GeomVert pij = m.GetGeomVertex(vert_id);
			Vec3f P(pij.GetCo(0), pij.GetCo(1), pij.GetCo(2));
            float alpha,beta;
            /*if(inc_edge==3){
             * alpha=1.0/2.0;
             * beta=1.0/12.0;
             * }
             * else if(inc_edge==4){
             * alpha=3.0/8.0;
             * beta=1.0/16.0;
             * }
             * else if(inc_edge==5){
             * alpha=3.0/20.0;
             * beta=1/20.0;
             * }
             * else if(inc_edge==6){
             * alpha=1.0/4.0;
             * beta=1.0/24.0;
             * }
              */
            Vec3f V = (Q + 2.0*R + (inc_faces - 3)*P) / ((float) inc_faces);

			
			for (int inc = 0; inc < inc_faces; inc++) {
				int match = 0;
				int facet_id = v.GetIncFacet(inc);
				TopoFacet f = m.GetFacet(facet_id);
				int edge_num = f.GetNumberEdges();
				for (int e = 0; e < edge_num; e++) {
					int eid = f.GetIncEdge(e);
					TopoEdge edg = m.GetEdge(eid);
					if (edg.GetVertex(0) == vert_id || edg.GetVertex(1) == vert_id) {
						E[match] = edge_map[eid];
						match++;
					}
					if (match == 2)
						break;
				}
				

				//Vec3f V(P.x, P.y, P.z);
				Vec3f fp = face_map[facet_id];
				meshpts.push_back(GeomVert(fp.x, fp.y, fp.z));
				meshpts.push_back(GeomVert(E[0].x, E[0].y, E[0].z));
				meshpts.push_back(GeomVert(E[1].x, E[1].y, E[1].z));
                //meshpts.push_back(GeomVert(V.x, V.y, V.z));
                outmesh.AddFacet(util.OrderPoints(meshpts));
                meshpts.clear();
                meshpts.push_back(GeomVert(E[0].x, E[0].y, E[0].z));
                meshpts.push_back(GeomVert(E[1].x, E[1].y, E[1].z));
                meshpts.push_back(GeomVert(V.x, V.y, V.z));
                outmesh.AddFacet(util.OrderPoints(meshpts));
                meshpts.clear();
				cout << "Face: " << inc << "Vert: "<<vert_id<<endl;
				cout << "fp " << fp.x << " " << fp.y << " " << fp.z << endl;
				cout << "ep1 " << E[0].x << " " << E[0].y << " " << E[0].z << endl;
				cout << "ep2 " << E[1].x << " " << E[1].y << " " << E[1].z << endl;
				cout << "V " << V.x << " " << V.y << " " << V.z << endl;
				cout << endl;
				//outmesh.AddFacet(util.OrderPoints(meshpts));
				//meshpts.clear();
				count++;

			}
			//break;
			
		}

		if(level==1)
		return outmesh;
		return createCatmull(outmesh, level - 1);
}
