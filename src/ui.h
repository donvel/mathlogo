#pragma once

#include <cstdio>
#include <vector>
#include <boost/thread.hpp>

#include "ofMain.h"
#include "interpreter.h"
#include "world.h"
#include "geometry.h"

using namespace std;
using namespace boost;

class logoApp : public ofBaseApp{
	public:
//		ofColor getPixel(gridPoint p);
		
		void bfsFill(gridPoint p, ofColor targetColor, int id);
		
		void setup();
		void update();
		void draw();
		
//-------------3D handling--------------//
		void setup3D();
		void update3D();
		void draw3D();
		void drawVieport2D();
		void updateTurtleMesh(ofVec3f pos, ofVec3f dir, ofVec3f dirUp);
		
		ofImage buffer[2];
		ofImage saver;
		int fillIter[2], traceIter[2];
		int w, h;
		bool saveScreen;
		
		void keyPressed(int key);
		/*Obsolete commands*/
//		void keyReleased(int key);
//		void mouseMoved(int x, int y);
//		void mouseDragged(int x, int y, int button);
//		void mousePressed(int x, int y, int button);
//		void mouseReleased(int x, int y, int button);
//		void windowResized(int w, int h);
//		void dragEvent(ofDragInfo dragInfo);
//		void gotMessage(ofMessage msg);
		
		ofEasyCam cam;
//		ofMesh mesh;
		ofVboMesh vboMesh, turtleMesh, turtleMeshTrans;
		ofImage *img;
		
		ofRectangle viewport2D;
		ofRectangle viewport3D;
		
		bool drawWireframe;
};

