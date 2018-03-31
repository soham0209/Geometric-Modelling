#ifndef HANDLER_H
#define HANDLER_H
#include "mesh.h"
class Handler{
public:
    Handler();
    Mesh getfile(string filename);
    void outputOFF_mesh(Mesh m, string filename);
    void setSurf(int surf);
    void createSurface();
    float checkPlanar(Mesh m);
private:
    int surf;
    int ncp,cp;

};
#endif // HANDLER_H
