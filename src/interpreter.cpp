#include "interpreter.h"

using namespace std;
using namespace boost;

Function::Function(int _argNum, bool _builtin, vector<string> _args, vector<string> _body) : 
	argNum(_argNum), builtin(_builtin), args(_args), body(_body) {}


Interpreter* Interpreter::interpreterInstance = NULL;

Interpreter::Interpreter() : running(false) {
	
	functions["if"] = Function(1);
	functions["while"] = Function(1);	
	functions["make"] = Function(2);	
	functions["forward"] = Function(1);
	functions["right"] = Function(1);
	functions["left"] = Function(1);
	functions["print"] = Function(1);
	functions["penup"] = Function(0);
	functions["pendown"] = Function(0);
	functions["setxy"] = Function(2);
	
	precedence["="] = 0;
	precedence["+"] = 1;
	precedence["-"] = 1;
	precedence["*"] = 2;
	precedence["/"] = 2;
	

}

bool Interpreter::isOperator(string token) {
	return precedence.find(token) != precedence.end();
}

bool Interpreter::isBracket(string token) {
	return token == "[" || token == "]" || token == "(" || token == ")";
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

void Interpreter::lowerCase(string &token) {
	for(int i = 0; i < (int)token.size(); i++) {
		token[i] = tolower(token[i]);
	}
}

void Interpreter::extend(string &token) {
	if(token == "fd") token = "forward";
	if(token == "rt") token = "right";
	if(token == "lt") token = "left";
	if(token == "cs") token = "clear";
	if(token == "pu") token = "penup";
	if(token == "pd") token = "pendown";
}

void Interpreter::parseScriptFile() {
	string token, name;
	int firstToken, lastToken;
	vector<string> args, body;
	vector<pair<int, int> > waitingKeywords; //some of these variables are not in use yet
	string type;
	scriptFile >> type;
	
	while(!scriptFile.eof()) {
		scriptFile >> token;

		string firstCharacter(1);
		copy(token.begin(), token.begin() + 1, firstCharacter.begin());
		if(isOperator(firstCharacter) || isBracket(firstCharacter)) {
			script.push_back(firstCharacter);
			token.erase(token.begin());
			if(token.empty()) continue;
		}

		lowerCase(token);
		extend(token);
		script.push_back(token);
	}
	script.pop_back(); // Reads last token twice (I don't know why), so we want to get rid of the duplicate.
	
	if(type == "COMMANDS") { // This means that the file conatains only basic commands
		useBareCommands = true;
	} else {
		useBareCommands = false;
			
		for(int i = 0; i < (int)script.size(); i++) {
			token = script[i];
			if(token == "to") {
				name = script[++i];
				firstToken = i + 1;
			} else if(token == "end") {
				lastToken = i - 1;
				args.clear(), body.clear();
				for(int j = firstToken; j <= lastToken; j++) {
					string cToken = string[j];
					if(cToken[0] == ":") {
						cToken.erase(cToken.begin())
						args.push_back(cToken);
					} else {
						body.push_back(cToken);
					}
					functions[name] = Function(args.size(), 0, args, body);
				}
			} else if(token == "while") {
				waitingKeywords.push_back(make_pair(name, make_pair(i, 0)));
			} else if(token == "ifelse") {
				waitingKeywords.push_back(make_pair(name, make_pair(i, 1)));
				waitingKeywords.push_back(make_pair(name, make_pair(i, 0)));
			} else if(token == "[") {
				blockFrames[waitingKeywords.back()].first = i + 1;
			} else if(token == "]") {
				blockFrames[waitingKeywords.back()].second = i - 1;
				waitingKeywords.pop_back();
			} 
		}
	}
}

void Interpreter::runCommands() {
	int iterator = 0;
	while(iterator < (int)script.size()) {
		this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
		//Turtle may stop after executing every command, so we can see it moving. 
		//Alternatively, it could pause after each 1 px movement
		if(!running) continue;
		//This makes the turtle inactive when interpreter is not running
		
		// A long list of commands, maybe there is a mor elegant way of implementing it
		if(script[iterator] == "forward") {
			World::instance()->forward(strtod(script[++iterator].c_str(), NULL));
			
		} else if(script[iterator] == "left") {
			World::instance()->rotate(strtod(script[++iterator].c_str(), NULL));
			
		} else if(script[iterator] == "right") {
			World::instance()->rotate(-strtod(script[++iterator].c_str(), NULL));
			
		} else if(script[iterator] == "toggle") {
			World::instance()->toggleTurtle();
			
		} else if(script[iterator] == "mobius") {
			comp args[4];
			for (int j = 0; j < 4; j++) {
				args[j].real(strtod(script[++iterator].c_str(), NULL));
				args[j].imag(strtod(script[++iterator].c_str(), NULL));
			}
			World::instance()->setMobius(args[0], args[1], args[2], args[3]);
			
		} else if(script[iterator] == "penup") {
			World::instance()->penUp();
			
		} else if(script[iterator] == "pendown") {
			World::instance()->penDown();
			
		} else if(script[iterator] == "clear") {
			World::instance()->clear();
			
		} else if(script[iterator] == "pencolor") {
			World::instance()->setPenColor(strtol(script[++iterator].c_str(), NULL, 10));
			
		} else { 
			cout << "Unknown command" << endl;
			toggleRunning(); // Switch off the intepreter
		}
		iterator++;
	}
	cout << "End of commands" << endl;
//	randomMoves(); We may want the turtle to move randomly, when we have no further ideas for commands
}

void Interpreter::execute() {
	if(script.empty()) {
		thread interpreterThread(&Interpreter::randomMoves, this); // start it in separate thread
		//actually it should accept input from the console
	} else {
		if(useBareCommands) {
			thread interpreterThread(&Interpreter::runCommands, this);
		} else {
//			pair<int, int> mainFrame = functionFrames["main"];
//			thread interpreterThread(&Interpreter::runFunction, this, mainFrame.first, mainFrame.second); // TODO
		}
	}
}

void Interpreter::randomMoves() {
	srand(time(0));
	while(true) {
		this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
		if(!running) continue;
		World::instance()->forward(rand() % 3);
		World::instance()->rotate(rand() % 6 - 3); // possible values: -3, ... 2 , so generally turns to one side
	}
}

void Interpreter::toggleRunning() {
	running = !running;
	cout << "State changed, running = " << (int)running << endl;
}

// -------------------TODO----------------------------------//
bool Interpreter::isData(string token) {
	return ((token[0] >= '0' && token[0] <= '9') || token[0] == "\"" || token[0] == ":");
}

LogoData Interpreter::getData(string token) {
	if(token[0] == "\"") {
		token.erase(token.begin());
		return LogoData(token);
	}
	if(token[0] == ":") {
		token.erase(token.begin());	
		for(int i = variables.size() - 1; i >= 0; i--) {
			if(variables[i].find(token) != variables[i].end()) return variables[i][token]; // dynamic scope
		}

//		if(variables.back().find(token) != variables.end()) return variables.back()[token]; // local variables
//		if(variables.front().find(token) != variables.end()) return variables.front()[token];// global variables
		cout << "variable " << token << " not defined." << endl;
		ofExit(0);
	}
	return LogoData(token);
}

LogoData executeOperator(string oper, LogoData a, LogoData b) {
	if(oper == "+") return a + b;
	if(oper == "-") return a - b;
	if(oper == "/") return a / b;
	if(oper == "*") return a * b;
	if(oper == "=") return a == b;
}

void Interpreter::executeLast(vector<LogoData> &values, vector<string> &stack, 
		vector<int> valuesNeeded, vector<int> valuesAvailable, int &iterator) {
	
	if(functions.find(stack.back()) != functions.end()) {
		int numArgs = valuesNeeded.back();
		vector<LogoData> args(numArgs);
		copy(values.begin() + (values.size() - numArgs), values.end(), args.begin());
		values.resize(values.begin() + (values.size() - numArgs));
		if(myFunctions.find(stack.end()) != myFunctions.end()) {
			values.push_back(runMyFunction(stack.end(), args, iterator));
		} else {
			values.push_back(runFunction(stack.end(), args));
		}
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

LogoData Interpreter::runMyFunction(int namePos, vector<LogoData> parameters, int &iterator) {
	string name = script[namePos];
	if(name == "print") {
		cout << parameters[0].toString() << endl;
	} else if(name == "make") {
		variables.back()[parameters[0].toString()] = parameters[1];
	} else if(name == "forward") {
		World::instance()->forward(parameters[0].toDouble());
	} else if(name == "right") {
		World::instance()->rotate(parameters[0].toDouble());
	} else if(name == "left") {
		World::instance()->rotate(parameters[0].toDouble());
	} else if(name == "ifelse") {
		if(parameters[0].toBoolean()) {
			pair<int, int> frame= blockFrames[make_pair(namePos, 0)];
			runFunction(frame.first, frame.second);
		} else {
			pair<int, int> frame= blockFrames[make_pair(namePos, 1)];
			runFunction(frame.first, frame.second);
		}
		iterator = blockFrames[make_pair(namePos, 1)].second + 1;
	} else if(name == "while") {
		if(parameters[0].toBoolean()) {
			pair<int, int> frame= blockFrames[make_pair(namePos, 0)];
			runFunction(frame.first, frame.second);
			iterator = namePos;
		} else {
			iterator = blockFrames[make_pair(namePos, 1)].second();
		}
	}
	return LogoData("");
}

LogoData Interpreter::runCode(string name, int firstToken, int lastToken, vector<LogoData> parameters, bool setParameters) {
//	if(myFunctions.find(name) != myFunctions.end()) return runMyFunction(name, firstToken, parameters);
//	int lastToken = functionFrames[name].second;
	
	if(setParameters) initiateParameters(firstToken, parameters);
	
	vector<int> valuesNeeded;
	valuesNeeded.push_back(0); // guardian
	vector<int> valuesAvailable;
	valuesAvailable.push_back(0); // guardian
	
	vector<LogoData> values;
	vector<int> stack; // Keeps function / operator names
	
	cout << "Running script file." << endl;
	
//	for(vector<string>::iterator it = script.begin() + firstToken; it != script.begin() + lastToken; it++) {
	for(int iterator = firstToken; iterator <= lastToken; iterator++) {
//		vector<string>::iterator it = script.begin() + iterator;
		string token = script[iterator];
		
		while(valuesAvailable.back() > 0 && valuesAvailable.back() == valuesNeeded.back() && operators.find(token) == operators.end()) {
			executeLast(values, stack, valuesNeeded, valuesAvailable, iterator);
		}
		token = script[iterator];
		
		if(functions.find(token) != functions.end()) {
			stack.push_back(iterator);
			valuesAvailable.push_back(0);
			valuesNeeded.push_back(functions[token]);
		} else if(isData(token)) {
			values.push_back(getData(token));
			valuesAvailable.back++;
		} else if(operators.find(token) != operators.end()) {
			while(!stack.empty() && operators.find(token) != operators.end() && precedence[token] <= precedence[stack.front()]) {
				executeLast(values, stack, valuesNeeded, valuesAvailable, iterator);
			}
			stack.push_back(iterator);
			
		} else if(token == "(") {
			stack.push_back(iterator);
		} else if(token == ")") {
			while(script[stack.back()] != "(") {
				executeLast(values, stack, valuesNeeded, valuesAvailable, iterator);
			}
			stack.pop_back();
		}
	}
	
	while(valuesAvailable.back() > 0 && valuesAvailable.back() == valuesNeeded.back()) {
		executeLast(values, stack, valuesNeeded, valuesAvailable, iterator);
	}
	return values[0];
}
