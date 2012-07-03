#pragma once
#include "geometry.h"

class Turtle {
	private:
		point position;
		vect direction; 
		long double stepSize;
	public:
		Turtle(point _position = point(), vect _direction = vect(), long double _stepSize = 1.0);
};
