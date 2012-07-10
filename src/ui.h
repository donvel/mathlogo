#pragma once

#include <cstdio>
#include <vector>

#include "ofMain.h"
#include "interpreter.h"
#include "world.h"
#include "geometry.h"



class logoApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		ofFbo fbo;
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

