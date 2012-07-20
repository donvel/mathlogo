#pragma once

#include <algorithm>
#include <cmath>
#include <cassert>

#include "ofMain.h"
/* Open Frameworks supplies its own geometry primitives. Maybe they should replace "geometry.h"
*/

const static double EPS = 1.0e-3;

struct vect; // Previous declaration is needed, since vect appears in the definition of point
struct gridPoint;

struct point {
	double x, y, z; // decent precision;
	point(double _x = 0.0, double _y = 0.0, double _z = 0.0);
	point(const gridPoint&);
	point(const ofVec2f);
	bool operator==(const point&);
	void operator+=(const vect&);	
	point translated(const vect&);	
};

struct vect : point { // vector == std::vector, so it is better to use another name
	vect(point p = point(0.0, 1.0, 0.0));
	vect(double x, double y, double z = 0.0);
	double length();
	void normalize();
	void operator*=(const double&);
	vect operator*(const double&) const;	
	vect operator+(const vect&) const;	
	vect operator- () const;
	vect rotated(double angle); // in degrees
};

const static point pointInInfinity(1000000.0, 1000000.0); // Maybe not in infinity, but at least outside the viewport
const static vect vec0 = vect(point(0.0, 0.0, 0.0));
		
struct voxel {
	voxel(ofColor _color = ofColor(255, 255, 255));
	bool visited;
	ofColor color;
	void visit(bool changeColor = false, ofColor newColor = ofColor());
};

struct segment {
	point a, b;
	ofColor color;
	segment();
	segment(point _a, point _b, ofColor _color = ofColor(0));
};

struct gridPoint {
	int x, y, z;
	gridPoint();
	gridPoint(const point&);
	gridPoint(int _x, int _y, int _z = 0);
};

double dist(point, point);
double angle(vect, vect); // in radians
bool intersect(segment a, segment b, point &p);
