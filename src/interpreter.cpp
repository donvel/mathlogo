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
		boost::thread interpreterThread(&Interpreter::randomMoves, this); // start the app	
	}
}

void Interpreter::randomMoves() {
	while(true) {
		int curentTime = time(0);
		while(time(0) < curentTime + 2);
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
