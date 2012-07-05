#include "world.h"

using namespace std;

World* World::worldInstance = NULL;

World::World() : width(300), height(300), depth(1),
	frameTime(10), backgroundColor(255, 255, 255), origin(150, 150)  {
	
	map = new voxel**[width];
	for(int i = 0; i < width; i++) {
		map[i] = new voxel*[height];	
		for(int j = 0; j < height; j++) {
			map[i][j] = new voxel[depth];
		}
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
	setupFile >> width >> height >> depth;
	setupFile >> frameTime;
		
	setupFile.close();
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

gridPoint World::getOrigin() {
	return origin;
}

ofColor World::getBackgroundColor() {
	return backgroundColor;
}

voxel* World::getVoxel(gridPoint gp) {
	return &map[gp.x + origin.x][gp.y + origin.y][gp.z + origin.z];
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

void World::rotate(long double angle) {// in degrees
	turtle.direction = turtle.direction.rotated(angle);	
}


void World::forward(long double distance) {

	vect displacement = turtle.direction * distance;
	point newPosition = turtle.position.translated(displacement);
    
//    cout << "Turtle position: " << newPosition.x << " " << newPosition.y << endl;
    
	if(outside(gridPoint(newPosition))) {
		return;
		// This can be done in a better way better, e. g. by finding the intersection of the (position, newPosition) segment with the world borders
		// The problem is that I don't know what those borders shall eventually look like (it may be even a 3D mesh), so for now I leave this solution	
	}
	
	point currentPosition = turtle.position; 
	while(dist(turtle.position, currentPosition) <= // Note: dist is a function, while distance is a parameter
		   	dist(turtle.position, newPosition)) {
		currentPosition = currentPosition.translated(turtle.direction);
		gridPoint visitedVoxel = currentPosition; 
		if(!outside(visitedVoxel)) getVoxel(visitedVoxel)->visit(turtle.isPenDown, turtle.penColor);
	}	

	turtle.position = newPosition;
}
