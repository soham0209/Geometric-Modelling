#pragma once
#include "mesh.h"
#include "geometry.h"
#include "Utils.h"
#include <map>
class CatmullClark {
public:
	CatmullClark();
	Mesh createCatmull(Mesh m, int level);
	
private:
	map<int, Vec3f> face_map;
	map<int, Vec3f>  edge_map;
	Utils util;
};