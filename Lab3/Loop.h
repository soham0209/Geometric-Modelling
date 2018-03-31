#pragma once
#include "mesh.h"
#include "Utils.h"
#include "geometry.h"
#include <map>
class Loop {
public:
	Mesh createLoop(Mesh m, int level);
private:
	map<int, Vec3f> edge_map;
	map<int, Vec3f> vert_map;
	Utils util;
};