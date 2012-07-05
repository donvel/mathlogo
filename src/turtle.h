#pragma once

#include "ofMain.h"
#include "geometry.h"

struct Turtle {
	point position;
	vect direction; // turtle's "nose"
	vect zDirection; // points up	
	long double stepSize;
	ofColor penColor;
	bool isPenDown;
	Turtle(point _position = point(), vect _direction = vect(), vect _zDirection = vect(point(0.0, 0.0, 1.0)), long double _stepSize = 1.0);
};
