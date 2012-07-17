#include "ui.h"

//--------------------------------------------------------------
void logoApp::setup(){
	w = World::instance()->getWidth();
	h = World::instance()->getHeight();
	ofBackground(World::instance()->getBackgroundColor());
	for(int i = 0; i < World::instance()->numViewports(); i++) {
		buffer[i].allocate(w, h, OF_IMAGE_COLOR);
	}
}

//--------------------------------------------------------------
void logoApp::update(){
	// Nothing to be done here.
}

//--------------------------------------------------------------



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

void logoApp::line (point p1, point p2, ofColor targetColor, int id) {
	double d = dist(p1, p2);
	for(double alpha = 0; alpha <= d + EPS; alpha += 0.5) {
		point p = point(p1.x * (alpha / d) + p2.x * (1 - alpha / d),
				p1.y * (alpha / d) + p2.y * (1 - alpha / d));
		buffer[id].setColor(round(p.x), round(p.y), targetColor);
	}
}

void logoApp::bfsFill(gridPoint p, ofColor targetColor, int id) {
	
	ofColor firstColor = buffer[id].getColor(p.x, p.y);
	if(firstColor == targetColor) return;

	vector<gridPoint> qu;
	int it = 0;
	qu.push_back(p);
	
	while(it < (int)qu.size()) {
		gridPoint v = qu[it++], u;
		for(int i = 0; i < 4; i++) {
			u = neighbour(v, i);
			if(u.x >= 0 && u.x < w && u.y >= 0 && u.y < h &&
					buffer[id].getColor(u.x, u.y) == firstColor) {
				buffer[id].setColor(u.x, u.y, targetColor);
				qu.push_back(u);
			}
		}
	}	
	cout << "qu size = " << qu.size() << endl;
}

void logoApp::draw() {
	
	int numViewports = World::instance()->numViewports();

	for(int i = 0; i < numViewports; i++) { 
		point ori = World::instance()->getOrigin(0);
		//We need to know the relative position of the origin, so 'Turtle coordinates' may be translated into OF coordinates
		//while(World::instance()->changing);
		vector<segment> *trace = World::instance()->getTrace(i);
		vector<Filler> *filler = World::instance()->getFiller(i);
	
		if(World::instance()->cleared[i]) {
			traceIter[i] = 0;
			fillIter[i] = 0;
			ofColor bgColor = World::instance()->getBackgroundColor();
			for(int j = 0; j < w; j++) {
				for(int k = 0; k < h; k++) {
					buffer[i].setColor(j, k, bgColor);
				}
			}
			World::instance()->cleared[i] = false;
		}
		for(int &j = traceIter[i]; j < (int)trace->size(); j++) {
		//	cout << (*trace)[j].a.x << " " << (*trace)[j].a.y << " " << (*trace)[j].b.x << " " << (*trace)[j].b.y << " " << (*trace)[j].color << endl;
			segment cSeg = (*trace)[j];
			if(World::instance()->crop(cSeg)) {// Draw only the part of the segment which is visible in the viewport
				line(cSeg.a.translated(ori), cSeg.b.translated(ori), (*trace)[j].color, i);
			}
		}

		for(int &j = fillIter[i]; j < (int)filler->size(); j++) {
			point p = (*filler)[j].first;
			if(!World::instance()->outside(p)) {
				bfsFill(p.translated(vect(ori)),(*filler)[j].second, i);
			}
		}
		ofSetColor(World::instance()->getBackgroundColor());
		buffer[i].update();
		buffer[i].draw(i * w, 0);
	}
	for(int i = 0; i < numViewports; i++) { 
		point ori = World::instance()->getOrigin(i);
		
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
	if(numViewports == 2) {// In this mode we have two viewports
		ofLine(w, 0, w, h);
		//Draws a line separating the two viewports
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