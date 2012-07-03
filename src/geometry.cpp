#include "geometry.h"
#include <algorithm>
#include <cmath>
/* Open Frameworks supplies its own geometry primitives. Maybe they should replace "geometry.h"
*/

point::point(long double _x, long double _y, long double _z) : x(_x), y(_y), z(_z) {}

bool point::operator==(const point& p) {
	return abs(x - p.x) < EPS && abs(y - p.y) < EPS && abs(z - p.z) < EPS;
}


vect::vect(point p) : point(p) {}

long double vect::length() {
	return sqrt(x * x + y * y + z * z);
}

void vect::normalize() {
	if(*this == vec0) return;
	long double myLength = length(); // this != vec0 ==> myLength != 0
	x /= myLength;
	y /= myLength;
	z /= myLength;
}

