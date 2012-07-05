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
	// Works only when the world is a plane <=> its depth == 1
    gridPoint ori = World::instance()->getOrigin();
	for(int x = 0; x < World::instance()->getWidth(); x++) {
		for(int y = 0; y < World::instance()->getHeight(); y++) {
			gridPoint gp(x - ori.x, y - ori.y, 0);
			ofSetColor(World::instance()->getVoxel(gp)->color);
			ofCircle(x, y, 1);
		}
	}
	vector<point> turtleShape = World::instance()->getTurtleShape();	
	ofSetColor(0, 0, 0);
	ofTriangle(turtleShape[0].x, turtleShape[0].y, turtleShape[1].x, turtleShape[1].y, turtleShape[2].x, turtleShape[2].y);
	
	
}

//--------------------------------------------------------------
void logoApp::keyPressed(int key){
	if(key == ' ') {
		cout << "Space pressed" << endl;
		Interpreter::instance()->toggleRunning();
	} else if(key == 'q') {
		cout << "\"q\" pressed" << endl;
		ofExit(0); // This doesn't work yet.
	}
}

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
