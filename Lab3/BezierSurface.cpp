#include "BezierSurface.h"
#include "geometry.h"
#include<fstream>
#include <iostream>
#include <string>
using namespace std;

BezierSurface::BezierSurface()
{
	vector<int>elements;
    counter=0;
	for (int j = 0; j<31; j++) {
		for (int i = 0; i <= j; i++) {
			if (i == 0 || i == j)
				elements.push_back(1);
			else
				elements.push_back(pascal[j - 1][i - 1] + pascal[j - 1][i]);
		}
		pascal.push_back(elements);
		elements.clear();
	}

}

float BezierSurface::B(int i, int n, float u)
{
	float a;
	a = (float)(pascal[n][i] * pow((1 - u), n - i)*pow(u, i));
	return a;
}

GeomVert BezierSurface::evalBezier(vector<vector<GeomVert>> Pij, float u, float w)
{
	size_t m = Pij.size() - 1;
	size_t n = Pij[0].size() - 1;
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	for (size_t i = 0; i <= m; i++) {
		for (size_t j = 0; j <= n; j++) {
			x += (Pij[i][j].GetCo(0))*B(i, m, u)*B(j, n, w);
			y += (Pij[i][j].GetCo(1))*B(i, m, u)*B(j, n, w);
			z += (Pij[i][j].GetCo(2))*B(i, m, u)*B(j, n, w);
		}
	}
	return GeomVert(x, y, z);

}

void BezierSurface::BezierMesh(Mesh & outMesh, int stepu, int stepw, vector<vector<GeomVert>> Pij)
{
	float ustep = 1 / float(stepu);

	float wstep = 1 / float(stepw);

    float u=0.0;
    float w=0.0;

    for (int i = 0; i < stepu; i++) {

        for (int j = 0; j < stepw; j++) {

			vector<GeomVert> geomfacet;


			GeomVert v1 = evalBezier(Pij, u, w);
			GeomVert v2 = evalBezier(Pij, u, w + wstep);
			GeomVert v3 = evalBezier(Pij, u + ustep, w + wstep);
			GeomVert v4 = evalBezier(Pij, u + ustep, w);
			

			geomfacet.push_back(v1);

			geomfacet.push_back(v2);

			geomfacet.push_back(v3);

			geomfacet.push_back(v4);

			outMesh.AddFacet(geomfacet);

            w+=wstep;

		}
        u+=ustep;
        w=0.0;

        cout << "Patch done: " << counter<<endl;
        counter++;
	}

}

float BezierSurface::dist(float x1, float y1, float z1, float x2, float y2, float z2)
{
	return ((x1 - x2)*(x1 - x2)) + ((y1 - y2)*(y1 - y2)) + ((z1 - z2)*(z1 - z2));
}


Mesh BezierSurface::createBezierPatch(string filename, int us, int ws)
{
	
	Mesh outMesh;

	ifstream file(filename);
	if (file.is_open())
		cout << filename << " opened" << endl;
	string str;

	file >> str;
	int u = stoi(str);
	file >> str;
	int v = stoi(str);
	//	int n = 16;
	cout << " u: " << u << " v: " << v << endl;


	vector<vector<GeomVert>> coord;

    for (int j = 0; j < u; j++) {
		cout << "Patch no: " << j << endl;
		vector<GeomVert> xyz;
        for (int i = 0; i < v; i++) {
			file >> str;
			float x = stof(str);


			file >> str;
			float y = stof(str);


			file >> str;
			float z = stof(str);

			GeomVert p(x, y, z);
			xyz.push_back(p);

		}
		coord.push_back(xyz);
	}
    //Enforcing G1 Continuity
    size_t row=coord.size()-1;
    size_t col=coord[0].size()-1;
    if(coord[row][0]==coord[0][0]){
       for(size_t j=0;j<=col;j++){
           float cx=(coord[row-1][j].GetCo(0)+coord[1][j].GetCo(0))/2.0;
           float cy=(coord[row-1][j].GetCo(1)+coord[1][j].GetCo(1))/2.0;
           float cz=(coord[row-1][j].GetCo(2)+coord[1][j].GetCo(2))/2.0;
            coord[0][j]=GeomVert(cx,cy,cz);
            coord[row][j]=GeomVert(cx,cy,cz);

       }
    }
	BezierMesh(outMesh, us, ws, coord);
	
	return outMesh;

}

