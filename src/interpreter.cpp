#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "interpreter.h"

using namespace std;

Interpreter* Interpreter::interpreterInstance = NULL;

Interpreter::Interpreter() {}

Interpreter* Interpreter::instance() {
	if(interpreterInstance == NULL) {
		interpreterInstance = new Interpreter();
	}
	return interpreterInstance;
}

void Interpreter::execute(char *filename) {
	fstream scriptFile;
	scriptFile.open(filename, fstream::in);
	if(!scriptFile.good()) {
		cout << "Corrupted script file." << endl;
		exit(1);
	}
	scriptFile.close();
}

void Interpreter::randomMoves() {
	while(1) cout << "a" << endl;		
}
