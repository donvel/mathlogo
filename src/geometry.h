#pragma once

const static long double EPS = 1.0e-3;

//long double abs(long double, long double);

struct point {
	long double x, y, z; // decent precision;
	point(long double _x = 0.0, long double _y = 0.0, long double _z = 0.0);
	bool operator==(const point&);
};

struct vect : point {
	vect(long double _x = 0.0, long double _y = 1.0, long double _z = 0.0);
	
};
