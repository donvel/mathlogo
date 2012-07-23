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
		rot.makeIdentityMatrix();
		planePoints[0] = ofVec2f(0, 0);
		planePoints[1] = ofVec2f(v1.length(), 0);
		planePoints[2] = ofVec2f(v2.length(), 0).rotated(v1.angle(v2));
		for(int i = 0; i < 3; i++) cout << planePoints[i].x << " " << planePoints[i].y << ", ";
		cout << endl;
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
			int sz = coords.size();
			line(coords[sz - 2], coords[sz - 1], ofColor::black, texture);
			line(coords[sz - 3], coords[sz - 1], ofColor::black, texture);
			line(coords[sz - 2], coords[sz - 3], ofColor::black, texture);
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
	for(int i = 0; i < (int)faces.size(); i++) {
		for(int j = 0; j < (int)faces.size(); j++) if(i != j) {
			for(int k = 0; k < 3; k++) {
				for(int l = 0; l < 3; l++) {
					pair<int, int> p1 = make_pair(faces[i].v[k], faces[i].v[(k + 1) % 3]);
					pair<int, int> p2 = make_pair(faces[j].v[l], faces[j].v[(l + 1) % 3]);
					bool rev = false;
					if((p1.first < p1.second) != (p2.first < p2.second)) {
						swap(p1.first, p1.second);
						rev = true;
					}		
					if(p1 == p2) {
						faces[i].nei[k] = TransInfo(j, l, rev);
						cout << "pair " << p1.first << " " << p1.second << " faces " << i << " and " << j << " rev = " << rev << endl;
					}
				}
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

void Data3D::drawSegment(ofVec2f p1, ofVec2f p2, int faceId) {
	vector<ofVec2f> tr1, tr2;
	for(int i = 0; i < 3; i++) {
		tr1.push_back(faces[faceId].planePoints[i]);
		tr2.push_back(coords[3 * faceId + i]);
	}
	p1 = translate(p1, tr1, tr2);
	p2 = translate(p2, tr1, tr2);
	line(p1, p2, ofColor::red, texture, true);
	this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
}

void Data3D::forward(double dist) {
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
				drawSegment(turtle.pos, ofVec2f(col.x, col.y), turtle.faceId);
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
				turtle.pos +=turtle.dir * (maxDist / 50000.0);
				dist -= maxDist / 50000.0;
				
				turtle.faceId = info.faceId;
				if(dist <= 0) return;
				move = true;
				cout << "pos = " << turtle.pos << ", dir = " << turtle.dir << ", faceId = " << turtle.faceId << endl;
				break;

			}
		}
	}
	drawSegment(turtle.pos, pos2, turtle.faceId);
	turtle.pos = pos2;
}

void Data3D::rotate(double angle) {
	turtle.dir.rotate(angle);
}

void Data3D::giveTurtleCoords(ofVec3f &pos, ofVec3f &dir) {
	Face cFace = faces[turtle.faceId];
//	ofVec4f v[3];
//	for(int i = 0; i < 3; i++) v[i] = cFace.planePoints[i];
//	ofVec4f relPos = turtle.pos;
//	ofMatrix4x4 rot1, rot2;
//	rot1.makeRotationMatrix(v[1] - v[0], verts[cFace.v[1]] - verts[cFace.v[0]]);
////	cout << (v[1] - v[0]) * rot1 << " " << verts[cFace.v[1]] - verts[cFace.v[0]] << endl;
//	for(int i = 0; i < 3; i++) {
//		v[i] = v[i] * rot1; 
//	}
////	cout << "tututu -  " << ofVec3f(v[1] - v[0]) << " " << verts[cFace.v[1]] - verts[cFace.v[0]] << endl;
//	rot2.makeRotationMatrix(v[2] - v[0], verts[cFace.v[2]] - verts[cFace.v[0]]);
////	cout << (v[1] - v[0])  << " " << verts[cFace.v[2]] - verts[cFace.v[0]] << endl;
//	for(int i = 0; i < 2; i++) cout << (ofVec3f(cFace.planePoints[i + 1]) - ofVec3f(cFace.planePoints[0])) << "\t";
//	cout << endl;
//	for(int i = 0; i < 2; i++) cout << (ofVec3f(cFace.planePoints[i + 1]) - ofVec3f(cFace.planePoints[0])) * rot1 << "\t";
//	cout << endl;
//	for(int i = 0; i < 2; i++) cout << (ofVec3f(cFace.planePoints[i + 1]) - ofVec3f(cFace.planePoints[0])) * rot1 * rot2 << "\t";
//	cout << endl;
//	for(int i = 0; i < 2; i++) cout << verts[cFace.v[i + 1]] - verts[cFace.v[0]]<< "\t";
//	cout << endl << endl << endl;
//	relPos = relPos * rot1 * rot2;
//	dir = ofVec4f(turtle.dir) * rot1 * rot2;
//	pos = verts[cFace.v[0]] + relPos;
	
	pos = ofVec4f(turtle.pos) * cFace.rot + verts(cFace.v[0]);
	dir = ofVec4f(turtle.dir) * cFace.rot;

	
}