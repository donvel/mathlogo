#include "ui.h"

//--------------------------------------------------------------
void logoApp::setup(){
	ofBackground(World::instance()->getBackgroundColor());
}

//--------------------------------------------------------------
void logoApp::update(){
	// Nothing to be done here.
}

//--------------------------------------------------------------
void logoApp::draw(){
	int numVieports = 1;
	ofBackground(World::instance()->getBackgroundColor());
	if(World::instance()->getMode() == TRANSFORM) {// In this mode we have two viewports
		numVieports = 2;
		ofLine(World::instance()->getWidth(), 0, World::instance()->getWidth(), World::instance()->getHeight());
		//Draws a line separating the two viewports
	}
	
	for(int i = 0; i < numVieports; i++) { // NOTE: this procedure is in fact quite slow, 
		//because for every frame it redraws entire turtle trace
		//it could be modified as to draw only new segments
		//however so far it is enough fast
		gridPoint ori = World::instance()->getOrigin(i);
		//We need to know the relative position of the origin, so 'Turtle coordinates' may be translated into OF coordinates
		vector<segment> trace = World::instance()->getTrace(i);
		//Entire turtle trace
		for(int j = 0; j < (int)trace.size(); j++) {
			ofSetColor(trace[j].color);
			// different parts may have different colors
			segment cSeg = trace[j];
			if(World::instance()->crop(cSeg)) {// Draw only the part of the segment which is visible in the viewport
				ofLine(cSeg.a.x + ori.x, cSeg.a.y + ori.y, cSeg.b.x + ori.x, cSeg.b.y + ori.y);
			}
		}
		//We get three points which define the turtle's triangular shape
		vector<point> turtleShape = World::instance()->getTurtleShape(i);	
		ofSetColor(0, 0, 0); // Let the turtle be always black
		segment turtleSide;
		for(int j = 0; j < 3; j++) {
			turtleSide = segment(turtleShape[j], turtleShape[(j + 1) % 3]);
			if(World::instance()->crop(turtleSide)) {// Draw only the part of the side which is visible in the viewport
				ofLine(turtleSide.a.x + ori.x, turtleSide.a.y + ori.y, turtleSide.b.x + ori.x, turtleSide.b.y + ori.y);
			}
		}
	}
}

//--------------------------------------------------------------
void logoApp::keyPressed(int key){
	if(key == ' ') {
		cout << "Space pressed" << endl;
		Interpreter::instance()->toggleRunning();
	} else if(key == 'q') {
		cout << "\"q\" pressed" << endl;
		ofExit(0); // exit entire program
	}
}

/* Obsolete commands

//--------------------------------------------------------------
void logoApp::keyReleased(int key){

}

//--------------------------------------------------------------
void logoApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void logoApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void logoApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void logoApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void logoApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void logoApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void logoApp::dragEvent(ofDragInfo dragInfo){ 

}
*/