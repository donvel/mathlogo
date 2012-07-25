#include "data3d.h"
#include "world.h"
#include "interpreter.h"


TransInfo::TransInfo() {}
TransInfo::TransInfo(int f, int v, bool r) : faceId(f), vertId(v), reversed(r) {}

Face::Face(int a, int b, int c, vector<ofVec3f> *vec) {
	v[0] = a, v[1] = b, v[2] = c;
	if(vec != NULL && (int)vec->size() > max(a, max(b, c))) {
		ofVec3f v1 = (*vec)[b] - (*vec)[a];
		ofVec3f v2 = (*vec)[c] - (*vec)[a];
		normal = v1.getCrossed(v2).normalized();
		planePoints[0] = ofVec2f(0, 0);
		planePoints[1] = ofVec2f(v1.length(), 0);
		planePoints[2] = ofVec2f(v2.length(), 0).rotated(v1.angle(v2));
		for(int i = 0; i < 3; i++) cout << planePoints[i].x << " " << planePoints[i].y << ", ";
		cout << endl;
		
		rot.makeIdentityMatrix();
		ofVec3f w1 = planePoints[1], w2 = planePoints[2];
		rot.makeRotationMatrix(w1, v1);
		cout << fixed << "w1 = " << ofVec3f(planePoints[1]) << "   w2 = " << ofVec3f(planePoints[2])  << endl;
		w1 = w1 * rot;
		w2 = w2 * rot;
//		ofMatrix4x4 rot2;
		double angle = w1.getCrossed(w2).angle(v1.getCrossed(v2));
		if(v1.angle(w1.getCrossed(w2).getCrossed(v1.getCrossed(v2))) > EPS) {
			angle = -angle;
		}
		rot *= ofMatrix4x4().newRotationMatrix(angle, v1);
		cout.precision(1);
		cout << "angle = " << w1.getCrossed(w2).angle(v1.getCrossed(v2)) << endl;
		cout << fixed << "w1 x w2 = " << w1.getCrossed(w2) << " v1 x v2 = " << v1.getCrossed(v2) << endl;
		cout << fixed << "w1 = " << w1 << "   w2 = " << w2 << endl;
		cout << fixed << "u1 = " << ofVec3f(planePoints[1]) * rot << "   u2 = " << ofVec3f(planePoints[2]) * rot << endl;
		cout << fixed << "v1 = " << v1 << "   v2 = " << v2 << endl;
		
	}

}

Data3D::Data3D(Turtle &turtleRef) :turtle(turtleRef) {
}

Data3D::~Data3D() {

}

void Data3D::calculateScaleRatio() {
	maxDist = 0;
	for(int i = 0;  i < (int)verts.size(); i++) {
		for(int j = 0;  j < (int)verts.size(); j++) {
			maxDist = max(maxDist, verts[i].distance(verts[j]));
		}
	}
	cout << "maxDist = " << maxDist << endl;
	scaleRatio = 1.0 / maxDist;
	normalSphereRadius *= maxDist;
}

void Data3D::createTexture() {
	int numFaces = faces.size();
	int w = sqrt(numFaces);
	int h = numFaces / w;
	assert(w != 0);
	while(h * w < numFaces) w++;
	cout << "w = " << w << " h = " << h << "numFaces = " << numFaces << endl;
	texture.allocate(w * faceResolution, h * faceResolution, OF_IMAGE_COLOR);
	for(int i = 0; i < texture.width; i++) {
		for(int j = 0; j < texture.height; j++) {
			texture.setColor(i, j, ofColor::blue);
		}
	}
//	ofImage white;
//	white.loadImage("white.png");
//	white.resize(texture.width, texture.height);
//	texture = white;
//	texture.loadImage("texture.jpg");
	
	

	for(int i = 0; i < w; i++) {
		if(i * w >= numFaces) break;
		for(int j = 0; j < h; j++) {
			if(i * w + j >= numFaces) break;
			coords.push_back(ofVec2f(i * faceResolution, j * faceResolution));
			coords.push_back(ofVec2f(i * faceResolution, (j + 1) * faceResolution - 1));
			coords.push_back(ofVec2f((i + 1) * faceResolution - 1, j * faceResolution));
//			int sz = coords.size();
//			line(coords[sz - 2], coords[sz - 1], ofColor::black, texture);
//			line(coords[sz - 3], coords[sz - 1], ofColor::black, texture);
//			line(coords[sz - 2], coords[sz - 3], ofColor::black, texture);
		}
	}
	
	
}

void Data3D::calculateFaceResolution() {
	faceResolution = 2000 / sqrt(faces.size());
}

