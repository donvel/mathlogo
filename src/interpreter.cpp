#include "interpreter.h"

using namespace std;
using namespace boost;

Interpreter* Interpreter::interpreterInstance = NULL;

Interpreter::Interpreter() : running(false) {
	

	myFunctions["if"] = 1;
	myFunctions["while"] = 1;	
	myFunctions["make"] = 2;	
	myFunctions["forward"] = 1;
	myFunctions["right"] = 1;
	myFunctions["left"] = 1;
	myFunctions["print"] = 1;
	myFunctions["penup"] = 0;
	myFunctions["pendown"] = 0;
	myFunctions["setxy"] = 2;

	for(map<string, int>::iterator it = myFunctions.begin(); it != myFunctions.end(); it++) {
		functions.insert(*it);
	}

	operators.insert("=");
	operators.insert("+");
	operators.insert("-");
	operators.insert("*");
	operators.insert("/");
	
	precedence["="] = 0;
	precedence["+"] = 1;
	precedence["-"] = 1;
	precedence["*"] = 2;
	precedence["/"] = 2;
	

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
	string token, waitingFunction;
	vector<pair<int, int> > waitingKeywords;
	while(!scriptFile.eof()) {
		scriptFile >> token;
		if(token == "to") {
			scriptFile >> waitingFunction;
			functionFrames[waitingFunction].first = script.size();
		} else if(token == "end") {
			functionFrames[waitingFunction].second = script.size() - 1;
		} else if(token == "while") {
			waitingKeywords.push_back(make_pair(script.size(), 0));
		} else if(token == "ifelse") {
			waitingKeywords.push_back(make_pair(script.size(), 1));
			waitingKeywords.push_back(make_pair(script.size(), 0));
		} else if(token == "[") {
			blockFrames[waitingKeywords.back()].first = script.size();
		} else if(token == "]") {
			blockFrames[waitingKeywords.back()].second = script.size() - 1;
		} else{
			string firstCharacter(1);
			copy(token.begin(), token.begin() + 1, firstCharacter.begin());
			if(operators.find(firstCharacter) != operators.end() || firstCharacter == "(" ||
					firstCharacter == ")") {
				script.push_back(firstCharacter);
				token.erase(token.begin());
				if(!token.empty()) script.push_back(token);
			} else {
				script.push_back(token);
			}
		}
	}
}

void Interpreter::execute() {
	if(script.empty()) {
		thread interpreterThread(&Interpreter::randomMoves, this); // start the app	
	} else {
		thread interpreterThread(&Interpreter::runFunction, this, "main"); // start the app	
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
//		for(int i = variables.size() - 1; i >= 0; i--) {
//			if(variables[i].find(token) != variables[i].end()) return variables[i][token];
//		}

		if(variables.back().find(token) != variables.end()) return variables.back()[token];
		if(variables.front().find(token) != variables.end()) return variables.front()[token];
		cout << "variable " << token << " not defined." << endl;
		ofExit(0);
	}
	return LogoData(token);
}

LogoData executeOperator(string oper, LogoData a, LogoData b) {
	if(oper == "+") return LogoData(a + b);
	if(oper == "-") return LogoData(a - b);
	if(oper == "/") return LogoData(a / b);
	if(oper == "*") return LogoData(a * b);
	if(oper == "=") return LogoData(a == b);
}

void Interpreter::executeLast(vector<LogoData> &values, vector<string> &stack, vector<int> valuesNeeded, vector<int> valuesAvailable) {
	if(functions.find(stack.back()) != functions.end()) {
		int numArgs = valuesNeeded.back();
		vector<LogoData> args(numArgs);
		copy(values.begin() + (values.size() - numArgs), values.end(), args.begin);
		values.resize(values.begin() + (values.size() - numArgs));
		values.push_back(runFunction(stack.end(), args));
		stack.pop_back();
		valuesAvailable.pop_back();
		valuesAvailable.back()++;
		valuesNeeded.pop_back();
	} else {
		LogoData a = values[values.size() - 2], b = values[values.size() - 1];
		values.resize(values.size() - 2);
		values.push_back(executeOperator(stack.back(), a, b));
		stack.pop_back();
		valuesAvailable.pop_back();
		valuesAvailable.back()++;
		valuesNeeded.pop_back();
	}
}

void Interpreter::initiateParameters(int &firstToken, vector<LogoData> parameters) {
	variables.resize(variables.size() + 1);
	for(vector<LogoData>::iterator it = parameters.begin(); it != parameters.end(); it++) {
		string token = script[firstToken++];
		if(token[0] != "\"") {
			cout << "Expected more parameters" << endl;
		} else {
			token = token.erase(token.begin());
			variables.back()[token] = *it;
		}
	}
}

LogoData Interpreter::runMyFunction(string name, int &fisrtToken, vector<LogoData> parametres) {
	if(name == "print") {
		cout << parametres[0].toString() << endl;
	} else if(name == "make") {
		variables.back()[parametres[0].toString()] = parametres[1];
	} else if(name == "forward") {
		World::instance()->forward(parametres[0].toDouble());
	} else if(name == "right") {
		World::instance()->rotate(parametres[0].toDouble());
	} else if(name == "left") {
		World::instance()->rotate(parametres[0].toDouble());
	} else if(name == "ifelse") {
		if(parameters[0].toBoolean) {
			runScript();
		} else {
			
		}
	}
	return LogoData("");
}

LogoData Interpreter::runFunction(string name, vector<LogoData> parameters, bool setParameters) {
	if(myFunctions.find(name) != myFunctions.end()) return runMyFunction(name, parameters);
	
	int firstToken = functionFrames[name], lastToken = functionFrames[name];
	
	if(setParameters) initiateParameters(firstToken, parameters);
	
	vector<int> valuesNeeded;
	valuesNeeded.push_back(0); // guardian
	vector<int> valuesAvailable;
	valuesAvailable.push_back(0); // guardian
	
	vector<LogoData> values;
	vector<string> stack;
	
	cout << "Running script file." << endl;
	
	for(vector<string>::iterator it = script.begin() + firstToken; it != script.begin() + lastToken; it++) {
		cout << *it << endl;
		
		while(valuesAvailable.back() > 0 && valuesAvailable.back() == valuesNeeded.back() && operators.find(*it) == operators.end()) {
			executeLast(values, stack, valuesNeeded, valuesAvailable);
		}
		if(functions.find(*it) != functions.end()) {
			stack.push_back(*it);
			valuesAvailable.push_back(0);
			valuesNeeded.push_back(functions[*it]);
		} else if(isData(*it)) {
			values.push_back(getData(*it));
			valuesAvailable.back++;
		} else if(operators.find(*it) != operators.end()) {
			while(!stack.empty() && operators.find(*it) != operators.end() && precedence[*it] <= precedence[stack.front()]) {
				executeLast();
			}
		} else if(*it == "(") {
			stack.push_back(*it);
		} else if(*it == ")") {
			while(stack.back() != "(") {
				executeLast();
			}
			stack.pop_back();
		}
	}
	
	while(valuesAvailable.back() > 0 && valuesAvailable.back() == valuesNeeded.back()) {
		executeLast(values, stack, valuesNeeded, valuesAvailable);
	}
	return values[0];
}
