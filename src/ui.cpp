#include "ui.h"

//--------------------------------------------------------------
void logoApp::setup(){
	saveScreen = false;
	if(World::instance()->getMode() == ESCAPE) {
		setup3D();
		return;
	}
	w = World::instance()->getWidth();
	h = World::instance()->getHeight();
	ofBackground(World::instance()->getBackgroundColor());
	for(int i = 0; i < World::instance()->numViewports(); i++) {
		buffer[i].allocate(w, h, OF_IMAGE_COLOR);
	}
}

//--------------------------------------------------------------
void logoApp::update(){
	if(World::instance()->getMode() == ESCAPE) {
		update3D();
	}
}

//--------------------------------------------------------------


//void line (point p1, point p2, ofColor targetColor, ofImage &img) {
//	double d = dist(p1, p2);
//	for(double alpha = 0; alpha <= d + EPS; alpha += 0.5) {
//		gridPoint gp = point(p1.x * (alpha / d) + p2.x * (1 - alpha / d),
//				p1.y * (alpha / d) + p2.y * (1 - alpha / d));
//	
//		if(gp.x >= 0 && gp.x < img.getWidth() && gp.y >= 0 && gp.y < img.getHeight()) {
//			img.setColor(gp.x, gp.y, targetColor);
//		}
//	}
//}

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

	if(World::instance()->getMode() == ESCAPE) {
		draw3D();
	} else {

	
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
					line(cSeg.a.translated(ori), cSeg.b.translated(ori), (*trace)[j].color, buffer[i]);
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


	//	this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
	}
		
	if(saveScreen) {
		saver.grabScreen(0, 0, ofGetWidth(), ofGetHeight() );
		string saveName;
		cin >> saveName;
		saver.saveImage(saveName);
		saveScreen = false;
	}
}

