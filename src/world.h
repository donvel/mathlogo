#pragma once

class World {
	// So far the world is just a rectangle	
	private:
		int width, height;
		World(); // World is a singleton
		static World *worldInstance;
	public:
		void setup(char*);
		int getWidth();
		int getHeight();
		static World* instance();
};
