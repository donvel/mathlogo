#pragma once
#include "geometry.h"

class turtle {
	point position;
	point direction; // this is the point, to which turtle should move after FD 1
   // maybe direction should be represented in a different way?	
	long double stepSize;
	turtle(point _position = point(), vect _direction = vect(), long double _stepSize = 1.0);
};
