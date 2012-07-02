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
		
		//if the number of arguments == 2 , then argv[2] = name of the script file
		if(argnum >= 3) {
			Interpreter::instance()->execute(argv[2]);
			if(argnum >= 4) {
				cout << "Expected at most 2 arguments." << endl;
			}
		} else {
			Interpreter::instance()->randomMoves();
		}
		//otherwise let turtle move in a random way
	}
	//otherwise world default values are used.
	

	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, World::instance()->getWidth(), World::instance()->getHeight(), OF_WINDOW);
	ofRunApp(new logoApp()); // start the app
}
