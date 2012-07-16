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
		ofColor getPixel(gridPoint p);
		gridPoint neighbour(gridPoint p, int i);
		void bfsFill(gridPoint p, int w, int h, ofColor targetColor);
		void line (double x1, double y1, double x2, double y2);
		void setup();
		void update();
		void draw();
		ofImage buffer[2];
		int fillIter[2], traceIter[2];
		
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
};

