#include <fstream>
#include <iostream>
#include <cstdlib>
#include <boost/thread.hpp>

#include "interpreter.h"
#include "world.h"

using namespace std;
using namespace boost;

Interpreter* Interpreter::interpreterInstance = NULL;

Interpreter::Interpreter() : running(false) {}

Interpreter* Interpreter::instance() {
	if(interpreterInstance == NULL) {
		interpreterInstance = new Interpreter();
	}
	return interpreterInstance;
}

void Interpreter::loadScript(char *filename) {
	scriptFile.open(filename, fstream::in);
	if(!scriptFile.good()) {
		cout << "Corrupted script file." << endl;
		exit(1);
	}
}

void Interpreter::execute() {
	if(!scriptFile.is_open()) {
		thread interpreterThread(&Interpreter::randomMoves, this); // start the app	
	}
}

void Interpreter::randomMoves() {
	while(true) {
		this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
		if(!running) continue;
		cout << "a" << endl;
	}
}

void Interpreter::toggleRunning() {
	running = !running;
	cout << "State changed, running = " << (int)running << endl;
}

Interpreter::~Interpreter() {
	scriptFile.close();
}
