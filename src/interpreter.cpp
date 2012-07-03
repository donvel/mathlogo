#include "interpreter.h"

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
	srand(time(0));
	while(true) {
		this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
		if(!running) continue;
		cout << "a" << endl;
		World::instance()->forward(rand() % 3);
		World::instance()->rotate(rand() % 6 - 3);
	}
}

void Interpreter::toggleRunning() {
	running = !running;
	cout << "State changed, running = " << (int)running << endl;
}

Interpreter::~Interpreter() {
	scriptFile.close();
}
