//Original author:Refer https://github.com/Sayan-m90/Geometric-Modelling.
//Modified by Soham Mukherjee

#include "BsplineSurface.h"
#include <string>

float BsplineSurface::evalspline(float u, float a[4][4], float v)
{
	float b[4], c[4];

	b[0] = pow(1 - u, 3) / 6.0;
	b[1] = (3 * pow(u, 3) - 6 * u*u + 4) / 6.0;
	b[2] = (-3 * u*u*u + 3 * u*u + 3 * u + 1) / 6.0;
	b[3] = u * u*u / 6.0;


	c[0] = pow(1 - v, 3) / 6.0;
	c[1] = (3 * pow(v, 3) - 6 * v*v + 4) / 6.0;
	c[2] = (-3 * v*v*v + 3 * v*v + 3 * v + 1) / 6.0;
	c[3] = v * v*v / 6.0;

	float x = 0.0;

	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			x += a[i][j] * b[i] * c[j];
		}
	}

	return x;
}

Mesh BsplineSurface::createBsplineSurf(string filename,char type)
{
	Mesh outmesh;

	ifstream file(filename);
    string str;

    file >> str;  int n = stoi(str);
    file >> str;  int m = stoi(str);
    cout << " u: " << m << " v: " << n << endl;

    float **ptx = new float*[m];
    float **pty = new float*[m];
    float **ptz = new float*[m];


	for (int i = 0; i<m; i++)
	{
        ptx[i] = new float[n];
        pty[i] = new float[n];
        ptz[i] = new float[n];
	}
    for (int i = 0; i<n; i++)
	{
        for (int j = 0; j<m; j++)
		{
            file >> str;              ptx[j][i] = stof(str);
            file >> str;              pty[j][i] = stof(str);
            file >> str;              ptz[j][i] = stof(str);

		}
	}

    float Px[4][4], Py[4][4], Pz[4][4];
    float ustep = 0.1f, wstep = 0.1f;

    if(type=='o'){
	for (int i = 0; i<m - 3; i++)
	{
        for (int j = 0; j<n - 3; j++)
		{

			for (int k = 0; k<4; k++)
			{

				for (int q = 0; q<4; q++)
				{

                    Px[k][q] = ptx[i + k][j + q];
                    Py[k][q] = pty[i + k][j + q];
                    Pz[k][q] = ptz[i + k][j + q];
				}

            }
            for (float u = 0; u < 1; u += ustep)
            {
				vector<GeomVert> tmp;
                for (float v = 0; v < 1; v += wstep)
				{

                    GeomVert vprime(evalspline(u, Px, v), evalspline(u, Py, v), evalspline(u, Pz, v));
                    GeomVert vprime2(evalspline(u, Px, v+wstep), evalspline(u, Py, v+wstep), evalspline(u, Pz, v+wstep));
                    GeomVert vprime3(evalspline(u+ustep, Px, v+wstep), evalspline(u+ustep, Py, v+wstep), evalspline(u+ustep, Pz, v+wstep));
                    GeomVert vprime4(evalspline(u+ustep, Px, v), evalspline(u+ustep, Py, v), evalspline(u+ustep, Pz, v));
                       tmp.push_back(vprime);
                       tmp.push_back(vprime2);
                       tmp.push_back(vprime3);
                       tmp.push_back(vprime4);
                       outmesh.AddFacet(tmp);
                       tmp.clear();

                }


			}
		}

	}
}

    else if(type=='c'){

        for (int i = 0; i<m-3; i++)
        {
            for (int j = 0; j<n; j++)
            {
                for (int k = 0; k<4; k++)
                {
                    vector<GeomVert> tmp;
                    for (int q = 0; q<4; q++)
                    {

                        Px[k][q] = ptx[i + k][(j + q)%n];
                        Py[k][q] = pty[i + k][(j + q)%n];
                        Pz[k][q] = ptz[i + k][(j + q)%n];
                    }


                }
                for (float u = 0; u <= 1; u += ustep)
                {
                    vector<GeomVert> tmp;
                    for (float v = 0; v <= 1; v += wstep)
                    {

                    GeomVert vprime(evalspline(u, Px, v), evalspline(u, Py, v), evalspline(u, Pz, v));
                    GeomVert vprime2(evalspline(u, Px, v+wstep), evalspline(u, Py, v+wstep), evalspline(u, Pz, v+wstep));
                    GeomVert vprime3(evalspline(u+ustep, Px, v+wstep), evalspline(u+ustep, Py, v+wstep), evalspline(u+ustep, Pz, v+wstep));
                    GeomVert vprime4(evalspline(u+ustep, Px, v), evalspline(u+ustep, Py, v), evalspline(u+ustep, Pz, v));
                       tmp.push_back(vprime);
                       tmp.push_back(vprime2);
                       tmp.push_back(vprime3);
                       tmp.push_back(vprime4);
                       outmesh.AddFacet(tmp);
                       tmp.clear();

                    }

                }
            }

        }
    }


	return outmesh;
}
