#include "turtle.h"

Turtle::Turtle(point _position, vect _direction, vect _zDirection) :
	position(_position), direction(_direction), zDirection(_zDirection), penColor(0, 0, 0), isPenDown(true), 
		pos(ofVec2f(0, 0)), dir(ofVec2f(1, 0)), faceId(0)
{
	direction.normalize();
}
