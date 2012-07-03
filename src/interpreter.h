#pragma once
#include <fstream>

using namespace std;

class Interpreter {
	private:
		Interpreter(); // Interpreter is a singleton
		~Interpreter();
		static Interpreter *interpreterInstance;
		bool running;
		fstream scriptFile;
	public:
		void loadScript(char*);
		void execute();
		void randomMoves();
		void toggleRunning();
		static Interpreter* instance();
};
