#pragma once

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <boost/thread.hpp>

#include "world.h"

using namespace std;

class Interpreter {
	private:
		Interpreter(); // Interpreter is a singleton
		~Interpreter();
		static Interpreter *interpreterInstance;
		bool running;
		fstream scriptFile;
	public:
		void loadScript(char* filename);
		void execute();
		void randomMoves();
		void toggleRunning();
		static Interpreter* instance();
};
