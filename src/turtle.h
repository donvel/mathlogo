#pragma once

#include "geometry.h"

struct Turtle {
	point position;
	vect direction; 
	long double stepSize;
	Turtle(point _position = point(), vect _direction = vect(), long double _stepSize = 1.0);
};
