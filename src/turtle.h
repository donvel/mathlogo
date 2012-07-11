#pragma once

#include "ofMain.h"
#include "geometry.h"

struct Turtle {
	point position;
	vect direction; // turtle's "nose"
	vect zDirection; // points up, since I don't have 3D yet, it is not useful
	ofColor penColor;
	bool isPenDown;
	Turtle(point _position = point(), vect _direction = vect(), vect _zDirection = vect(point(0.0, 0.0, 1.0)));
};
