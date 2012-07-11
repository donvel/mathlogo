#pragma once

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "trans.h"
#include "turtle.h"
#include "geometry.h"
#include "ofMain.h"

enum Mode {NORMAL, TRANSFORM, ESCAPE}; // Mode is specified in the world.conf file

class World {
	// So far the world is just a rectangle	
	private:
		World(); // World is a singleton
		~World();
		static World *worldInstance;
		
		int width, height, depth; // the third value won't probably be used
		int frameTime;
		ofColor backgroundColor;
		int paletteSize;
		vector<ofColor> palette;
		Mode mode;
		int activeTurtle;
		bool useVoxels;
		
		// We need two turtles for the TRANSFORM mode
		Turtle turtle[2];
		vector<segment> trace[2];  
		transformation trans[2];
		gridPoint origin[2]; // Origin of the coordinate system - its location on the viewport 
		voxel ***map; // not needed so far
		
		bool outside(gridPoint p);

	public:
		static World* instance();
		
		void setup(char* filename);
		void createMap(); // not needed so far
	
		int getWidth();
		int getHeight();
		int getDepth();
		int getFrameTime();
		Mode getMode();
		
/// World boundaries (we assume htat it is a rectangle)
		int getLeft();
		int getRight();
		int getTop();
		int getBottom();
/// -------------------------------------------------------------------------------|
	


		/// Graphics handling

		gridPoint getOrigin(int id);
		voxel* getVoxel(gridPoint gp);		
		ofColor getBackgroundColor();
		vector<point> getTurtleShape(int id);
		bool crop(segment &seg);
		vector<segment> getTrace(int id);

		// Turtle handling
		void updateTurtle(int id, pair<point, vect> coords);
		void rotate(long double angle);
		void forward(long double distance);
		void toggleTurtle();
		void penUp();
		void penDown();
		void setPenColor(int colId);
		void clear();
		
		// Transform handling
		void setMobius(comp a, comp b, comp c, comp d); // comp is a typedef complex<long double, long double>

};
