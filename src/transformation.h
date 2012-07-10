#pragma once
#include <complex>
#include "geometry.h"

using namespace std;

struct transformation {
	//complex<long double> a, b, c, d; // f(z) = (az + b) / (cz + d)
	pair<point, vect> setCoords(pair<point, vect> coords);
};
