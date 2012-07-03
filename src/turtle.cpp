#include "turtle.h"

Turtle::Turtle(point _position, vect _direction, long double _stepSize) :
	position(_position), direction(_direction), stepSize(_stepSize) {
	direction.normalize();
}
