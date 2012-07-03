#include "world.h"

using namespace std;

World* World::worldInstance = NULL;

World::World() : width(300), height(300), depth(1),
	frameTime(100), backgroundColor(255, 255, 255), origin(150, 150)  {
	
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

ofColor World::getBackgroundColor() {
	return backgroundColor;
}

int getLeft() {
	return -origin.x;
}

int getRight() {
	return -origin.x + width;
}

int getBottom() {
	return -origin.y;
}

int getTop() {
	return -origin.y + height;
}

bool World::outside(point p) {
	return p.y < getBottom() || p.y > getTop() || p.x < getLeft() || p.x > getRight();
}

void World::rotate(long double angle) {// in degrees
	
}


void World::forward(long double distance) {
	vect displacement = turtle.direction * distance;
	point newPosition = turtle.position.translated(displacement);
	if(!outside(newPosition)) {
		
	}
	turtle.position = newPosition;
}
