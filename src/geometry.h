#pragma once

#include <algorithm>
#include <cmath>

#include "ofMain.h"
/* Open Frameworks supplies its own geometry primitives. Maybe they should replace "geometry.h"
*/

const static long double EPS = 1.0e-3;

struct vect; // Previous declaration is needed, since vect appears in the definition of point

struct point {
	long double x, y, z; // decent precision;
	point(long double _x = 0.0, long double _y = 0.0, long double _z = 0.0);
	bool operator==(const point&);
	void operator+=(const vect&);	
//	virtual point operator+(const vect&) const;
	point translated(const vect&);	
};

struct vect : point { // vector == std::vector, so it is better to use another name
	vect(point p = point(0.0, 1.0, 0.0));
	long double length();
	void normalize();
	void operator*=(const long double&);
	vect operator*(const long double&) const;	
	vect operator+(const vect&) const;	
	vect operator- () const;
	vect rotated(long double angle); // in degrees
};

const static vect vec0 = vect(point(0.0, 0.0, 0.0));
		
struct voxel {
	voxel(ofColor _color = ofColor(255, 255, 255));
	bool visited;
	ofColor color;
	void visit(bool changeColor = false, ofColor newColor = ofColor());
};

struct gridPoint {
	int x, y, z;
	gridPoint(const point&);
	gridPoint(int _x, int _y, int _z = 0);
};

long double dist(point, point);
