#include "geometry.h"
#include <algorithm>

point::point(long double _x, long double _y, long double _z) : x(_x), y(_y), z(_z) {}

bool point::operator==(const point& p) {
	return abs(x - p.x) < EPS && abs(y - p.y) < EPS && abs(z - p.z) < EPS;
}


vect::vect(point p) : point(p) {}

void vect::normalize() {
	

}

