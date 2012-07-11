#include "trans.h"

transformation::transformation() : a(1), b(0), c(0), d(1) {}

void transformation::setValues(comp _a, comp _b, comp _c, comp _d) {
	a = _a, b = _b, c = _c, d = _d;
}

point transformation::setPos(point pos) {
	comp z(pos.x, pos.y);
	if(c * z + d == comp(0, 0)) {
		cout << "division by 0" << endl;
		return pos;
	}
	return point(z.real(), z.imag());
}

pair<point, vect> transformation::setCoords(pair<point, vect> coords) {
	
	comp z(coords.first.x, coords.first.y);
	if(c * z + d == comp(0, 0)) {
		cout << "division by 0" << endl;
		return coords;
	}
	z = (a * z + b) / (c * z + d);
	
	point npos(z.real(), z.imag());
	
	point nextStep = coords.first.translated(coords.second);
	
	comp w(nextStep.x, nextStep.y);
	if(c * w + d == comp(0, 0)) {
		cout << "division by 0" << endl;
		return coords;
	}
	
	w = (a * w + b) / (c * w + d);
	
	w -= z;
	
	vect ndir(w.real(), w.imag());
	ndir.normalize();
	
	return make_pair(npos, ndir);
}
