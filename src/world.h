#pragma once

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <boost/thread.hpp>

#include "trans.h"
#include "turtle.h"
#include "geometry.h"
#include "data3d.h"
#include "ofMain.h"

using namespace std;
using namespace boost;

enum Mode {NORMAL, TRANSFORM, ESCAPE}; // Mode is specified in the world.conf file

typedef pair<point, ofColor> Filler;

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
		Mode mode;
		int activeTurtle;
		bool useVoxels;
		
		// We need two turtles for the TRANSFORM mode
		Turtle turtle[2];
		vector<segment> trace[2];  
		vector<Filler > filler[2];
		transformation trans[2];
		gridPoint origin[2]; // Origin of the coordinate system - its location on the viewport 
		voxel ***voxelMap[2];
		string circleStyle;

	public:
		map<int, int> revPalette;
		vector<ofColor> palette;
		static World* instance();
		bool changing;
		bool frozen;
		
		void setup(char* filename);
		void createMap(); 
	
		int getWidth();
		int getHeight();
		int getDepth();
		int getFrameTime();
		int numViewports();
		Mode getMode();
		bool outside(gridPoint p);
		point getTurtlePosition();
		double getTurtleAngle();
		
/// World boundaries (we assume that it is a rectangle)
		int getLeft();
		int getRight();
		int getTop();
		int getBottom();
/// -------------------------------------------------------------------------------|
	


		/// Graphics handling

		bool cleared[2];
		gridPoint getOrigin(int id);
		voxel* getVoxel(gridPoint gp, int id);		
		ofColor getBackgroundColor();
		vector<point> getTurtleShape(int id);
		bool crop(segment &seg);
		vector<segment>* getTrace(int id);
		vector<Filler >* getFiller(int id);
		void fill();
		void fillOne(int id);
		void addCircle(point pos, double r);

		// Turtle handling
		void updateTurtle(int id, pair<point, vect> coords);
		void moveTurtleTo(point p);
		void headTurtleTo(point p);
		void rotate(double angle);
		void forward(double distance);
		void toggleTurtle();
		void penUp();
		void penDown();
		void setPenColor(int colId);
		void clearScreen();
		
		// Transform handling
		vector<comp> addTransform(comp a, comp b, comp c, comp d, bool mirror = false, double y = 0); // comp is a typedef complex<long double, long double>
		void clearTransform();
		
		void debug();
		
		// 3D
		
		Data3D *world3D;

};
