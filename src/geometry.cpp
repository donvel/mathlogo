#include "geometry.h"
/* Open Frameworks supplies its own geometry primitives. Maybe they should replace "geometry.h"
*/

//----------------POINT-----------------------------------------//

point::point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
point::point(const gridPoint& gp) : x(gp.x), y(gp.y), z(gp.z) {}
point::point(const ofVec2f ov) : x(ov.x), y(ov.y) {}

bool point::operator==(const point& p) {
	return abs(x - p.x) < EPS && abs(y - p.y) < EPS && abs(z - p.z) < EPS; // two points are considered equal
	// when they are very close to each other
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

vect::vect(double x, double y, double z) : point(x, y, z) {}

void vect::operator*= (const double& l) {
	x *= l;
	y *= l;
	z *= l;
}

vect vect::operator* (const double& l) const {
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

double vect::length() {
	return sqrt(x * x + y * y + z * z);
}

void vect::normalize() {// makes this->length 1
	if(*this == vec0) return;
	double myLength = length(); // this != vec0 ==> myLength != 0
	x /= myLength;
	y /= myLength;
	z /= myLength;
}

vect vect::rotated(double angle) {
	angle = angle * 2.0 * M_PI / 360.0; // degrees --> radians
	vect res;
	res.x = x * cos(angle) - y * sin(angle);
	res.y = x * sin(angle) + y * cos(angle);
	/*
	 * x' = x cos phi - y sin phi
	 * y' = x sin phi + y cos phi
	 * It works only when the turtle is on a plane, i. e. zDirection = [0, 0, 1]
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
segment::segment() {}

//----------------GRIDPOINT-----------------------------------------//

gridPoint::gridPoint() {}
gridPoint::gridPoint(const point &p) : x(round(p.x)), y(round(p.y)), z(round(p.z)) {}
gridPoint::gridPoint(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

//----------------FUNCTIONS-----------------------------------------//

double dist(point p1, point p2) {
	vect v1 = p1, v2 = p2, v3;
	v3 = v1 + (-v2);
	return v3.length();
}

double angle(vect v1, vect v2) { // in radians
	double cp = v1.x * v2.y - v1.y * v2.x;
	double res = asin(cp / (v1.length() * v2.length()));
	if (v1.x * v2.x + v1.y * v2.y < 0) {
		res = M_PI - res;
	}
	cout << "angle " << v1.x << ", " << v1.y << " and " << v2.x << ", "
			<< v2.y << " =  "<< res;
	return res;
}

double crossProductThreePoints(point a, point b, point c) {
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool intersect(segment s1, segment s2, point &p) {// function returns true if s1 and s2 intersect
	//and sets p as the intersection point
//	cout << "Cheing intersection " << s1.a.x << " " << s1.a.y << ", " << s1.b.x << " " << s1.b.y << 
//			" and " << s2.a.x << " " << s2.a.y << ", " << s2.b.x << " " << s2.b.y <<  endl;
	double c1 = crossProductThreePoints(s1.a, s1.b, s2.a);
	double c2 = crossProductThreePoints(s1.a, s1.b, s2.b);
	if(c1 * c2 >= 0) return false;
	double c3 = crossProductThreePoints(s2.a, s2.b, s1.a);
	double c4 = crossProductThreePoints(s2.a, s2.b, s1.b);
	if(c3 * c4 >= 0) return false;
	// Now we know that s1 and s2 intersect
	
	//Code adapted from "Algorytmika praktyczna" by Piotr Stanczyk
	point p1 = s1.a, p2 = s1.b, l1 = s2.a, l2 = s2.b;
	//(l2 -> p2) x (l1 -> l2)
	double s = (l2.x - p2.x) * (l1.y - l2.y) - (l2.y - p2.y) * (l1.x - l2.x);
	//(p1 -> p2) x (l1 -> l2)
	double t = (p1.x - p2.x) * (l1.y - l2.y) - (p1.y - p2.y) * (l1.x - l2.x);
	
	assert(t != 0);
	if(t == 0) {// double assertion, but I am not sure which one I should use
		cout << "t == 0, and this shouldn't have happened..." << endl;
		return false;
	}
	double w = s / t;
	
	assert(w > 0 && w < 1);
	if(w <= 0 || w >= 1) {
		cout << "w not in (0, 1)" << endl;
		return false;
	}
	
	p.x = p1.x * w + p2.x * (1 - w);
	p.y = p1.y * w + p2.y * (1 - w);
	
//	cout << "Intersection point: " << p.x << " " << p.y << endl;
	
	return true;
}

void line (point p1, point p2, ofColor targetColor, ofImage &img, bool thick) {
	double d = dist(p1, p2);
	for(double alpha = 0; alpha <= d + EPS; alpha += 0.5) {
		gridPoint gp = point(p1.x * (alpha / d) + p2.x * (1 - alpha / d),
				p1.y * (alpha / d) + p2.y * (1 - alpha / d));
		
		if(gp.x >= 0 && gp.x < img.getWidth() && gp.y >= 0 && gp.y < img.getHeight()) {
			img.setColor(gp.x, gp.y, targetColor);
		}
		if(thick) {
			for(int i = 0; i < 4; i++) {
				gridPoint np = neighbour(gp, i);
				if(np.x >= 0 && np.x < img.getWidth() && np.y >= 0 && np.y < img.getHeight()) {
					img.setColor(np.x, np.y, targetColor);
				}
			}
		}
	}
}

gridPoint neighbour(gridPoint p, int i) {
	switch(i) {
		case 0:
			return gridPoint(p.x + 1, p.y);
		break;
		case 1:
			return gridPoint(p.x, p.y + 1);
		break;
		case 2:
			return gridPoint(p.x - 1, p.y);
		break;
		case 3:
			return gridPoint(p.x, p.y - 1);
		break;
		
	}
	return p;
}