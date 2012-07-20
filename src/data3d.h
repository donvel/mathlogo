#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

#include "ofMain.h"
#include "turtle.h"

using namespace std;

struct Face {
	int v[3];
	Face(int a, int b, int c);
};


class Data3D {
public:
	void setup(fstream *setupFile);
	void createTexture();
	void giveMesh(vector<ofVec3f> &vertices, vector<ofVec2f> &coordinates, vector<int> &triangles);
	float getScaleRatio();
	ofImage texture;
	void forward(Turtle &turtle, double dist);
	void rotate(Turtle &turtle, double angle);
private:
	void calculateScaleRatio();
	void calculateFaceResolution();
	float scaleRatio;
	int faceResolution;
	vector<ofVec3f> verts;
	vector<ofVec2f> coords;
	vector<Face> faces;
};

