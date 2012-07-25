#include "geometry.h"
/* Open Frameworks supplies its own geometry primitives. Maybe they should replace "geometry.h"
*/

//----------------POINT-----------------------------------------//

point::point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
point::point(const gridPoint& gp) : x(gp.x), y(gp.y), z(gp.z) {}
point::point(const ofVec2f ov) : x(ov.x), y(ov.y) {}

bool point::operator==(const point& p) {
	return abs(x - p.x) < EPS && abs(y - p.y) < EPS && abs(z - p.z) < EPS; // two points are considered equal
	// when they are very close to each other
}

void point::operator+= (const vect& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

point point::translated(const vect &v) {
	point res = *this;
	res += v;
	return res;
}


//----------------VECT-----------------------------------------//

vect::vect(point p) : point(p) {}

vect::vect(double x, double y, double z) : point(x, y, z) {}

void vect::operator*= (const double& l) {
	x *= l;
	y *= l;
	z *= l;
}

vect vect::operator* (const double& l) const {
	vect res = *this;
	res *= l;
	return res;
}

vect vect::operator+ (const vect& v) const {
	vect res = *this;
	res += v;
	return res;
}

vect vect::operator- () const {
	vect res;
	res.x = -x;
	res.y = -y;
	res.z = -z;
	return res;
}

double vect::length() {
	return sqrt(x * x + y * y + z * z);
}

void vect::normalize() {// makes this->length 1
	if(*this == vec0) return;
	double myLength = length(); // this != vec0 ==> myLength != 0
	x /= myLength;
	y /= myLength;
	z /= myLength;
}

vect vect::rotated(double angle) {
	angle = angle * 2.0 * M_PI / 360.0; // degrees --> radians
	vect res;
	res.x = x * cos(angle) - y * sin(angle);
	res.y = x * sin(angle) + y * cos(angle);
	/*
	 * x' = x cos phi - y sin phi
	 * y' = x sin phi + y cos phi
	 * It works only when the turtle is on a plane, i. e. zDirection = [0, 0, 1]
	 */
	return res;
}

//----------------VOXEL-----------------------------------------//

voxel::voxel(ofColor _color) : visited(false), color(_color) {}

void voxel::visit(bool changeColor, ofColor newColor) {
	visited = true;
	if(changeColor) {
        color = newColor;
      }
}

//----------------SEGMENT----------------------------------------//

segment::segment(point _a, point _b, ofColor _color) : a(_a), b(_b), color(_color) {}
segment::segment() {}

//----------------GRIDPOINT-----------------------------------------//

gridPoint::gridPoint() {}
gridPoint::gridPoint(const point &p) : x(round(p.x)), y(round(p.y)), z(round(p.z)) {}
gridPoint::gridPoint(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

//----------------FUNCTIONS-----------------------------------------//

double dist(point p1, point p2) {
	vect v1 = p1, v2 = p2, v3;
	v3 = v1 + (-v2);
	return v3.length();
}

double angle(vect v1, vect v2) { // in radians
	double cp = v1.x * v2.y - v1.y * v2.x;
	double res = asin(cp / (v1.length() * v2.length()));
	if (v1.x * v2.x + v1.y * v2.y < 0) {
		res = M_PI - res;
	}
	cout << "angle " << v1.x << ", " << v1.y << " and " << v2.x << ", "
			<< v2.y << " =  "<< res;
	return res;
}

double crossProductThreePoints(point a, point b, point c) {
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool intersect(segment s1, segment s2, point &p) {// function returns true if s1 and s2 intersect
	//and sets p as the intersection point
//	cout << "Cheing intersection " << s1.a.x << " " << s1.a.y << ", " << s1.b.x << " " << s1.b.y << 
//			" and " << s2.a.x << " " << s2.a.y << ", " << s2.b.x << " " << s2.b.y <<  endl;
	double c1 = crossProductThreePoints(s1.a, s1.b, s2.a);
	double c2 = crossProductThreePoints(s1.a, s1.b, s2.b);
	if(c1 * c2 >= 0) return false;
	double c3 = crossProductThreePoints(s2.a, s2.b, s1.a);
	double c4 = crossProductThreePoints(s2.a, s2.b, s1.b);
	if(c3 * c4 >= 0) return false;
	// Now we know that s1 and s2 intersect
	
	//Code adapted from "Algorytmika praktyczna" by Piotr Stanczyk
	point p1 = s1.a, p2 = s1.b, l1 = s2.a, l2 = s2.b;
	//(l2 -> p2) x (l1 -> l2)
	double s = (l2.x - p2.x) * (l1.y - l2.y) - (l2.y - p2.y) * (l1.x - l2.x);
	//(p1 -> p2) x (l1 -> l2)
	double t = (p1.x - p2.x) * (l1.y - l2.y) - (p1.y - p2.y) * (l1.x - l2.x);
	
	assert(t != 0);
	if(t == 0) {// double assertion, but I am not sure which one I should use
		cout << "t == 0, and this shouldn't have happened..." << endl;
		return false;
	}
	double w = s / t;
	
	assert(w > 0 && w < 1);
	if(w <= 0 || w >= 1) {
		cout << "w not in (0, 1)" << endl;
		return false;
	}
	
	p.x = p1.x * w + p2.x * (1 - w);
	p.y = p1.y * w + p2.y * (1 - w);
	
//	cout << "Intersection point: " << p.x << " " << p.y << endl;
	
	return true;
}

void line (point p1, point p2, ofColor targetColor, ofImage &img, bool thick) {
	double d = dist(p1, p2);
	for(double alpha = 0; alpha <= d + EPS; alpha += 0.5) {
		gridPoint gp = point(p1.x * (alpha / d) + p2.x * (1 - alpha / d),
				p1.y * (alpha / d) + p2.y * (1 - alpha / d));
		
		if(gp.x >= 0 && gp.x < img.getWidth() && gp.y >= 0 && gp.y < img.getHeight()) {
			img.setColor(gp.x, gp.y, targetColor);
		}
		if(thick) {
			for(int i = 0; i < 4; i++) {
				gridPoint np = neighbour(gp, i);
				if(np.x >= 0 && np.x < img.getWidth() && np.y >= 0 && np.y < img.getHeight()) {
					img.setColor(np.x, np.y, targetColor);
				}
			}
		}
	}
}

gridPoint neighbour(gridPoint p, int i) {
	switch(i) {
		case 0:
			return gridPoint(p.x + 1, p.y);
		break;
		case 1:
			return gridPoint(p.x, p.y + 1);
		break;
		case 2:
			return gridPoint(p.x - 1, p.y);
		break;
		case 3:
			return gridPoint(p.x, p.y - 1);
		break;
		
	}
	return p;
}

float angle(ofVec3f v1, ofVec3f v2) {
	float angle = v1.angle(v2);
	if(isnan(angle)) {
		if(v1.x * v2.x > 0) return 0;
		return 180;
	}
	return angle;
}

float crossProd(ofVec2f p1, ofVec2f p2) {
	return p1.x * p2.y - p1.y * p2.x;
}

float areaInCircle(ofVec2f p0, ofVec2f p1, ofVec2f p2, float r) {
	ofVec2f p[3];
	p[0] = p0, p[1] = p1, p[2] = p2;

	vector<pair<ofVec2f, bool> > points;
	vector<ofVec2f> inter;
	
	for(int i = 0; i < 3; i++) {
		if(p[i].length() <= r) p[i] *= 0.999;
		if(p[i].length() > r) p[i] *= 1.001;
	}
	
	for(int i = 0; i < 3; i++) {
		ofVec2f v1 = p[i];
		ofVec2f v2 = p[(i + 1) % 3];
		cout << (int)(v1.length() < r) << " " << (int)(v2.length() < r) << endl;
		if(v1.length() < r) {
			points.push_back(make_pair(p[i], true));
			if(v2.length() > r) {
				segmentCircleIntersection(v1, v2, r, inter);
				points.push_back(make_pair(inter[0], false));
			}
		} else {
			if(v2.length() < r) {
				segmentCircleIntersection(v1, v2, r, inter);
				points.push_back(make_pair(inter[0], true));
			} else {
				segmentCircleIntersection(v1, v2, r, inter);
				if(inter.size() == 2) {
					points.push_back(make_pair(inter[0], true));
					points.push_back(make_pair(inter[1], false));
				}
			}
		}
	}
	for(int i = 0; i < (int)points.size(); i++) {
		cout << points[i].first << " tri = " << (int)points[i].second << endl;
	}
	cout << endl;
	
	float res = 0.0;
	float circleArea = M_PI * r * r;
	if(points.size() == 0) {
		for(int i = 0; i < 3; i++) {
			if(crossProd(p[i], p[(i + 1) % 3]) * crossProd(p[(i + 1) % 3], p[(i + 2) % 3]) < 0) {
				return 0;
			}
		}
		return circleArea;
	}
	for(int i = 0; i < (int)points.size(); i++) {
	
		ofVec2f v1 = points[i].first;
		ofVec2f v2 = points[(i + 1) % points.size()].first;
		if(points[i].second) {
			res += 0.5 * crossProd(v1, v2);
		} else {
			res += circleArea * (angle(v1, v2) / 360.0) * (crossProd(v1, v2) > 0 ? 1 : -1);
		}
	}

	return abs(res);
}

void segmentCircleIntersection(ofVec2f v1, ofVec2f v2, float r, vector<ofVec2f> &inter) {
	inter.clear();
	ofVec2f n(1, 0);
	ofVec2f p1 = n * (v2 - v1).length();
	ofVec2f p2 = n.rotated(angle(v2 - v1, -v1)) * v1.length();
	cout << "p1 = " << p1 << " p2 = " << p2 << endl;
	float dist = abs(p2.y);
	if(dist >= r) return;
	float len = pow(r * r - dist * dist, 0.5);
	float i1 = p2.x - len, i2 = p2.x + len;
	if(i1 > 0 && i1 < p1.length()) {
		inter.push_back(v1 + (v2 - v1).normalized() * i1);
	} 
	if(i2 > 0 && i2 < p1.length()) {
		inter.push_back(v1 + (v2 - v1).normalized() * i2);
	} 
	
	cout << " i1 = " << i1 << " i2 = " << i2 << endl; 
}