void Data3D::setup(fstream *setupFile) {
	string meshFileName;
	*setupFile >> meshFileName;
	*setupFile >> faceResolution;
	*setupFile >> normalSphereRadius;
	fstream meshFile;
	meshFile.open(meshFileName.c_str(), fstream::in);
	if(!meshFile.good()) {
		cout << "Corrupted mesh file." << endl;
		ofExit(1);
	}
	string type;
	while(!meshFile.eof()) {
		meshFile >> type;
		if(type == "v") {
			float x, y, z;
			meshFile >> x >> y >> z;
			verts.push_back(ofVec3f(x, y, z));
			cout << "vert " << x << " " << y << " " << z << " " << endl;
		} else if(type == "f") {
			int a, b, c;
			meshFile >> a >> b >> c;
			faces.push_back(Face(a - 1, b - 1, c - 1, &verts));
			cout << "face " << a << " " << b << " " << c << " " << endl;
		}
	}
	
	faces.pop_back(); // reads last face twice
	
	//Function below is SLOW. Oh man it is!
	
//	for(int i = 0; i < (int)faces.size(); i++) {
//		for(int j = 0; j < (int)faces.size(); j++) if(i != j) {
//			for(int k = 0; k < 3; k++) {
//				for(int l = 0; l < 3; l++) {
//					pair<int, int> p1 = make_pair(faces[i].v[k], faces[i].v[(k + 1) % 3]);
//					pair<int, int> p2 = make_pair(faces[j].v[l], faces[j].v[(l + 1) % 3]);
//					bool rev = false;
//					if((p1.first < p1.second) != (p2.first < p2.second)) {
//						swap(p1.first, p1.second);
//						rev = true;
//					}		
//					if(p1 == p2) {
//						faces[i].nei[k] = TransInfo(j, l, rev);
//						cout << "pair " << p1.first << " " << p1.second << " faces " << i << " and " << j << " rev = " << rev << endl;
//					}
//				}
//			}
//		}
//	}
	
	adjacentFaces.resize(verts.size());
	
	map<pair<int, int> , int> edgeMap;
	
	for(int i = 0; i < (int)faces.size(); i++) {
		for(int j = 0; j < 3; j++) {
			int a = faces[i].v[j], b = faces[i].v[(j + 1) % 3];
			adjacentFaces[faces[i].v[j]].push_back(i);
			if(edgeMap.find(make_pair(a, b)) != edgeMap.end()) {
				int f = edgeMap[make_pair(a, b)];
				int l = 0;
				for(; faces[f].v[l] != a; l++);
				faces[i].nei[j] = TransInfo(f, l, false);
				faces[f].nei[l] = TransInfo(i, j, false);
			} else if(edgeMap.find(make_pair(b, a)) != edgeMap.end()) {
				int f = edgeMap[make_pair(b, a)];
				int l = 0;
				for(; faces[f].v[l] != b; l++);
				faces[i].nei[j] = TransInfo(f, l, true);
				faces[f].nei[l] = TransInfo(i, j, true);
			} else {
				edgeMap[make_pair(a, b)] = i;
			}
		}
	}
	
	
	calculateScaleRatio();
	if(faceResolution == 0) calculateFaceResolution();
	createTexture();
	turtle.pos = (faces[0].planePoints[0] + faces[0].planePoints[1] + faces[0].planePoints[2]) * (1.0 / 3.0);
	meshFile.close();
}

void Data3D::giveTurtleMesh(vector<ofVec3f> &vertices, vector<ofVec2f> &coordinates, vector<int> &triangles) {
	string meshFileName = "meshes/turtle.obj";
	fstream meshFile;
	vertices.clear(), triangles.clear();
	meshFile.open(meshFileName.c_str(), fstream::in);
	if(!meshFile.good()) {
		cout << "Corrupted turtle mesh file." << endl;
		ofExit(1);
	}
	string type;
	while(!meshFile.eof()) {
		meshFile >> type;
		if(type == "v") {
			float x, y, z;
			meshFile >> x >> y >> z;
			vertices.push_back(ofVec3f(x, y, z));
			cout << "vert " << x << " " << y << " " << z << " " << endl;
		} else if(type == "f") {
			int v[3];
			cout << "face ";
			for(int i = 0; i < 3; i++) {
				meshFile >> v[i];
				cout << v[i] << " ";
				triangles.push_back(v[i] - 1);
			}
			cout << endl;
		}
	}
	for(int i = 0; i < 3; i++) {
		triangles.pop_back(); // reads last face twice
	}
	meshFile.close();
}

