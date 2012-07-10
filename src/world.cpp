#include "world.h"
#include "transformation.h"

using namespace std;

World* World::worldInstance = NULL;

World::World() : width(300), height(300), depth(1),
	frameTime(10), backgroundColor(255, 255, 255), origin(150, 150)  {
}

void World::createMap() {
	map = new voxel**[width];
	for(int i = 0; i < width; i++) {
		map[i] = new voxel*[height];	
		for(int j = 0; j < height; j++) {
			map[i][j] = new voxel[depth];
		}
	}
}

World::~World() {
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
			delete[] map[i][j];
		}
		delete[] map[i];
	}
	delete[] map;
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
	setupFile >> origin.x >> origin.y >> origin.z;
	setupFile >> frameTime;
	cout << width << " " << height << " " << depth << " " << endl;
	cout << origin.x << " " << origin.y << " " << origin.z;
	cout << frameTime << endl;
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


gridPoint World::getOrigin() {
	return origin;
}

ofColor World::getBackgroundColor() {
	return backgroundColor;
}

voxel* World::getVoxel(gridPoint gp) {
	return &map[gp.x + origin.x][gp.y + origin.y][gp.z + origin.z];
}

vector<segment> getTrace(int id) {
	return trace[id];
}

int World::getLeft() {
	return -origin.x;
}

int World::getRight() {
	return -origin.x + width;
}

int World::getBottom() {
	return -origin.y;
}

int World::getTop() {
	return -origin.y + height;
}

bool World::outside(gridPoint p) {
	return p.y < getBottom() || p.y >= getTop() || p.x < getLeft() || p.x >= getRight();
}

void World::updateTurtle(int id, pair<point, vect> coords) {
	if(turtle[id].isPenDown) {
		trace[id].push_back(segment(turtle[id].position, coords.first, turtle[id].penColor));
	}
	turtle[id].position = coords.first;
	turtle[id].direction = coords.second;
}

void World::rotate(long double angle) {// in degrees
	updateTurtle(activeTurtle, make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction.rotated(angle)));	
	if(mode == TRANSFORM) {
		updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));	
	}
}


void World::forward(long double distance) {

	vect displacement = turtle[activeTurtle].direction * distance;
	point newPosition = turtle[activeTurtle].position.translated(displacement);
    point newPosition2 = trans[activeTurtle].setCoords(make_pair(newPosition, vect())).first;
//    cout << "Turtle position: " << newPosition.x << " " << newPosition.y << endl;
    
	if(outside(gridPoint(newPosition)) || outside(newPosition2)) {
		return;
		// This can be done in a better way better, e. g. by finding the intersection of the (position, newPosition) segment with the world borders
		// The problem is that I don't know what those borders shall eventually look like (it may be even a 3D mesh), so for now I leave this solution	
	}
	
	point currentPosition = turtle[activeTurtle].position; 
	while(dist(turtle[activeTurtle].position, currentPosition) <= // Note: dist is a function, while distance is a parameter
		   	dist(turtle[activeTurtle].position, newPosition)) {
		currentPosition = currentPosition.translated(turtle[activeTurtle].direction);
//		gridPoint visitedVoxel = currentPosition; 
//		if(!outside(visitedVoxel)) getVoxel(visitedVoxel)->visit(false); // We draw trace with segments, not with voxels
		updateTurtle(activeTurtle, make_pair(currentPosition, turtle[activeTurtle].direction));
		if(mode == TRANSFORM) updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));
	}	

	updateTurtle(activeTurtle, make_pair(currentPosition, turtle[activeTurtle].direction));
	if(mode == TRANSFORM) updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));
}

vector<point> World::getTurtleShape(int id) {
	vector<point> res(3);
	res[0] = turtle[id].position.translated(turtle[id].direction * 30.0);
	res[1] = turtle[id].position.translated(turtle[id].direction.rotated(90.0) * 10.0);
	res[2] = turtle[id].position.translated(turtle[id].direction.rotated(-90.0) * 10.0);	
	for (int i = 0; i < 3; i++) {
		res[i] = res[i].translated((vect)(point)origin);
	}

	return res;
}
