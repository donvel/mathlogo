#include <fstream>
#include <iostream>
#include <cstdlib>

#include "world.h"

using namespace std;

World* World::worldInstance = NULL;

World::World() : width(300), height(300),
	frameTime(20), turtle(), origin(150, 150) {}

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
	setupFile >> width >> height;
	setupFile >> frameTime;
		
	setupFile.close();
}

int World::getWidth() {
	return width;
}

int World::getHeight() {
	return height;
}

int World::getFrameTime() {
	return frameTime;
}