void Data3D::giveMesh(vector<ofVec3f> &vertices, vector<ofVec2f> &coordinates, vector<int> &triangles) {
	vertices = verts;
	coordinates = coords;
	triangles.clear();
	for(int i = 0;  i < (int)faces.size(); i++) {
		for(int j = 0; j < 3; j++) {
			triangles.push_back(faces[i].v[j]);
		}
	}

}

float Data3D::getScaleRatio() {
	return scaleRatio;
}

float area(ofVec2f a, ofVec2f b, ofVec2f c) {
	//cout << "area " << a << " " << b << " " << c << " = ";
	a -= c, b -= c;
//	cout << 0.5 * abs(a.x * b.y - a.y * b.x) << endl;
	return 0.5 * abs(a.x * b.y - a.y * b.x);
}

ofVec2f translate(ofVec2f p, vector<ofVec2f> tr1, vector<ofVec2f> tr2) {
	float w = area(p, tr1[0], tr1[1]) / area(tr1[0], tr1[1], tr1[2]);
	float v = area(p, tr1[0], tr1[2]) / area(tr1[0], tr1[1], tr1[2]);
	ofVec2f res = tr2[0] + v * (tr2[1] - tr2[0]) + w * (tr2[2] - tr2[0]);
//	cout << "map " << p << " = " << res << endl;
//	for(int i = 0 ; i < 3; i++) cout << tr1[i] << " ";
//	cout << endl;
//	for(int i = 0 ; i < 3; i++) cout << tr2[i] << " ";
//	cout << endl;
	return res;
}


void Data3D::drawSegment(ofVec2f p1, ofVec2f p2, int faceId, double step) {
	double dist = (p2 - p1).length();
	while(dist > 1.0) {
		turtle.pos += turtle.dir * step;
		updateOrthoCast();
		this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
		dist -= step;
	}
	turtle.pos += turtle.dir * dist;
	updateOrthoCast();
	this_thread::sleep(posix_time::milliseconds((int)round(dist * (double)World::instance()->getFrameTime())));
	
	vector<ofVec2f> tr1, tr2;
	for(int i = 0; i < 3; i++) {
		tr1.push_back(faces[faceId].planePoints[i]);
		tr2.push_back(coords[3 * faceId + i]);
	}
	p1 = translate(p1, tr1, tr2);
	p2 = translate(p2, tr1, tr2);
	line(p1, p2, ofColor::red, texture, true);
	
}

void Data3D::forward(double dist) {
	forwardStep(dist);
//	double step = (dist > 0) ? 1.0 : -1.0;
//	while(abs(dist) > 1.0) {
//		forwardStep(step);
//		this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
//		dist -= step;
//	}
//	forwardStep(dist);
//	this_thread::sleep(posix_time::milliseconds((int)round(dist * (double)World::instance()->getFrameTime())));
}

void Data3D::forwardStep(double dist) {
	if(abs(dist) <= EPS) return;
	dist = dist * maxDist / 500.0;
	point col;
	ofVec2f pos2;
	bool move = true;
	while(move) {
		pos2 = turtle.pos + turtle.dir * dist;
//		cout << "dist = " << dist << " pos2 = " << pos2 << endl;
		move = false;
		for(int i = 0; i < 3; i++) {
			if(intersect(segment(turtle.pos, pos2), segment(
					faces[turtle.faceId].planePoints[i], 
					faces[turtle.faceId].planePoints[(i + 1) % 3]
					), col)) {
				cout << "intersection " << col.x << ", " << col.y << endl;
//				Interpreter::instance()->toggleRunning();
				
				dist -= turtle.pos.distance(ofVec2f(col.x, col.y));
				drawSegment(turtle.pos, ofVec2f(col.x, col.y), turtle.faceId, maxDist / 500.0);
//				drawSegment((faces[turtle.faceId].planePoints[0] + faces[turtle.faceId].planePoints[1] + faces[turtle.faceId].planePoints[2]) 
//						* (1.0 / 3.0), ofVec2f(col.x, col.y), turtle.faceId);
				TransInfo info = faces[turtle.faceId].nei[i];

				ofVec2f edge1 = faces[turtle.faceId].planePoints[(i + 1) % 3] - faces[turtle.faceId].planePoints[i];
				ofVec2f edge2 = faces[info.faceId].planePoints[(info.vertId + 1) % 3] - faces[info.faceId].planePoints[info.vertId];
				ofVec2f edge = edge2;
				float angle;
				cout << "edges " << edge1.x << " " << edge1.y << ", " << edge2.x << " " << edge2.y << endl;
				if(!info.reversed) {
					edge.scale(faces[turtle.faceId].planePoints[i].distance(ofVec2f(col.x, col.y)));
				} else {
					edge.scale(faces[turtle.faceId].planePoints[(i + 1) % 3].distance(ofVec2f(col.x, col.y)));
					edge1 = -edge1;
				}
				cout << edge.x << " " << edge.y << endl;
				turtle.pos = faces[info.faceId].planePoints[info.vertId] + edge;
//				turtle.pos = faces[info.faceId].planePoints[0] + faces[info.faceId].planePoints[1] + faces[info.faceId].planePoints[2];
//				turtle.pos *= (1.0 / 3.0);
				angle = edge1.angle(turtle.dir);
				turtle.dir = edge2.rotated(angle);
				turtle.pos += turtle.dir * (maxDist / 50000.0);
				dist -= maxDist / 50000.0;
				
				turtle.faceId = info.faceId;
				if(dist <= 0) return;
				move = true;
				cout << "pos = " << turtle.pos << ", dir = " << turtle.dir << ", faceId = " << turtle.faceId << endl;
				break;

			}
		}
	}
	drawSegment(turtle.pos, pos2, turtle.faceId, maxDist / 500.0);
	turtle.pos = pos2;
}

