#include "world.h"
#include "data3d.h"

World* World::worldInstance = NULL;

World::World() : width(300), height(300), depth(1),
	frameTime(10), backgroundColor(255, 255, 255), 
	paletteSize(27), mode(NORMAL), activeTurtle(0), 
	useVoxels(false), changing(false), frozen(false), world3D(NULL) {
	origin[0] = gridPoint(150, 150, 0);
	origin[1] = gridPoint(450, 150, 0);
	voxelMap[0] = NULL;
	voxelMap[1] = NULL;
	cleared[0] = true;
	cleared[1] = true;
	
	palette.resize(paletteSize);
	for(int i = 0; i < paletteSize; i++) {
		ofColor col = ofColor((i / 9) * 127, ((i / 3) % 3) * 127, (i % 3) * 127);// sets different RGB values
		palette[i] = col; 
		revPalette[col.getHex()] = i;
//		cout << "color " << (i / 9) * 127 << " " << ((i / 3) % 3) * 127 << " " << (i % 3) * 127 << endl;
	}
}

void World::createMap() { // not in use

	
	for(int tur = 0; tur < numViewports(); tur++) {
		voxelMap[tur] = new voxel**[width];
		for(int i = 0; i < width; i++) {
			voxelMap[tur][i] = new voxel*[height];	
			for(int j = 0; j < height; j++) {
				voxelMap[tur][i][j] = new voxel[depth];
			}
		}
	}
}

World::~World() {
	
	for(int tur = 0; tur < numViewports(); tur++) {
		if(voxelMap[tur] != NULL) {
			for(int i = 0; i < width; i++) {
				for(int j = 0; j < height; j++) {
					delete[] voxelMap[tur][i][j];
				}
				delete[] voxelMap[tur][i];
			}
			delete[] voxelMap[tur];
		}
	}
	if(world3D != NULL) {
		delete world3D;
	}
}

World* World::instance() {
	if(worldInstance == NULL) {
		worldInstance = new World();
	}
	return worldInstance;
}

void World::setup(char *filename) {
	fstream setupFile;
	setupFile.open(filename, fstream::in);
	if(!setupFile.good()) {
		cout << "Corrupted config file." << endl;
		exit(1);
	}
	
	string tmpString;
	
	setupFile >> tmpString;
	if(tmpString == "TRANSFORM") {
		mode = TRANSFORM;
	} else if (tmpString == "ESCAPE") {
		mode = ESCAPE;
	} else {
		mode = NORMAL;
	}
	
	setupFile >> width >> height;
	cout << "World dimensions: " << width << " " << height << " " << depth << " " << endl;
	if(mode != ESCAPE) {
		setupFile >> origin[0].x >> origin[0].y;
		origin[1] = origin[0];
		origin[1].x += width;
		setupFile >> frameTime;
		if(mode == TRANSFORM) {
			setupFile >> circleStyle;
//			drawCircles = boolValue == "true";
		}
	//	cout << origin.x << " " << origin.y << " " << origin.z;
		cout << "Frame time: " << frameTime << endl;
	} else { 
		setupFile >> frameTime;
		world3D = new Data3D(turtle[0]);
		world3D->setup(&setupFile);
	}
	setupFile >> tmpString;
	if(tmpString == "true") {
		recordVideo = true;
	} else {
		recordVideo = false;
	}
	setupFile.close();
	cout << "World setup complete" << endl;
}

int World::getWidth() {
	return width;
}

int World::getHeight() {
	return height;
}

int World::getDepth() {
	return depth;
}

int World::getFrameTime() {
	return frameTime;
}

Mode World::getMode() {
	return mode;
}

gridPoint World::getOrigin(int id) {
	return origin[id];
}

ofColor World::getBackgroundColor() {
	return backgroundColor;
}

voxel* World::getVoxel(gridPoint gp, int id) {
	return &voxelMap[id][gp.x + origin[0].x][gp.y + origin[0].y][gp.z + origin[0].z];
}

vector<segment>* World::getTrace(int id) {
	return &trace[id];
}

vector<Filler>* World::getFiller(int id) {
	return &filler[id];
}


int World::getLeft() {
	return -origin[0].x;
}

int World::getRight() {
	return -origin[0].x + width - 1;
}

int World::getBottom() {
	return -origin[0].y;
}

int World::getTop() {
	return -origin[0].y + height - 1;
}

point World::getTurtlePosition() {
	return turtle[activeTurtle].position;
}

double World::getTurtleAngle() {
	return angle(turtle[activeTurtle].direction, vect(0, 1)) / (2 * M_PI) * 360;
}

