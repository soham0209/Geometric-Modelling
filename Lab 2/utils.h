#ifndef UTILS_H
#define UTILS_H

#include<iostream>
#include<fstream>
#include <QPointF>
#include <vector>
#include "mesh.h"

using namespace std;
class Mesh;
class Utils{

public:
    Utils();
    ~Utils();
    void writetofile(Mesh m);
    Mesh createData(vector<QPointF> mypoints, char c, int numslices, bool *flag);
    Mesh createEx(vector<QPointF> mypoints,int numslices,float z);
    Mesh createSweep(vector<QPointF> mypoints, vector<QPointF> trajpoints, int numslices);
    float B(int i, int n, float u);
    vector<QPointF> drawBezier(vector<QPointF> mypoints, char type, int max);
    vector<QPointF> Utils::BSubDivide(vector<QPointF> p, char type, int m);


private:
    struct vec3{
            float x;
            float y;
            float z;

            void round(vec3* p){
            p->x = roundf(p->x * 100) / 100;
            p->y = roundf(p->y * 100) / 100;
            p->z = roundf(p->z * 100) / 100;
            }
        };



};




#endif // UTILS_H