//--------------------------------------------------------------
void logoApp::keyPressed(int key){
	if(key == ' ') {
		cout << "Space pressed" << endl;
		Interpreter::instance()->toggleRunning();
	} else if(key == 'q') {
		cout << "\"q\" pressed" << endl;
		cout << "Saving texture..." << endl;
		img->saveImage("texture.jpg");
		ofExit(0); // exit entire program
	} else if(key == 's') {
		saveScreen = true;
		cout << "Enter file name" << endl;
	} else if(key == 'w') {
		cout << "w pressed" << endl;
		drawWireframe = !drawWireframe;
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

void logoApp::setup3D() {
	
	viewport2D.set(0, 0, (double)ofGetWindowWidth() / 2.0, ofGetWindowHeight());
	viewport3D.set((double)ofGetWindowWidth() / 2.0, 0, (double)ofGetWindowWidth() / 2.0, ofGetWindowHeight());
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(10, 10, 10);
	glEnable(GL_DEPTH_TEST);
	vector<ofVec3f> vertices;
	vector<ofVec2f> coords;
	vector<int> triangles;
	
//	img.loadImage("white.png");
	img = &(World::instance()->world3D->texture);
	vboMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	World::instance()->world3D->giveMesh(vertices, coords, triangles);

	for(int i = 0; i < (int)triangles.size(); i++) {
		vboMesh.addTexCoord(coords[i]);
		//cout << (i % 3 == 1) * 99 << " " << (i % 3 == 2) * 99 << endl;
		vboMesh.addVertex(vertices[triangles[i]]);
	}
	
	cout << "World mesh loaded" << endl;
	
	World::instance()->world3D->giveTurtleMesh(vertices, coords, triangles);
	for(int i = 0; i < (int)triangles.size(); i++) {
		turtleMesh.addVertex(vertices[triangles[i]]);
	}
	turtleMeshTrans = turtleMesh;
	
	cout << "Turtle mesh loaded" << endl;
	
//	orthoCam.setParent(orthoCamNode);
}

void logoApp::draw3D() {

//	ofRotateY(ofGetElapsedTimef() * 30); // slowly rotate the model
	float scaleRatio = min(viewport3D.width, viewport3D.height) * 0.6 * World::instance()->world3D->getScaleRatio();
//	cout << "scaleRatio = " << scaleRatio << endl; 
	cam.begin(viewport3D);
	ofScale(scaleRatio, scaleRatio, scaleRatio); // scale everything
//	img->update(); // Necessary??
    //img->bind(); // bind the image to begin texture mapping
	ofImage tex = *img;
//	if(World::instance()->world3D->cameraType == "FOLLOW") {
//		
//	}
//	World::instance()->world3D->giveTurtleCoords(pos, dir, dirUp);
	Turtle turtle = World::instance()->world3D->turtle;
	Face cFace = World::instance()->world3D->faces[turtle.faceId];
	pos = ofVec3f(turtle.pos) * cFace.rot + World::instance()->world3D->verts[cFace.v[0]];
	dir = ofVec3f(turtle.dir) * cFace.rot;
	dirUp = cFace.normal;
	updateTurtleMesh(pos, dir, dirUp);
	tex.bind();

	ofSetColor(ofColor::white);
	vboMesh.draw();
	if(drawWireframe) {
		ofSetColor(ofColor::black);
		vboMesh.drawWireframe();
	}

	//img->unbind();
	tex.unbind();
	ofSetColor(ofColor::black);
//	orthoCamNode.draw();
	turtleMeshTrans.drawWireframe();
	ofSetColor(ofColor::red);
	turtleMeshTrans.draw();
//	this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
	cam.end();
	ofLine(viewport2D.width, 0, viewport2D.width, viewport2D.height);
	drawVieport2D();
	ofSetColor(ofColor::white);
	
}

void logoApp::drawVieport2D() {
//	
//	ofViewport(viewport2D);
//	orthoCam.begin(viewport2D);
////	ofPushMatrix();
//	ofPushStyle();
//	float scaleRatio = min(viewport2D.width, viewport2D.height) * 0.6 * World::instance()->world3D->getScaleRatio();
////	ofTranslate(100.0, 100.0, 100.0);
////	ofScale(scaleRatio, scaleRatio, scaleRatio); // scale everything
////
//	cout << pos << " " << turtleMeshTrans.getVertex(0) << endl;
//	dirUp.normalize();
//	orthoCam.setGlobalPosition(turtleMeshTrans.getVertex(0) + dirUp * 100);
//
//	
//	orthoCam.lookAt(turtleMeshTrans.getVertex(0), dir);
////	orthoCam.setNearClip(0.0);
////	orthoCam.enableOrtho();
//
//
//	
//	ofImage tex = *img;
//	tex.bind();
//
//	ofSetColor(ofColor::white);
//	vboMesh.draw();
//	if(drawWireframe) {
//		ofSetColor(ofColor::black);
//		ofSetLineWidth(5);
//		vboMesh.drawWireframe();
//	}
//	tex.unbind();
//	ofSetColor(ofColor::black);
//	turtleMeshTrans.drawWireframe();
//	ofSetColor(ofColor::red);
//	turtleMeshTrans.draw();
//	orthoCam.end();
//   
//	ofPopStyle();
//	ofPopMatrix();
	
	ofPushStyle();
	ofPushView();
	ofViewport(viewport2D);

	ofSetupScreen();
	ofSetLineWidth(1);
	ofNoFill();
	ofSetColor(ofColor::red);
	ofVec2f center(viewport2D.width / 2.0, viewport2D.height / 2.0);
	ofTriangle(center, center + ofVec2f(10, 30), center + ofVec2f(-10, 30));
	ofFill();
	ofSetColor(ofColor::white);
	vector<vector<ofVec2f> > &triangles = World::instance()->world3D->orthoCast;
	for(int i = 0; i < (int)triangles.size(); i++) {
		ofTriangle(triangles[i][0], triangles[i][1], triangles[i][2]);
	}
//	ofNoFill();


	ofPopView();
	ofPopStyle();
	
}

void logoApp::update3D() {
	viewport2D.set(0, 0, (double)ofGetWindowWidth() / 2.0, ofGetWindowHeight());
	viewport3D.set((double)ofGetWindowWidth() / 2.0, 0, (double)ofGetWindowWidth() / 2.0, ofGetWindowHeight());
}

void logoApp::updateTurtleMesh(ofVec3f pos, ofVec3f dir, ofVec3f dirUp) {
	ofMatrix4x4 trans, rot;
	dir.normalize();
	dirUp.normalize();
	double angle;
	trans.makeTranslationMatrix(pos);
	rot.makeRotationMatrix(ofVec3f(0, 0, 1), dir);
	ofVec3f currentUp = ofVec3f(0, 1, 0) * rot;
	ofVec3f v1 = dir.getCrossed(currentUp), v2 = dir.getCrossed(dirUp);
	cout.precision(5);
	cout << v1 << " " << v2 << endl;
	cout << v1.getNormalized() << " " << -v2.getNormalized() << endl;
	
	if((v1.getNormalized() + v2.getNormalized()).length() < EPS) {
		angle = 180;
	} else if((v1.getNormalized() - v2.getNormalized()).length() < EPS) {
		angle = 0;
	} else {
		angle = v1.angle(v2);

		if(v1.getCrossed(v2).angle(dir) > EPS) {
			angle = -angle;
		}
	} 
	assert(!isnan(angle));
//	cout << "rot = " << rot << endl;
	rot *= ofMatrix4x4().newRotationMatrix(angle, dir);
	vector<ofVec3f> verts = turtleMesh.getVertices();
	for(int i = 0; i < (int)verts.size(); i++) {
//		cout << verts[i] << "-->";
//		verts[i] = verts[i] * rot * trans;
		verts[i].map(pos, dir.getCrossed(dirUp), dirUp, dir);
//		cout << verts[i] << endl;
	}
	cout << "angle = " << angle << endl;

//	orthoCamNode.setPosition(ofVec3f(0, 0, 0) * rot * trans);
	cout << " pos = " << pos << " dir = " << dir << " dirUp = " << dirUp << endl;
	turtleMeshTrans.getVertices() = verts;
}