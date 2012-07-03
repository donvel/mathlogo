#include "geometry.h"
/* Open Frameworks supplies its own geometry primitives. Maybe they should replace "geometry.h"
*/

//----------------POINT-----------------------------------------//

point::point(long double _x, long double _y, long double _z) : x(_x), y(_y), z(_z) {}

bool point::operator==(const point& p) {
	return abs(x - p.x) < EPS && abs(y - p.y) < EPS && abs(z - p.z) < EPS;
}

void point::operator+= (const vect& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

point point::translated(const vect &v) {
	point res = *this;
	res += v;
	return res;
}


//----------------VECT-----------------------------------------//

vect::vect(point p) : point(p) {}


void vect::operator*= (const long double& l) {
	x *= l;
	y *= l;
	z *= l;
}

vect vect::operator* (const long double& l) const {
	vect res = *this;
	res *= l;
	return res;
}

vect vect::operator+ (const vect& v) const {
	vect res = *this;
	res += v;
	return res;
}

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

//----------------VOXEL-----------------------------------------//

voxel::voxel(ofColor _color) : visited(false), color(_color) {}

void voxel::visit(bool changeColor, ofColor newColor) {
	visited = true;
	color = newColor;
}

