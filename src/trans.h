#pragma once
#include <complex>
#include <vector>

#include "geometry.h"

using namespace std;

typedef complex<double> comp;

//So far it works only for the Moebius transformation.

struct singleTransformation {
	virtual pair<point, vect> setCoords(pair<point, vect> coords) {return make_pair(point(), vect());}
	virtual point setPos(point pos) {return point();}
	virtual vector<comp> fixedPoints() {return vector<comp>();}
};

struct moebius : singleTransformation {
	comp a, b, c, d; // f(z) = (az + b) / (cz + d)
	moebius(comp _a = 1, comp _b = 0, comp _c = 0, comp _d = 1);
	pair<point, vect> setCoords(pair<point, vect> coords); 
	point setPos(point pos);
	void normalize();
	vector<comp> fixedPoints();
};

struct mirror : singleTransformation {
	double y;
	mirror(double _y = 0);
	pair<point, vect> setCoords(pair<point, vect> coords); 
	point setPos(point pos);
};

struct transformation {
	vector<singleTransformation*> trans;
	transformation();
	~transformation();
	pair<point, vect> setCoords(pair<point, vect> coords); 
	point setPos(point pos);
	vector<comp> addMoebius(comp a, comp b, comp c, comp d); 
	void addMirror(double y);
};

vector<vector<comp> > matrixMul(vector<vector<comp> > m1, vector<vector<comp> > m2);


