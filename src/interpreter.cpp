#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <boost/thread.hpp>

#include "interpreter.h"

using namespace std;

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
		boost::thread interpreterThread(randomMoves); // start the app	
	}
}

void Interpreter::randomMoves() {
	while(1) {
		while(!running);
		cout << "a" << endl;
	}
}

void Interpreter::toggleRunning() {
	running = ~running;
}

Interpreter::~Interpreter() {
	scriptFile.close();
}
