#include "turtle.h"

Turtle::Turtle(point _position, vect _direction, vect _zDirection, long double _stepSize) :
	position(_position), direction(_direction), zDirection(_zDirection), stepSize(_stepSize), penColor(0, 0, 0), isPenDown(true) {
	direction.normalize();
}
