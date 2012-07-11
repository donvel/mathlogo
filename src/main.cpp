#include <iostream>

#include "ui.h"
#include "interpreter.h"
#include "world.h"
#include "ofAppGlutWindow.h"

using namespace std;


//--------------------------------------------------------------
int main(int argnum, char **argv) { 
	
	//if the number of arguments >= 1 , then argv[1] = name of the config file
	if(argnum >= 2) { // note that arg[0] is always passed, so argnum >= 1
		World::instance()->setup(argv[1]);
	} //otherwise world default values are used
	
	//World::instance()->createMap(); Creating voxels that may be marked as
	//visited by the turtle may be useful int the future, but not know
	
	
	//if the number of arguments == 2 , then argv[2] = name of the script file
	if(argnum >= 3) {
		Interpreter::instance()->loadScript(argv[2]);
		if(argnum >= 4) {
			cout << "Expected at most 2 arguments." << endl;
		}
	} 
	//otherwise let turtle move in the random way
	//TODO otherwise let turtle accept input from the console
	

	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	int width = World::instance()->getWidth();
	int height = World::instance()->getHeight();
	if(World::instance()->getMode() == TRANSFORM) {
		width *= 2; // In this mode we have two viewports, so overall width is doubled
	}
	cout << "Window width = " << width << ", height = " << height << endl;

	ofSetupOpenGL(&window, width, height, OF_WINDOW);

	Interpreter::instance()->execute(); // start interpreter (in separate thread)
	ofRunApp(new logoApp()); // start the app
}
