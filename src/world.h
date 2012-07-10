#pragma once

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "turtle.h"
#include "geometry.h"
#include "ofMain.h"

enum Mode {NORMAL, TRANSFORM, ESCAPE};

class World {
	// So far the world is just a rectangle	
	private:
		World(); // World is a singleton
		~World();
		static World *worldInstance;
		bool outside(gridPoint gp);
		
		int width, height, depth;
		int frameTime;
		ofColor backgroundColor;
		Mode mode;
		
		Turtle turtle;
		gridPoint origin; // Origin of the coordinate system - its location on the viewport 
		voxel ***map;

	public:
		void setup(char* filename);
		void createMap(); 
	
		int getWidth();
		int getHeight();
		int getDepth();
		int getFrameTime();
		Mode getMode();

		
/// Those functions may be unnecessary, since world does not have to be a rectangle
		int getLeft();
		int getRight();
		int getTop();
		int getBottom();
/// -------------------------------------------------------------------------------|
	
		gridPoint getOrigin();
		voxel* getVoxel(gridPoint gp);

		ofColor getBackgroundColor();
		vector<point> getTurtleShape();

		static World* instance();

		// Turtle handling
		void rotate(long double angle);
		void forward(long double distance);

};
