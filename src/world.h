#pragma once
#include "turtle.h"
#include "geometry.h"

class World {
	// So far the world is just a rectangle	
	private:
		int width, height;
		int frameTime;
		World(); // World is a singleton
		static World *worldInstance;
		Turtle turtle;
		point origin; // Origin of the coordinate system - its location on the viewport 
	public:
		void setup(char*);
		int getWidth();
		int getHeight();
		int getFrameTime();
		static World* instance();
};