void Data3D::rotate(double angle) {
	double step = (angle > 0) ? 1.0 : -1.0;
	while(abs(angle) > 1.0) {
		turtle.dir.rotate(step);
		this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
		angle -= step;
	}
	turtle.dir.rotate(angle);
	this_thread::sleep(posix_time::milliseconds((int)round(angle * (double)World::instance()->getFrameTime())));
}

void Data3D::giveTurtleCoords(ofVec3f &pos, ofVec3f &dir, ofVec3f &dirUp) {
	Face cFace = faces[turtle.faceId];
	ofVec3f tp = turtle.pos, td = turtle.dir;
	pos = ofVec3f(tp) * cFace.rot + verts[cFace.v[0]];
	dir = ofVec3f(td) * cFace.rot;
	dirUp = cFace.normal;
	
}

void Data3D::updateOrthoCast() {
	updateOrthoNormal();
	orthoCast.resize(1);
	orthoCast[0].push_back(ofVec2f(200, 200));
	orthoCast[0].push_back(ofVec2f(400, 200));
	orthoCast[0].push_back(ofVec2f(500, 500));
}

void Data3D::updateOrthoNormal() {
//	ofVec3f res(0, 0, 0);
//	for(int i = 0; i < (int)faces.size(); i++) {
//		res += faces[i].normal.normalized() * areaInSphere(i);
//	}
//	orthoPlaneNormal = res;
	orthoPlaneNormal = faces[turtle.faceId].normal.normalized();
	cout << "ortho normal" << orthoPlaneNormal << endl;
}

float Data3D::areaInSphere(int f) {
	ofVec3f v1 = verts[faces[f].v[1]] - verts[faces[f].v[0]];
	ofVec3f v2 = verts[faces[f].v[2]] - verts[faces[f].v[0]];
	ofVec3f w = turtle.pos - verts[faces[f].v[0]];
	ofVec3f p = v1.getPerpendicular(v2);
	
	float r = normalSphereRadius * normalSphereRadius - p.dot(w) * p.dot(w);
	if(r <= 0) return 0.0;
	r = pow(r, 0.5);
	
	ofVec3f o = p.getCrossed(w).rotated(-90, ofVec3f(0, 0, 0), p);
	ofVec3f o2 = w - p * (p.dot(w));
//	cout << v1 << " " << v2 << " " << w << " " << p << " " << o << " " << o2 << endl;
	assert(o.match(o2));
	ofVec3f t1 = -o,t2 = -o + v1, t3 = -o + v2;
	ofVec2f n(1, 0);
	ofVec2f p1 = n * t1.length();
	ofVec2f p2 = n.getRotated(angle(t1, t2)) * t2.length();
	ofVec2f p3 = n.getRotated(angle(t1, t3)) * t3.length();
	
	if(abs((p2 - p3).length() - (t2 - t3).length()) > EPS) {
		p3 = n.getRotated(-angle(t1, t3)) * t3.length();
	}
	
	cout.precision(2);
//	cout << t1 << " " << t2 << " " << t3 << endl;
//	cout << p1 << " " << p2 << " " << p3 << endl;
	
	assert(abs((p1 - p2).length() - (t1 - t2).length()) < EPS
			&& abs((p1 - p3).length() - (t1 - t3).length()) < EPS
			&& abs((p2 - p3).length() - (t2 - t3).length()) < EPS
			);

	
	
	return areaInCircle(p1, p2, p3, r);
}

