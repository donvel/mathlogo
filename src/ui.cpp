#include "ui.h"

//--------------------------------------------------------------
void logoApp::setup(){
	ofBackground(World::instance()->getBackgroundColor());
	for(int i = 0; i < World::instance()->numViewports(); i++) {
		buffer[i].allocate(World::instance()->getWidth(), World::instance()->getHeight(), OF_IMAGE_COLOR);
	}
}

//--------------------------------------------------------------
void logoApp::update(){
	// Nothing to be done here.
}

//--------------------------------------------------------------

ofColor logoApp::getPixel(gridPoint p, int i) {
//	unsigned char rgb[3];
//	glReadPixels(p.x, p.y, 1,1, GL_RGB, GL_UNSIGNED_BYTE, rgb);
////	if(rgb[0] != 255) {
////		cout << "getPixel(" << p.x << ", " << p.y << ") = " << (int)rgb[0] << " " << (int)rgb[1] << " " << (int)rgb[2] << endl;
////	}
//	return ofColor(rgb[0], rgb[1], rgb[2]);
	return buffer.getColor(p.x, p.y);
	
}

gridPoint logoApp::neighbour(gridPoint p, int i) {
	switch(i) {
		case 0:
			return gridPoint(p.x + 1, p.y);
		break;
		case 1:
			return gridPoint(p.x, p.y + 1);
		break;
		case 2:
			return gridPoint(p.x - 1, p.y);
		break;
		case 3:
			return gridPoint(p.x, p.y - 1);
		break;
		
	}
	return p;
}

void logoApp::bfsFill(gridPoint p, int w, int h, ofColor targetColor) {
	p.y = h - p.y;
	ofColor firstColor = getPixel(p);

	
	vector< vector<bool> > vis;
	vis.resize(w);
	for(int i = 0; i < w; i++) {
		vis[i].resize(h, false);
	}
	
	vector<gridPoint> qu;
	int it = 0;
	qu.push_back(p);
	
	while(it < (int)qu.size()) {
		gridPoint v = qu[it++], u;
		for(int i = 0; i < 4; i++) {
			u = neighbour(v, i);
			if(u.x >= 0 && u.x < w && u.y >= 0 && u.y < h && !vis[u.x][u.y] &&
					getPixel(u) == firstColor) {
				vis[u.x][u.y] = true;
				qu.push_back(u);
			}
		}

		buffer.setColor(v.x, h - v.y, targetColor);
	}	
	cout << "qu size = " << qu.size() << endl;
}

void logoApp::draw() {

	int w = World::instance()->getWidth();
	int h = World::instance()->getHeight();
	
	int numViewports = World::instance()->numViewports();
	ofBackground(World::instance()->getBackgroundColor());
	if(numViewports == 2) {// In this mode we have two viewports
		ofLine(w, 0, w, h);
		//Draws a line separating the two viewports
	}
	
	for(int i = 0; i < numViewports; i++) { // NOTE: this procedure is in fact quite slow, 
		//because for every frame it redraws entire turtle trace
		//it could be modified as to draw only new segments
		//however so far it is enough fast
		gridPoint ori = World::instance()->getOrigin(i);
		//We need to know the relative position of the origin, so 'Turtle coordinates' may be translated into OF coordinates
		//while(World::instance()->changing);
		vector<segment> *trace = World::instance()->getTrace(i);
		//World::instance()->frozen = true;
		//Entire turtle trace
		for(int j = 0; j < (int)trace->size(); j++) {
		//	cout << (*trace)[j].a.x << " " << (*trace)[j].a.y << " " << (*trace)[j].b.x << " " << (*trace)[j].b.y << " " << (*trace)[j].color << endl;
			ofSetColor((*trace)[j].color);
//			// different parts may have different colors
			segment cSeg = (*trace)[j];
			if(World::instance()->crop(cSeg)) {// Draw only the part of the segment which is visible in the viewport
				ofLine(cSeg.a.x + ori.x, cSeg.a.y + ori.y, cSeg.b.x + ori.x, cSeg.b.y + ori.y);
			}
		}
		buffer.grabScreen(i * w, 0, w, h);
		vector<Filler> *filler = World::instance()->getFiller(i);
		//World::instance()->frozen = true;
		//Entire turtle trace
		for(int j = 0; j < (int)filler->size(); j++) {
//			cout << trace[j].a.x << " " << trace[j].a.y << " " << trace[j].b.x << " " << trace[j].b.y << " " << trace[j].color.black << endl;
			//ofSetColor((*filler)[j].second);
//			// different parts may have different colors
			point p = (*filler)[j].first;
			if(!World::instance()->outside(p)) {
				bfsFill(p.translated(vect(ori)), w, h, (*filler)[j].second);
			}
		}
		ofSetColor(World::instance()->getBackgroundColor());
		buffer.draw(i * w, 0);
		
		
		//World::instance()->frozen = false;
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
	this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
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