bool World::outside(gridPoint p) {
	return p.y < getBottom() || p.y >= getTop() || p.x < getLeft() || p.x >= getRight();
}

bool World::crop(segment& seg) { // this function returns ture when part of the segment "seg"  is in the viewport
	//and then crops it to this part
	if(outside(seg.a) && outside(seg.b)) return false; // segment completely outside
	//This is not always the case, but seems to be a reasonable approximation
	if(!outside(seg.a) && !outside(seg.b)) return true; // segment completely in viewport (always true)
	if(!outside(seg.a)) {
		point tmp = seg.a;
		seg.a = seg.b;
		seg.b = tmp;
	} // now point a is outside the viewport and b is inside
	
	point corners[4]; // vertices of the viewport rectangle
	corners[0] = point(getTop(), getRight());
	corners[1] = point(getBottom(), getRight());
	corners[2] = point(getBottom(), getLeft());
	corners[3] = point(getTop(), getLeft());
	
	for(int i = 0; i < 4; i++) {
		point tmp;
		if (intersect(seg, segment(corners[i], corners[(i + 1) % 4]), tmp)) {
			seg.a = tmp; // crop the segment
		}
	}
	return true;
}

//---- TURTLE handling ------------------------------//

void World::updateTurtle(int id, pair<point, vect> coords) {
	this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
	if(turtle[id].isPenDown && !(coords.first == turtle[id].position)) {// if the turtle moved
		//while(frozen);
		changing = true;
		trace[id].push_back(segment(turtle[id].position, coords.first, turtle[id].penColor)); // add new segment
		changing = false;
	}
	turtle[id].position = coords.first;
	turtle[id].direction = coords.second;
}

int World::numViewports() {
	if (getMode() == TRANSFORM) return 2;
	return 1;
}

void World::fill() {
	for(int tur = 0; tur < numViewports(); tur++) {
		fillOne(tur);
	}
}

void World::fillOne(int id) {
	filler[id].push_back(make_pair(turtle[id].position, turtle[id].penColor));
}

void World::addCircle(point pos, double r) {
	if(circleStyle == "false") return;
	if(circleStyle == "full") {
		for(int i = 0; i < 360; i++) {
			for(int j = 0; j < 2; j++) {
				trace[j].push_back(segment(pos.translated(vect(r, 0).rotated(i-1)), pos.translated(vect(r, 0).rotated(i)), ofColor().red));
			}
		}
	} else {
		for(int i = 0; i < 60; i++) {
			for(int j = 0; j < 2; j++) {
				trace[j].push_back(segment(pos.translated(vect(r, 0).rotated(6 * i-1)), pos.translated(vect(r, 0).rotated(6 * i)), ofColor().red));
			}
		}
	}
	cout << "Adding circle " << pos.x << " " << pos.y << " " << r << endl;
}

void World::toggleTurtle() {
	activeTurtle = !activeTurtle; // 0 = !1, 1 = !0
}

void World::clearTransform() { // restores the turtle to its initial state and position
	for(int i = 0; i < 2; i++) {
		trans[i] = transformation();
		trace[i].clear();
		filler[i].clear();
		turtle[i] = Turtle();
		cleared[i] = true;
	}
}

void World::clearScreen() { // restores the turtle to its initial state and position
	
	trace[activeTurtle].clear();
	filler[activeTurtle].clear();
	cleared[activeTurtle] = true;

}

void World::penDown() {// affects only one turtle
	turtle[activeTurtle].isPenDown= true;
}

void World::penUp() {
	turtle[activeTurtle].isPenDown= false;
}

void World::setPenColor(int colId) {
	if(colId < 0 || colId >= paletteSize) {
		cout << "Colors should be in range [0, " << paletteSize << endl;
		return;
	}
	turtle[activeTurtle].penColor = palette[colId];
}

void World::moveTurtleTo(point p) {// Does not work as expected.

	if(outside(p)) return;
	
	vect oldDirection[2];
	for(int i = 0; i < 2; i++) {
		oldDirection[i] = turtle[activeTurtle].direction;
	}
	rotate(angle(turtle[activeTurtle].direction, vect(p) + (-vect(turtle[activeTurtle].position))) * 360 / (2 * M_PI));
	forward(dist(turtle[activeTurtle].position, p));
	for(int i = 0; i < 2; i++) {
		turtle[activeTurtle].direction = oldDirection[i];
	}
	
}

