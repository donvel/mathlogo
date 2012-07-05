#include "interpreter.h"

using namespace std;
using namespace boost;

Interpreter* Interpreter::interpreterInstance = NULL;

Interpreter::Interpreter() : running(false) {
	
	keywords.insert("if");
	keywords.insert("while");
	keywords.insert("make");
	
	functions.insert("forward");
	functions.insert("turn");
	functions.insert("print");
	
	operators.insert("+");
	operators.insert("-");
	operators.insert("*");
	operators.insert("/");
}


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
	parseScriptFile();
	scriptFile.close();
}

void Interpreter::parseScriptFile() {
	string token;
	while(!scriptFile.eof()) {
		scriptFile >> token;
		script.push_back(token);
	}
}

void Interpreter::execute() {
	if(script.empty()) {
		thread interpreterThread(&Interpreter::randomMoves, this); // start the app	
	} else {
		thread interpreterThread(&Interpreter::runScript, this, script); // start the app	
	}
}

void Interpreter::randomMoves() {
	srand(time(0));
	while(true) {
		this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
		if(!running) continue;
		World::instance()->forward(rand() % 3);
		World::instance()->rotate(rand() % 6 - 3);
	}
}

void Interpreter::toggleRunning() {
	running = !running;
	cout << "State changed, running = " << (int)running << endl;
}


bool Interpreter::isData(string token) {
	return ((token[0] >= '0' && token[0] <= '9') || token[0] == '-' || token[0] == "\"" || token[0] == ":");
}

LogoData Interpreter::getData(string token) {
	if(token[0] == "\"") {
		token.erase(token.begin());
		return LogoData(token);
	}
	if(token[0] == ":") {
		token.erase(token.begin());	
		for(int i = variables.size() - 1; i >= 0; i--) {
			if(variables[i].find(token) != variables[i].end()) return variables[i][token];
		}
		cout << "variable " << token << " not defined." << endl;
		ofExit(0);
	}
	return LogoData(token);
}

void Interpreter::runScript(vector<string> code) {

	vector<LogoData> stack;
	cout << "Running script file." << endl;
	for(vector<string>::iterator it = code.begin(); it != code.end(); it++) {
		cout << *it << endl;
		if(functions.find(*it) !- functions.end()) {
			stack.push_back(getData(*it, true));
		} else if(isData(*it)) {
			stack.push_back(getData(*it));
		} else if(operators.find(*it) != operators.end()) {
			
		}
//		} else if (isOperator(token)) {
//			
//		} else if (token == "(") {
//			stack.push_back(token);
//		} else if (token == ")") {
//			
//		} else if 
//		}
	}
	
}
