#pragma once
#include <complex>
#include <vector>

#include "geometry.h"

using namespace std;

typedef complex<long double> comp;

//So far it works only for the Mobius transformation.
struct coefficients {
	comp a, b, c, d; // f(z) = (az + b) / (cz + d)
	coefficients(comp _a = 1, comp _b = 0, comp _c = 0, comp _d = 1);
	pair<point, vect> setCoords(pair<point, vect> coords); 
	point setPos(point pos);
};

struct transformation {
	vector<coefficients> coeff;
	vector<double> mirrorY;
	transformation();
	pair<point, vect> setCoords(pair<point, vect> coords); 
	point setPos(point pos);
	void addValues(comp a, comp b, comp c, comp d); 
};


