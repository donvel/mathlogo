#pragma once

class Interpreter {
	private:
		Interpreter(); // Interpreter is a singleton
		static Interpreter *interpreterInstance;
	public:
		void execute(char*);
		void randomMoves();
		static Interpreter* instance();
};
