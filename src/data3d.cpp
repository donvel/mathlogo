#include "data3d.h"
#include "geometry.h"

Face::Face(int a, int b, int c) {
	v[0] = a, v[1] = b, v[2] = c;
}

void Data3D::calculateScaleRatio() {
	float maxDist = 0;
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
	texture.allocate(w * faceResolution, h * faceResolution, OF_IMAGE_COLOR);
//	for(int i = 0; i < texture.width; i++) {
//		for(int j = 0; j < texture.height; j++) {
//			
//		}
//	}
	for(int i = 0; i < w; i++) {
		if(i * w >= numFaces) break;
		for(int j = 0; j < h; j++) {
			if(i * w + j >= numFaces) break;
			coords.push_back(ofVec2f(i * faceResolution, j * faceResolution));
			coords.push_back(ofVec2f(i * faceResolution, j * faceResolution + 1));
			coords.push_back(ofVec2f(i * faceResolution + 1, j * faceResolution));
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
		} else if(type == "f") {
			int a, b, c;
			meshFile >> a >> b >> c;
			faces.push_back(Face(a - 1, b - 1, c - 1));
		}
	}
	
	calculateScaleRatio();
	if(faceResolution == 0) calculateFaceResolution();
	createTexture();
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

void Data3D::forward(Turtle& turtle, double dist) {
	point col;
	bool move = true;
	while(move) {
		ofVec2f pos2 = turtle.pos + turtle.dir * dist;
		move = false;
		for(int i = 0; i < 2; i++) {
			if(intersect(segment(turtle.pos, pos2), col)) {
				drawSegment(turtle.pos, ofVec2f(colx, col.y));
				move = true;
			}
		}
	}
}

void Data3D::rotate(Turtle& turtle, double angle) {
	turtle.dir.rotate(angle);
}