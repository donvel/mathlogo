#pragma once
#include "geometry.h"

class Turtle {
	point position;
	point direction; // this is the point, to which Turtle should move after FD 1
   // maybe direction should be represented in a different way?	
	long double stepSize;
	Turtle(point _position = point(), vect _direction = vect(), long double _stepSize = 1.0);
};
