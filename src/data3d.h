#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <boost/thread.hpp>

#include "ofMain.h"
#include "turtle.h"
#include "geometry.h"


using namespace std;
using namespace boost;

struct TransInfo {
	int faceId;
	int vertId;
	bool reversed;
	TransInfo();
	TransInfo(int f, int v, bool r);
};

struct Face {
	ofMatrix4x4 rot;
	int v[3];
	TransInfo nei[3];
	ofVec2f planePoints[3];
	ofVec3f normal;
	Face(int a, int b, int c, vector<ofVec3f> *vec = NULL);
//	void findNeighbours();
};


class Data3D {
public:
	Data3D(Turtle &turtleRef);
	~Data3D();
	void setup(fstream *setupFile);
	void createTexture();
	void giveMesh(vector<ofVec3f> &vertices, vector<ofVec2f> &coordinates, vector<int> &triangles);
	void giveTurtleMesh(vector<ofVec3f> &vertices, vector<ofVec2f> &coordinates, vector<int> &triangles);
	void giveTurtleCoords(ofVec3f &pos, ofVec3f &dir, ofVec3f &dirUp);
	float getScaleRatio();
	void drawSegment(ofVec2f p1, ofVec2f p2, int faceId, double step = 1.0);
	
	
	string cameraType;
	ofImage texture;
	void forward(double dist);
	void forwardStep(double dist);
	void rotate(double angle);
	vector<ofVec3f> verts;
	vector<ofVec2f> coords;
	vector<Face> faces;
	Turtle &turtle;
private:
	void calculateScaleRatio();
	void calculateFaceResolution();
	float scaleRatio;
	float maxDist;
	int faceResolution;
	

};

ofVec2f translate(ofVec2f p, vector<ofVec2f> tr1, vector<ofVec2f> tr2);
float area(ofVec2f a, ofVec2f b, ofVec2f c);
