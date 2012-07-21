#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

#include "ofMain.h"
#include "turtle.h"
#include "geometry.h"

using namespace std;

struct TransInfo {
	int faceId;
	int vertId;
	bool reversed;
	TransInfo();
	TransInfo(int f, int v, bool r);
};

struct Face {
	int v[3];
	TransInfo nei[3];
	ofVec2f planePoints[3];
	Face(int a, int b, int c, vector<ofVec3f> *vec = NULL);
//	void findNeighbours();
};


class Data3D {
public:
	~Data3D();
	void setup(fstream *setupFile, Turtle &turtle);
	void createTexture();
	void giveMesh(vector<ofVec3f> &vertices, vector<ofVec2f> &coordinates, vector<int> &triangles);
	float getScaleRatio();
	void drawSegment(ofVec2f p1, ofVec2f p2, int faceId);

	ofImage texture;
	void forward(Turtle &turtle, double dist);
	void rotate(Turtle &turtle, double angle);
	vector<ofVec3f> verts;
	vector<ofVec2f> coords;
	vector<Face> faces;
private:
	void calculateScaleRatio();
	void calculateFaceResolution();
	float scaleRatio;
	float maxDist;
	int faceResolution;

};

ofVec2f translate(ofVec2f p, vector<ofVec2f> tr1, vector<ofVec2f> tr2);
float area(ofVec2f a, ofVec2f b, ofVec2f c);
