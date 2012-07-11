#pragma once
#include <complex>
#include "geometry.h"

using namespace std;

typedef complex<long double> comp;

//So far it works only for the Mobius transformation.
struct transformation {
	comp a, b, c, d; // f(z) = (az + b) / (cz + d)
	transformation();
	pair<point, vect> setCoords(pair<point, vect> coords); 
	point setPos(point pos);
	void setValues(comp _a, comp _b, comp _c, comp _d); 
};


