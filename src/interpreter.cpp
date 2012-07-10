#include "interpreter.h"

using namespace std;
using namespace boost;

Interpreter* Interpreter::interpreterInstance = NULL;

Interpreter::Interpreter() : running(false) {
	

/*	myFunctions["if"] = 1;
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
	
*/
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
	string type;
	scriptFile >> type;
	
	if(type == "COMMANDS") {
		useBareCommands = true;
		while(!scriptFile.eof()) {
			scriptFile >> token;
			cout << "token = " << token << endl;
			script.push_back(token);
		}
	} else {
		useBareCommands = false;
			/*
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
		}*/
	}
	script.pop_back(); // Reads last token twice, so we want to get rid of the duplicate.
}

void Interpreter::runCommands() {
	int iterator = 0;
	while(iterator < (int)script.size()) {
		this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
		if(!running) continue;
		cout << script[iterator] << endl;
		if(script[iterator] == "FORWARD") {
			World::instance()->forward(strtod(script[++iterator].c_str(), NULL));
		} else if(script[iterator] == "LEFT") {
			World::instance()->rotate(strtod(script[++iterator].c_str(), NULL));
		} else if(script[iterator] == "RIGHT") {
			World::instance()->rotate(-strtod(script[++iterator].c_str(), NULL));
		} else {
			cout << "Unknown command" << endl;
			toggleRunning();
		}
		iterator++;
	}
	cout << "End of commands" << endl;
}

