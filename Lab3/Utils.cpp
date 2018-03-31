#include "Utils.h"
#include "mesh.h"
#include <algorithm>
vector<GeomVert> Utils::OrderPoints(vector<GeomVert> points)
{
	size_t n = points.size();
	vector<Vec3f> pts;
	vector<GeomVert> ordered_pts;
	float cx = 0.0, cy = 0.0, cz = 0.0;
	for (size_t i = 0; i < n; i++) {
		cx += points[i].GetCo(0);
		cy += points[i].GetCo(1);
		cz += points[i].GetCo(2);
		pts.push_back(Vec3f(points[i].GetCo(0), points[i].GetCo(1), points[i].GetCo(2)));
	}
	
	cx /= n;
	cy /= n;
	cz /= n;
	Vec3f c(cx, cy, cz);
	Vec3f norm = (pts[0] - pts[1]).crossProduct(pts[0] - pts[2]);
	if (c.dotProduct(norm) < 0)
		norm = -norm;
	comparator comp(c, norm);
	std::sort(pts.begin() + 1, pts.end(), comp);
	for (size_t i = 0; i < n; i++) {
		ordered_pts.push_back(GeomVert(pts[i].x,pts[i].y,pts[i].z));
		//cout << pts[i].x << " " << pts[i].y << " " << pts[i].z << endl;
	}
	return ordered_pts;
}
