#include <fstream>
#include <iostream>
#include <cstdlib>

#include "world.h"

using namespace std;

World* World::worldInstance = NULL;

World::World() : width(300), height(300) {}

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
	setupFile.close();
}

int World::getWidth() {
	return width;
}

int World::getHeight() {
	return height;
}

