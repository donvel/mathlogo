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
	}
	World::instance()->createMap();
	//otherwise world default values are used.
	
	//if the number of arguments == 2 , then argv[2] = name of the script file
	if(argnum >= 3) {
		Interpreter::instance()->loadScript(argv[2]);
		if(argnum >= 4) {
			cout << "Expected at most 2 arguments." << endl;
		}
	} 
	//otherwise let turtle move in a random way
	

	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	int width = World::instance()->getWidth();
	int height = World::instance()->getHeight();
	if(World::instance()->getMode() == TRANSFORM) {
		width *= 2;
	}
	cout << width << " " << height << endl;
	ofSetupOpenGL(&window, width, height, OF_WINDOW);

	Interpreter::instance()->execute(); // start interpreter
	ofRunApp(new logoApp()); // start the app
//	thread uiThread(ofRunApp, new logoApp()); // start the app	
}
