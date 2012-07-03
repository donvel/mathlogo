#pragma once

const static long double EPS = 1.0e-3;


struct point {
	long double x, y, z; // decent precision;
	point(long double _x = 0.0, long double _y = 0.0, long double _z = 0.0);
	bool operator==(const point&);
};

struct vect : point {
	vect(point p = point(0.0, 1.0, 0.0));
	long double length();
	void normalize();	
};

const static vect vec0 = vect(point(0.0, 0.0, 0.0));
