#include "trans.h"

coefficients::coefficients(comp _a, comp _b, comp _c, comp _d) : a(_a), b(_b), c(_c), d(_d) {}

transformation::transformation() : a(1), b(0), c(0), d(1) {} // identity

void transformation::setValues(comp _a, comp _b, comp _c, comp _d) {
	a = _a, b = _b, c = _c, d = _d;
}

point transformation::setPos(point pos) {
	comp z(pos.x, pos.y);
	if(c * z + d == comp(0, 0)) {
		cout << "division by 0" << endl; // this point is in infinity, hard to say what to do in that case
		return pointInInfinity;
	}
	z = (a * z + b) / (c * z + d);
	return point(z.real(), z.imag());
}

pair<point, vect> transformation::setCoords(pair<point, vect> coords) {
	
	//first let us set the new position
	
	comp z(coords.first.x, coords.first.y);
	if(c * z + d == comp(0, 0)) {
		cout << "division by 0" << endl;
		return make_pair(pointInInfinity, coords.second);
	}
	z = (a * z + b) / (c * z + d);
	
	point npos(z.real(), z.imag()); // this is the new position
	
	// Now let us determine the direction vector
	
	point nextStep = coords.first.translated(coords.second);
	
	comp w(nextStep.x, nextStep.y);
	if(c * w + d == comp(0, 0)) {
		cout << "division by 0" << endl;
		return make_pair(pointInInfinity, coords.second);
	}
	
	w = (a * w + b) / (c * w + d);
	
	w -= z;
	// Now w is the displacement vector
	
	vect ndir(point(w.real(), w.imag()));
	ndir.normalize();// we have to normalize the direction vector
	
	return make_pair(npos, ndir);
}
