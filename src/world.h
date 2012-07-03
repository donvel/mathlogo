#pragma once
#include "turtle.h"

class World {
	// So far the world is just a rectangle	
	private:
		int width, height;
		int frameTime;
		World(); // World is a singleton
		static World *worldInstance;
	public:
		void setup(char*);
		int getWidth();
		int getHeight();
		int getFrameTime();
		static World* instance();
};
