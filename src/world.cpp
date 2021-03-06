#include "world.h"


using namespace std;

World* World::worldInstance = NULL;

World::World() : width(300), height(300), depth(1),
	frameTime(10), backgroundColor(255, 255, 255), 
	paletteSize(27), mode(NORMAL), activeTurtle(0), 
	useVoxels(false), map(NULL) {
	origin[0] = gridPoint(150, 150, 0);
	origin[1] = gridPoint(450, 150, 0);
	
	palette.resize(paletteSize);
	for(int i = 0; i < paletteSize; i++) {
		palette[i] = ofColor((i / 9) * 128, ((i % 9) / 3) * 128, (i % 3) * 128); // sets different RGB values
	}
}

void World::createMap() { // not in use
	map = new voxel**[width];
	for(int i = 0; i < width; i++) {
		map[i] = new voxel*[height];	
		for(int j = 0; j < height; j++) {
			map[i][j] = new voxel[depth];
		}
	}
}

World::~World() { // not in use
	if(map != NULL) {
		for(int i = 0; i < width; i++) {
			for(int j = 0; j < height; j++) {
				delete[] map[i][j];
			}
			delete[] map[i];
		}
		delete[] map;
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
	
	setupFile >> width >> height >> depth;
	setupFile >> origin[0].x >> origin[0].y >> origin[0].z;
	origin[1] = origin[0];
	origin[1].x += width;
	setupFile >> frameTime;
	cout << "World dimensions: " << width << " " << height << " " << depth << " " << endl;
//	cout << origin.x << " " << origin.y << " " << origin.z;
	cout << "Frame time: " << frameTime << endl;
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

voxel* World::getVoxel(gridPoint gp) { // not in use
	return &map[gp.x + origin[0].x][gp.y + origin[0].y][gp.z + origin[0].z];
}

vector<segment> World::getTrace(int id) {
	return trace[id];
}

int World::getLeft() {
	return -origin[0].x;
}

int World::getRight() {
	return -origin[0].x + width;
}

int World::getBottom() {
	return -origin[0].y;
}

int World::getTop() {
	return -origin[0].y + height;
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
	if(turtle[id].isPenDown && !(coords.first == turtle[id].position)) {// if the turtle moved
		trace[id].push_back(segment(turtle[id].position, coords.first, turtle[id].penColor)); // add new segment
	}
	turtle[id].position = coords.first;
	turtle[id].direction = coords.second;
}

void World::toggleTurtle() {
	activeTurtle = !activeTurtle; // 0 = !1, 1 = !0
}

void World::clear() { // restores the turtle to its initial state and position
	for(int i = 0; i < 2; i++) {
		trans[i] = transformation();
		trace[i].clear();
		turtle[i] = Turtle();
	}
}

void World::penDown() {// affects only one turtle
	turtle[activeTurtle].isPenDown= true;
}

void World::penUp() {
	turtle[activeTurtle].isPenDown= false;
}

void World::setPenColor(int colId) {
	if(colId < 0 || colId >= paletteSize) {
		cout << "Colors shoud be in range [0, " << paletteSize << endl;
		return;
	}
	turtle[activeTurtle].penColor = palette[colId];
}

void World::rotate(long double angle) {// in degrees
	updateTurtle(activeTurtle, make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction.rotated(angle)));	
	if(mode == TRANSFORM) { // we have to update also linked turtle's rotation
		updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));	
	}
}

void World::forward(long double distance) {

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
		currentPosition = currentPosition.translated(turtle[activeTurtle].direction);
		if(useVoxels) {// We draw trace with segments, not with voxels
			gridPoint visitedVoxel = currentPosition; // not used now
			if(!outside(visitedVoxel)) getVoxel(visitedVoxel)->visit(false); 
		}
		
		updateTurtle(activeTurtle, make_pair(currentPosition, turtle[activeTurtle].direction));
		if(mode == TRANSFORM) updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));
	}	
	// We move the turtle to its final position to finish the movement
	updateTurtle(activeTurtle, make_pair(currentPosition, turtle[activeTurtle].direction));
	if(mode == TRANSFORM) updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));
}

vector<point> World::getTurtleShape(int id) {
	vector<point> res(3);
	res[0] = turtle[id].position.translated(turtle[id].direction * 30.0);
	res[1] = turtle[id].position.translated(turtle[id].direction.rotated(90.0) * 10.0);
	res[2] = turtle[id].position.translated(turtle[id].direction.rotated(-90.0) * 10.0);	
	//the points are given in 'Turtle' coordinates, not OF coordinates

	return res;
}

void World::setMobius(comp a, comp b, comp c, comp d) {
	if(a * d - b * c == comp(0, 0)) {
		cout << "a * d - b * c == 0" << endl;
		return;
	}
	if(outside(turtle[activeTurtle].position)) {
		cout << "Active turtle outside viewport" << endl;
		return;
	}
	trans[activeTurtle].setValues(a, b, c, d); //f
	trans[!activeTurtle].setValues(-d, b, c, -a); //f^-1
	trace[!activeTurtle].clear();
	for(vector<segment>::iterator it = trace[activeTurtle].begin(); it < trace[activeTurtle].end(); it++) {
		trace[!activeTurtle].push_back(segment(trans[activeTurtle].setPos(it->a), 
				trans[activeTurtle].setPos(it->b), it->color));
	} // we transform active turtle's trace to the other viewport
	//I still wonder what should be done in this case - it is  not the only option.
}

