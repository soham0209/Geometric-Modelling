#pragma once
#include <vector>
#include "mesh.h"
#include "geometry.h"
using namespace std;

class BezierSurface {
public:
	BezierSurface();
	float B(int i, int n, float u);
	GeomVert evalBezier(vector<vector<GeomVert>>Pij, float u, float w);
	void BezierMesh(Mesh& outMesh, int stepu, int stepw, vector<vector<GeomVert>> Pij);
	float dist(float x1, float y1, float z1, float x2, float y2, float z2);
    Mesh createBezierPatch(string filename, int us, int ws);

private:
	int counter;
	vector<vector<int>>pascal;
    int cp,ncp;
};
