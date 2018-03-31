#pragma once
#include "mesh.h"
#include <vector>
#include <fstream>
#include <iostream>
class BsplineSurface {
public:
    float evalspline(float u, float a[4][4], float v);
    Mesh createBsplineSurf(string filename, char type);
};
