#pragma once
#include "geometry.h"
#include "mesh.h"
#include <vector>
using std::vector;
using std::cout;
using std::endl;
class comparator {
public:
	Vec3f c,norm;
	comparator(Vec3f c,Vec3f norm) {
		this->c = c;
		this->norm = norm;
	}
	bool operator() (const Vec3f& a, const Vec3f& b ) {
		Vec3f cross = (a - c).crossProduct(b - c);
		float d = norm.dotProduct(cross);
		if (d > 0)  //Positive for CCW direction
			return true;
		else
			return false;
	}
	
};
class Utils {
public:
	vector<GeomVert> OrderPoints(vector<GeomVert> points);
	
};