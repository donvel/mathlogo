#include "geometry.h"
#include <algorithm>

point::point(long double _x, long double _y, long double _z) : x(_x), y(_y), z(_z) {}

bool point::operator==(const point& p) {
	return abs(x - p.x) < EPS && abs(y - p.y) < EPS && abs(z - p.z) < EPS;
}

vect::vect(long double _x, long double _y, long double _z) : x(_x), y(_y), z(_z) {}


//long double abs(long double a, long double b) {
	//long double c = a - b;
	//if (c > 0) return c;
	//return -c;
//}
