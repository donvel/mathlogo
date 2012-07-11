#pragma once

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "trans.h"
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
		int activeTurtle;
		
		Turtle turtle[2];
		vector<segment> trace[2];  
		transformation trans[2];
		gridPoint origin[2]; // Origin of the coordinate system - its location on the viewport 
		voxel ***map;

	public:
		void setup(char* filename);
		void createMap(); 
	
		int getWidth();
		int getHeight();
		int getDepth();
		int getFrameTime();
		Mode getMode();
		vector<segment> getTrace(int id);
		
/// Those functions may be unnecessary, since world does not have to be a rectangle
		int getLeft();
		int getRight();
		int getTop();
		int getBottom();
/// -------------------------------------------------------------------------------|
	
		gridPoint getOrigin(int id);
//		voxel* getVoxel(gridPoint gp);

		/// Graphics handling
		
		ofColor getBackgroundColor();
		vector<point> getTurtleShape(int id);
		bool crop(segment &seg);

		static World* instance();

		// Turtle handling
		void updateTurtle(int id, pair<point, vect> coords);
		void rotate(long double angle);
		void forward(long double distance);
		void toggleTurtle();
		void penUp();
		void penDown();
		void setPenColor(int hex);
		void clear();
		
		
		// Transform handling
		void setMobius(comp a, comp b, comp c, comp d);

};
