#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "ofMain.h"

using namespace std;


class Data3D {
public:
	void setup(fstream *setupFile);
	void giveMesh(vector<ofVec3f> &vertices, vector<int> &triangles);
private:

};

