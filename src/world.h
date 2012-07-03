#pragma once

#include <fstream>
#include <iostream>
#include <cstdlib>

#include "turtle.h"
#include "geometry.h"
#include "ofMain.h"

class World {
	// So far the world is just a rectangle	
	private:
		World(); // World is a singleton
		static World *worldInstance;
		
		int width, height, depth;
		ofColor backgroundColor;
		int frameTime;
		
		Turtle turtle;
		gridPoint origin; // Origin of the coordinate system - its location on the viewport 
		voxel ***map;

	public:
		void setup(char* filename);
		int getWidth();
		int getHeight();
		int getDepth();
		int getFrameTime();
		gridPoint getOrigin();
		voxel* getVoxel(gridPoint gp);
		bool outside(gridPoint gp);
/// Those functions may be unnecessary, since world does not have to be a rectangle
		int getLeft();
		int getRight();
		int getTop();
		int getBottom();
/// -------------------------------------------------------------------------------|
		ofColor getBackgroundColor();
		static World* instance();

		// Turtle handling
		void rotate(long double angle);
		void forward(long double distance);

};