void World::headTurtleTo(point p) {// Does not work as expected.

	cout << "Heading turtle to " << p.x << " " << p.y << endl;
	rotate(angle(turtle[activeTurtle].direction, vect(p) + (-vect(turtle[activeTurtle].position))) * 360 / (2 * M_PI));
	if(getMode() == TRANSFORM) {
		turtle[!activeTurtle].direction = trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)).second;
	}
}


vector<comp> World::addTransform(comp a, comp b, comp c, comp d, bool mirror, double y) {

	vector<comp> fp;	
	
	if(outside(turtle[activeTurtle].position)) {
		cout << "Active turtle outside viewport" << endl;
		return fp;
	}
	if(mirror) {
		trans[activeTurtle].addMirror(y); //f
		trans[!activeTurtle].addMirror(y); //f^-1
	} else { // Moebius
		if(a * d - b * c == comp(0, 0)) {
			cout << "a * d - b * c == 0" << endl;
			return fp;
		}
		cout << a << " " << b << " " << c << " " << d << endl;
//		ofExit(0);
		fp = trans[activeTurtle].addMoebius(a, b, c, d); //f
		trans[!activeTurtle].addMoebius(-d, b, c, -a); //f^-1
	}
	trace[!activeTurtle].clear();
	filler[!activeTurtle].clear();
	cleared[!activeTurtle] = true;
	for(vector<segment>::iterator it = trace[activeTurtle].begin(); it < trace[activeTurtle].end(); it++) {
		trace[!activeTurtle].push_back(segment(trans[activeTurtle].setPos(it->a), 
				trans[activeTurtle].setPos(it->b), it->color));
	}
	for(vector<Filler>::iterator it = filler[activeTurtle].begin(); it < filler[activeTurtle].end(); it++) {
		filler[!activeTurtle].push_back(make_pair(trans[activeTurtle].setPos(it->first), it->second));
	}
	// we transform active turtle's trace to the other viewport
	//I still wonder what should be done in this case - it is  not the only option.
	pair<point, vect> coords = trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction));
	turtle[!activeTurtle].position = coords.first;
	turtle[!activeTurtle].direction = coords.second;
	
	return fp;
}

vector<point> World::getTurtleShape(int id) {
	vector<point> res(3);
	res[0] = turtle[id].position.translated(turtle[id].direction * 60.0);
	res[1] = turtle[id].position.translated(turtle[id].direction.rotated(90.0) * 20.0);
	res[2] = turtle[id].position.translated(turtle[id].direction.rotated(-90.0) * 20.0);	
	//the points are given in 'Turtle' coordinates, not OF coordinates

	return res;
}

void World::debug() {
	
	cout << turtle[activeTurtle].position.x + origin[activeTurtle].x << " " << turtle[activeTurtle].position.y + origin[activeTurtle].y << endl; 
}

void World::forward(double distance) {
	if(mode == ESCAPE) {
		world3D->forward(distance);
		return;
	}
	vect displacement = turtle[activeTurtle].direction * distance;
	point newPosition = turtle[activeTurtle].position.translated(displacement); 
	// active turtle's position after this move
    point newPosition2 = trans[activeTurtle].setCoords(make_pair(newPosition, vect())).first;
	// another turtle's position after this move
    
	if(outside(newPosition) && outside(newPosition2)) { // We want at least one turtle to stay in the viewport
		return;
	}
	
	point currentPosition = turtle[activeTurtle].position; 
	point begPosition = currentPosition;
	
	while(dist(begPosition, currentPosition) <= // Note: dist is a function, while distance is a parameter
		   	dist(begPosition, newPosition)) {
		// Turtle makes many 1px movements to get to the target
		currentPosition = currentPosition.translated(turtle[activeTurtle].direction * (distance > 0 ? 1 : -1));
		if(useVoxels) {// We draw trace with segments, not with voxels
			gridPoint visitedVoxel = currentPosition; // not used now
			if(!outside(visitedVoxel)) getVoxel(visitedVoxel, activeTurtle)->visit(false); 
		}
		
		updateTurtle(activeTurtle, make_pair(currentPosition, turtle[activeTurtle].direction));
		if(mode == TRANSFORM) updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));
	}	
	// We move the turtle to its final position to finish the movement
	updateTurtle(activeTurtle, make_pair(newPosition, turtle[activeTurtle].direction));
	if(mode == TRANSFORM) updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));
}



void World::rotate(double angle) {// in degrees
	if(mode == ESCAPE) {
		world3D->rotate(angle);
		return;
	}
	updateTurtle(activeTurtle, make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction.rotated(angle)));	
	if(mode == TRANSFORM) { // we have to update also linked turtle's rotation
		updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));	
	}
}