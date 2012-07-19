#include "trans.h"

moebius::moebius(comp _a, comp _b, comp _c, comp _d) : a(_a), b(_b), c(_c), d(_d) {
	normalize();
} 

void moebius::normalize() {
	comp k = comp(1, 0) / pow(a * d - b * c, 0.5);
	a /= k, b /= k, c /= k, d /= k;
}

vector<comp> moebius::fixedPoints() {
	vector<comp> res;
	comp deltaSqrt = pow(pow(a + d, 2) - comp(4, 0), 0.5);
	res.push_back((a - d + deltaSqrt) / (comp(2, 0) * c));
	res.push_back((a - d - deltaSqrt) / (comp(2, 0) * c));
	return res;
}

mirror::mirror(double _y) : y(_y) {}

transformation::transformation() {} 
transformation::~transformation() {
	for(int i = 0; i < (int)trans.size(); i++) {
		delete trans[i];
	}
} 

vector<comp> transformation::addMoebius(comp a, comp b, comp c, comp d) {
	trans.push_back(new moebius(a, b, c, d));
	cout << "Added Moebius" << " " << this << endl; 
	cout << trans.size() << endl;
	return trans.back()->fixedPoints();
}

void transformation::addMirror(double y) {
	trans.push_back(new mirror(y));
}

point transformation::setPos(point pos) {
	for(int i = 0; i < (int)trans.size(); i++) {
		pos = trans[i]->setPos(pos);
	}
	return pos;
}

pair<point, vect> transformation::setCoords(pair<point, vect> coords) {

	for(int i = 0; i < (int)trans.size(); i++) {
		coords = trans[i]->setCoords(coords);
	}
	return coords;
}

point mirror::setPos(point pos) {
	return point(pos.x, y - (pos.y - y));
}

pair<point, vect> mirror::setCoords(pair<point, vect> coords) {
	return make_pair(setPos(coords.first), vect(coords.second.x, -coords.second.y));
}

point moebius::setPos(point pos) {

	comp z(pos.x, pos.y);
	if(c * z + d == comp(0, 0)) {
		cout << "division by 0" << endl; // this point is in infinity, hard to say what to do in that case
		return pointInInfinity;
	}
	z = (a * z + b) / (c * z + d);
	return point(z.real(), z.imag());
}

pair<point, vect> moebius::setCoords(pair<point, vect> coords) {
	point npos = setPos(coords.first);
	
	point nextStep = coords.first.translated(coords.second);
	
	vect ndir = vect(setPos(nextStep)) + (-vect(npos));
	
	ndir.normalize();// we have to normalize the direction vector
	
	return make_pair(npos, ndir);
}

vector<vector<comp> > matrixMul(vector<vector<comp> > m2, vector<vector<comp> > m1) {
	vector<vector<comp> > res;
	res.resize(2, vector<comp>(2));
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j  < 2; j++) {
			for(int k = 0; k < 2; k++) {
				res[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
	return res;
}