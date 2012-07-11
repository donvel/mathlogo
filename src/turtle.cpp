#include "turtle.h"

Turtle::Turtle(point _position, vect _direction, vect _zDirection) :
	position(_position), direction(_direction), zDirection(_zDirection), penColor(0, 0, 0), isPenDown(true) {
	direction.normalize();
}
