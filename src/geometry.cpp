#include "geometry.h"
/* Open Frameworks supplies its own geometry primitives. Maybe they should replace "geometry.h"
*/

//----------------POINT-----------------------------------------//

point::point(long double _x, long double _y, long double _z) : x(_x), y(_y), z(_z) {}
point::point(const gridPoint& gp) : x(gp.x), y(gp.y), z(gp.z) {}

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

vect vect::operator- () const {
	vect res;
	res.x = -x;
	res.y = -y;
	res.z = -z;
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

vect vect::rotated(long double angle) {
	angle = angle * 2.0 * M_PI / 360.0; // degrees --> radians
	vect res;
	res.x = x * cos(angle) - y * sin(angle);
	res.y = x * sin(angle) + y * cos(angle);
	/*
	 * x' = x cos phi - y sin phi
	 * y' = x sin phi + y cos phi
	 * It works only when the turtle is ona a plane, i. e. zDirection = [0, 0, 1]
	 */
	return res;
}

//----------------VOXEL-----------------------------------------//

voxel::voxel(ofColor _color) : visited(false), color(_color) {}

void voxel::visit(bool changeColor, ofColor newColor) {
	visited = true;
	if(changeColor) {
        color = newColor;
      }
}

//----------------SEGMENT----------------------------------------//

segment::segment(point _a, point _b, ofColor _color) : a(_a), b(_b), color(_color) {}

//----------------GRIDPOINT-----------------------------------------//

gridPoint::gridPoint() {}
gridPoint::gridPoint(const point &p) : x(p.x), y(p.y), z(p.z) {}
gridPoint::gridPoint(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

//----------------FUNCTIONS-----------------------------------------//

long double dist(point p1, point p2) {
	vect v1 = p1, v2 = p2, v3;
	v3 = v1 + (-v2);
	return v3.length();
}