void Interpreter::execute() {
	if(script.empty()) {
		thread interpreterThread(&Interpreter::randomMoves, this); // start the app	
	} else {
		if(useBareCommands) {
			thread interpreterThread(&Interpreter::runCommands, this);
		} else {
//			pair<int, int> mainFrame = functionFrames["main"];
//			thread interpreterThread(&Interpreter::runFunction, this, mainFrame.first, mainFrame.second); // start the app	
		}
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

//
//bool Interpreter::isData(string token) {
//	return ((token[0] >= '0' && token[0] <= '9') || token[0] == '-' || token[0] == "\"" || token[0] == ":");
//}
//
//LogoData Interpreter::getData(string token) {
////	if(token[0] == "\"") {
////		token.erase(token.begin());
////		return LogoData(token);
////	}
////	if(token[0] == ":") {
////		token.erase(token.begin());	
//////		for(int i = variables.size() - 1; i >= 0; i--) {
//////			if(variables[i].find(token) != variables[i].end()) return variables[i][token];
//////		}
////
////		if(variables.back().find(token) != variables.end()) return variables.back()[token];
////		if(variables.front().find(token) != variables.end()) return variables.front()[token];
////		cout << "variable " << token << " not defined." << endl;
////		ofExit(0);
////	}
////	return LogoData(token);
//}
//
//LogoData executeOperator(string oper, LogoData a, LogoData b) {
////	if(oper == "+") return LogoData(a + b);
////	if(oper == "-") return LogoData(a - b);
////	if(oper == "/") return LogoData(a / b);
////	if(oper == "*") return LogoData(a * b);
////	if(oper == "=") return LogoData(a == b);
//}
//
//void Interpreter::executeLast(vector<LogoData> &values, vector<string> &stack, 
//		vector<int> valuesNeeded, vector<int> valuesAvailable, int &iterator) {
//	
////	if(functions.find(stack.back()) != functions.end()) {
////		int numArgs = valuesNeeded.back();
////		vector<LogoData> args(numArgs);
////		copy(values.begin() + (values.size() - numArgs), values.end(), args.begin());
////		values.resize(values.begin() + (values.size() - numArgs));
////		if(myFunctions.find(stack.end()) != myFunctions.end()) {
////			values.push_back(runMyFunction(stack.end(), args, iterator));
////		} else {
////			values.push_back(runFunction(stack.end(), args));
////		}
////		stack.pop_back();
////		valuesAvailable.pop_back();
////		valuesAvailable.back()++;
////		valuesNeeded.pop_back();
////	} else {
////		LogoData a = values[values.size() - 2], b = values[values.size() - 1];
////		values.resize(values.size() - 2);
////		values.push_back(executeOperator(stack.back(), a, b));
////		stack.pop_back();
////		valuesAvailable.pop_back();
////		valuesAvailable.back()++;
////		valuesNeeded.pop_back();
////	}
//}
//
//void Interpreter::initiateParameters(int &firstToken, vector<LogoData> parameters) {
////	variables.resize(variables.size() + 1);
////	for(vector<LogoData>::iterator it = parameters.begin(); it != parameters.end(); it++) {
////		string token = script[firstToken++];
////		if(token[0] != "\"") {
////			cout << "Expected more parameters" << endl;
////		} else {
////			token = token.erase(token.begin());
////			variables.back()[token] = *it;
////		}
////	}
//}
//
//LogoData Interpreter::runMyFunction(int namePos, vector<LogoData> parameters, int &iterator) {
////	string name = script[namePos];
////	if(name == "print") {
////		cout << parameters[0].toString() << endl;
////	} else if(name == "make") {
////		variables.back()[parameters[0].toString()] = parameters[1];
////	} else if(name == "forward") {
////		World::instance()->forward(parameters[0].toDouble());
////	} else if(name == "right") {
////		World::instance()->rotate(parameters[0].toDouble());
////	} else if(name == "left") {
////		World::instance()->rotate(parameters[0].toDouble());
////	} else if(name == "ifelse") {
////		if(parameters[0].toBoolean()) {
////			pair<int, int> frame= blockFrames[make_pair(namePos, 0)];
////			runFunction(frame.first, frame.second);
////		} else {
////			pair<int, int> frame= blockFrames[make_pair(namePos, 1)];
////			runFunction(frame.first, frame.second);
////		}
////		iterator = blockFrames[make_pair(namePos, 1)].second + 1;
////	} else if(name == "while") {
////		if(parameters[0].toBoolean()) {
////			pair<int, int> frame= blockFrames[make_pair(namePos, 0)];
////			runFunction(frame.first, frame.second);
////			iterator = namePos;
////		} else {
////			iterator = blockFrames[make_pair(namePos, 1)].second();
////		}
////	}
////	return LogoData("");
//}
//
//LogoData Interpreter::runFunction(int firstToken, int lastToken, vector<LogoData> parameters, bool setParameters) {
//////	if(myFunctions.find(name) != myFunctions.end()) return runMyFunction(name, firstToken, parameters);
//////	int lastToken = functionFrames[name].second;
////	
////	if(setParameters) initiateParameters(firstToken, parameters);
////	
////	vector<int> valuesNeeded;
////	valuesNeeded.push_back(0); // guardian
////	vector<int> valuesAvailable;
////	valuesAvailable.push_back(0); // guardian
////	
////	vector<LogoData> values;
////	vector<int> stack; // Keeps function / operator names
////	
////	cout << "Running script file." << endl;
////	
//////	for(vector<string>::iterator it = script.begin() + firstToken; it != script.begin() + lastToken; it++) {
////	for(int iterator = firstToken; iterator <= lastToken; iterator++) {
//////		vector<string>::iterator it = script.begin() + iterator;
////		string token = script[iterator];
////		
////		while(valuesAvailable.back() > 0 && valuesAvailable.back() == valuesNeeded.back() && operators.find(token) == operators.end()) {
////			executeLast(values, stack, valuesNeeded, valuesAvailable, iterator);
////		}
////		token = script[iterator];
////		
////		if(functions.find(token) != functions.end()) {
////			stack.push_back(iterator);
////			valuesAvailable.push_back(0);
////			valuesNeeded.push_back(functions[token]);
////		} else if(isData(token)) {
////			values.push_back(getData(token));
////			valuesAvailable.back++;
////		} else if(operators.find(token) != operators.end()) {
////			while(!stack.empty() && operators.find(token) != operators.end() && precedence[token] <= precedence[stack.front()]) {
////				executeLast(values, stack, valuesNeeded, valuesAvailable, iterator);
////			}
////			stack.push_back(iterator);
////			
////		} else if(token == "(") {
////			stack.push_back(iterator);
////		} else if(token == ")") {
////			while(script[stack.back()] != "(") {
////				executeLast(values, stack, valuesNeeded, valuesAvailable, iterator);
////			}
////			stack.pop_back();
////		}
////	}
////	
////	while(valuesAvailable.back() > 0 && valuesAvailable.back() == valuesNeeded.back()) {
////		executeLast(values, stack, valuesNeeded, valuesAvailable, iterator);
////	}
////	return values[0];
//}
