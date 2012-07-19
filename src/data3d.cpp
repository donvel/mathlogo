#include "data3d.h"

void Data3D::setup(fstream *setupFile) {
	
}

void Data3D::giveMesh(vector<ofVec3f> &vertices, vector<int> &triangles) {
	vertices.push_back(ofVec3f(0, 0, 0));
	vertices.push_back(ofVec3f(100, 0, 0));
	vertices.push_back(ofVec3f(0, 100, 0));
	vertices.push_back(ofVec3f(0, 0, 100));
	int tab[] = {1, 2, 3, 1, 3, 4, 1, 2, 4, 2, 3, 4};
	for(int i = 0; i < 12; i++) triangles.push_back(tab[i] - 1);

}