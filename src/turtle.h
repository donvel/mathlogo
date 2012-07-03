#pragma once
#include "geometry.h"

class turtle {
	point position;
	point direction; // this is the point, to which turtle should move after FD 1
   // maybe direction should be represented in a different way?	
	long double stepSize;
	turtle(point _position = point(), vect _direction = vect(1.0, 0.0, 0.0), long double _stepSize = 1.0);
};
