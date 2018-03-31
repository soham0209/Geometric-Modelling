#pragma once
#include <vector>
#include "mesh.h"
#include "Utils.h"
#include <map>
class newpoint {
public:
	vector<GeomVert> vert;
	map<int, int>parent;
};
class DooSabin{

public:
	Mesh createDooSabin(Mesh m, int level);
private:
	map <int, newpoint> list;
	Utils util;

	
};