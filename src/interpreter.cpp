#include "interpreter.h"

using namespace std;
using namespace boost;

Function::Function(int _argNum, bool _builtin, vector<string> _args, vector<string> _body) : 
	argNum(_argNum), builtin(_builtin), args(_args), body(_body) {}


Interpreter* Interpreter::interpreterInstance = NULL;

Interpreter::Interpreter() : running(false) {
	
	srand(time(0));
	
	functions["ifelse"] = Function(1);
	functions["if"] = Function(1);
	functions["while"] = Function(1);
	functions["repeat"] = Function(1);
	functions["make"] = Function(2);	
	functions["forward"] = Function(1);
	functions["right"] = Function(1);
	functions["left"] = Function(1);
	functions["print"] = Function(1);
	functions["penup"] = Function(0);
	functions["pendown"] = Function(0);
	functions["setpencolor"] = Function(1);
	functions["clearscreen"] = Function(0);
	functions["setxy"] = Function(2);
	functions["headto"] = Function(2);
	functions["getx"] = Function(0);
	functions["gety"] = Function(0);
	functions["getangle"] = Function(0);
	functions["fill"] = Function(0);
	functions["return"] = Function(1);
	functions["random"] = Function(1);
	functions["sqrt"] = Function(1);
	
/* -------- Transformations -------------------*/
	
	transFunctions["moebius"] = Function(8);
	transFunctions["threepointsmoebius"] = Function(12);
	transFunctions["translate"] = Function(2);
	transFunctions["inverse"] = Function(3); //Complex inverse + mirror
	transFunctions["mirror"] = Function(1); //can't be implemented with Moebius!
	transFunctions["rotate"] = Function(3);
	transFunctions["homothety"] = Function(3);
	transFunctions["cleartransform"] = Function(0);
	transFunctions["toggle"] = Function(0);
	
	
	
	precedence["="] = 0;
	precedence["<"] = 0;
	precedence[">"] = 0;
	precedence["+"] = 1;
	precedence["-"] = 1;
	precedence["*"] = 2;
	precedence["/"] = 2;
	precedence["%"] = 2;
	
	if(World::instance()->getMode() == TRANSFORM) {
		for(map<string, Function>::iterator it = transFunctions.begin(); it != transFunctions.end(); it++) {
			functions.insert(*it);
		}
	}

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

void Interpreter::lowerCase(string &token) { // Logo does not distinguish between upper and lowercase
	for(int i = 0; i < (int)token.size(); i++) {
		token[i] = tolower(token[i]);
	}
}

void Interpreter::extend(string &token) {
	if(token == "fd") token = "forward";
	if(token == "rt") token = "right";
	if(token == "lt") token = "left";
	if(token == "cs") token = "clearscreen";
	if(token == "pu") token = "penup";
	if(token == "pd") token = "pendown";
	if(token == "ct") token = "cleartransform";
	if(token == "tpm") token = "threepointsmoebius";
	
}

void Interpreter::parseScriptFile() {
	string token, name;
	int firstToken = 0, lastToken;
	vector<string> args, body;
	string type;
	scriptFile >> type;
	
	while(!scriptFile.eof()) {
		scriptFile >> token;

		string firstCharacter("#");
		bool nextToken = false;
		copy(token.begin(), token.begin() + 1, firstCharacter.begin());
		while(isOperator(firstCharacter) || isBracket(firstCharacter)) { // We separate "glued" characters
			script.push_back(firstCharacter);
			token.erase(token.begin());
			if(token.empty()) {
				nextToken = true;
				break;
			};
			copy(token.begin(), token.begin() + 1, firstCharacter.begin());
		}
		if(nextToken) continue;

		lowerCase(token);
		extend(token);
		
		string lastCharacter("#");
		vector<string> reversed;
		copy(token.end() - 1, token.end(), lastCharacter.begin());
		while(isOperator(lastCharacter) || isBracket(lastCharacter)) {
			token.erase(token.end() - 1);
			if(token.empty()) break;
			reversed.push_back(lastCharacter);
			copy(token.end() - 1, token.end(), lastCharacter.begin());
		}
		if(!token.empty()) script.push_back(token);
		for(int i = (int)reversed.size() - 1; i >= 0; i--) {
			script.push_back(reversed[i]);
		}
	}
	script.pop_back(); // Reads last token twice (I don't know why), so we want to get rid of the duplicate.
	
	if(type == "COMMANDS") { // This means that the file contains only basic commands
		useBareCommands = true; // we are not going to use it anymore
	} else {
		useBareCommands = false;
			
		for(int i = 0; i < (int)script.size(); i++) {
			token = script[i];
			cout << token << endl;

			if(token == "to") {
				name = script[++i];
				cout << name << endl;
				firstToken = i + 1;
			} else if(token == "end") {
				lastToken = i - 1;
				args.clear(), body.clear();
				bool begin = true;
				for(int j = firstToken; j <= lastToken; j++) {
					string cToken = script[j];
					if(cToken[0] == ':' && begin) {
						cToken.erase(cToken.begin());
						args.push_back(cToken);
					} else {
						begin = false;
						body.push_back(cToken);
					}
				}
				functions[name] = Function(args.size(), 0, args, body);
				cout << "parsing " << name << endl;
				functions[name].parseBody();
			} 
		}
	}
	for(map<string, Function>::iterator it = functions.begin(); it != functions.end(); it++) {
		cout << it->first << " " << it->second.argNum << " " << it->second.builtin << endl;
		for(int i = 0; i < (int)it->second.args.size(); i++) cout << it->second.args[i] << " ";
		cout << endl;
		for(int i = 0; i < (int)it->second.body.size(); i++) cout << it->second.body[i] << " ";
		cout << endl;
	}
}

void Function::parseBody() {
	string token;
	vector<pair<int, int> > waitingKeywords; 
	for(int i = 0; i < (int)body.size(); i++) {
		token = body[i];
		if(token == "while" || token == "repeat" || token == "if") {
			waitingKeywords.push_back(make_pair(i, 0));
		} else if(token == "ifelse") {
			waitingKeywords.push_back(make_pair(i, 1));
			waitingKeywords.push_back(make_pair(i, 0));
		} else if(token == "[") {
			blockFrames[waitingKeywords.back()].first = i + 1;
		} else if(token == "]") {
			blockFrames[waitingKeywords.back()].second = i - 1;
			waitingKeywords.pop_back();
		} 
	}
	cout << "After parsing " << endl;
	for(map<pair<int, int>, pair<int, int> >::iterator it = blockFrames.begin(); it != blockFrames.end(); it++) {
		cout << it->first.first << " " << it->first.second << " " << it->second.first << " " << it->second.second << endl;
	}
}

void Interpreter::execute() {
	if(script.empty()) {
		thread interpreterThread(&Interpreter::randomMoves, this); // start it in separate thread
		//actually it should accept input from the console
	} else {
		if(useBareCommands) {
//			thread interpreterThread(&Interpreter::runCommands, this); // OBSOLETE
		} else {
			thread interpreterThread(&Interpreter::runCode, this, "main", 0, functions["main"].body.size() - 1, vector<LogoData>()); // TODO
		}
	}
}

void Interpreter::randomMoves() {
	srand(time(0));
	while(true) {
		this_thread::sleep(posix_time::milliseconds(World::instance()->getFrameTime()));
		if(!running) continue;
		World::instance()->forward(1 + rand() % 3);
		World::instance()->rotate(rand() % 7 - 3); // possible values: -3, ... 3 , so generally turns to one side
	}
}

void Interpreter::stopRunning() {
	running = false;
	cout << "State changed, running = " << (int)running << endl;
}

void Interpreter::startRunning() {
	running = true;
	cout << "State changed, running = " << (int)running << endl;
}

void Interpreter::toggleRunning() {
	running = !running;
	cout << "State changed, running = " << (int)running << endl;
}

bool Interpreter::isData(string token) {
	return ((token[0] >= '0' && token[0] <= '9') || token[0] == '\"' || token[0] == ':');
}

LogoData Interpreter::getData(string token) {
	if(token[0] == '\"') {
		token.erase(token.begin());
		return LogoData(token);
	}
	if(token[0] == ':') {
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
	cout << a.toString() << " :" << oper << ": " << b.toString() << endl;
	if(oper == "+") return a + b;
	if(oper == "-") return a - b;
	if(oper == "/") return a / b;
	if(oper == "*") return a * b;
	if(oper == "%") return a % b;
	if(oper == "=") return a == b;
	if(oper == "<") return a < b;
	if(oper == ">") return a > b;
	return LogoData();
}

void Interpreter::executeLast(vector<LogoData> &values, vector<int> &stack, 
		vector<int> &valuesNeeded, vector<int> &valuesAvailable, string functionName, int &iterator) {
	// I would write some comments to make this code more readable
	// But I have already forgotten what it does.
	
	string name = functions[functionName].body[stack.back()];
	
	cout << "executeLast " << name << " in " << functionName << ", iterator = " << iterator << endl;

	cout << "values: ";
	for(int i = 0; i < (int)values.size(); i++) {
		cout << values[i].toString() << " ";
	}
	cout << endl << "stack: ";

	for(int i = 0; i < (int)stack.size(); i++) {
		cout << functions[functionName].body[stack[i]] << " ";
	}
	cout << endl << "needed: ";
	for(int i = 0; i < (int)valuesNeeded.size(); i++) {
		cout << valuesNeeded[i] << " ";
	}
	cout << endl << "available: ";
	for(int i = 0; i < (int)valuesAvailable.size(); i++) {
		cout << valuesAvailable[i] << " ";
	}
	cout << endl;
	
	if(functions.find(name) != functions.end()) {
		int numArgs = valuesNeeded.back();
		vector<LogoData> args(numArgs);
		copy(values.begin() + (values.size() - numArgs), values.end(), args.begin());
		values.resize(values.size() - numArgs);
		LogoData tmp;
		if(functions[name].builtin) {
			tmp = runMyFunction(stack.back(), args, functionName, iterator);
			if(!tmp.empty()) values.push_back(tmp);
		} else {
			tmp = runCode(name, 0, (int)functions[name].body.size() - 1, args);
			if(!tmp.empty()) values.push_back(tmp);
		}
		stack.pop_back();
		valuesAvailable.pop_back();
		if(!tmp.empty()) valuesAvailable.back()++;
		valuesNeeded.pop_back();
	} else {
		if(values.size() < 2) {
			cout << "too few values for operator!" << endl;
		}
		LogoData a = values[values.size() - 2], b = values[values.size() - 1];
		values.resize(values.size() - 2);
		values.push_back(executeOperator(name, a, b));
		cout << "and the result is " << values.back().toString() << endl;
		stack.pop_back();
		valuesAvailable.pop_back();
		valuesAvailable.back()++;
		valuesNeeded.pop_back();
	}
}

LogoData Interpreter::runMyFunction(int namePos, vector<LogoData> parameters, string functionName, int &iterator) {

	vector<string> &code = functions[functionName].body;
	string name = code[namePos];
	if(name == "print") {
		cout << parameters[0].toString() << endl;
		
	} else if(name == "make") {
		(*variables.begin())[parameters[0].toString()] = parameters[1];
		cout << "new variable " << parameters[0].toString() << " =  " << variables.front()[parameters[0].toString()].toString() << endl;
		
	} else if(name == "forward") {
		World::instance()->forward(parameters[0].toDouble());
		
	} else if(name == "right") {
		World::instance()->rotate(parameters[0].toDouble());
		
	} else if(name == "left") {
		World::instance()->rotate(-parameters[0].toDouble());
		
	} else if(name == "ifelse") {
		if(parameters[0].toBoolean()) {
			pair<int, int> frame= functions[functionName].blockFrames[make_pair(namePos, 0)];
			runCode(functionName, frame.first, frame.second);
		} else {
			pair<int, int> frame= functions[functionName].blockFrames[make_pair(namePos, 1)];
			runCode(functionName, frame.first, frame.second);
		}
		iterator = functions[functionName].blockFrames[make_pair(namePos, 1)].second + 1;
		
	} else if(name == "while") {
		if(parameters[0].toBoolean()) {
			pair<int, int> frame= functions[functionName].blockFrames[make_pair(namePos, 0)];
			runCode(functionName, frame.first, frame.second);
			iterator = namePos;
		} else {
			iterator = functions[functionName].blockFrames[make_pair(namePos, 0)].second + 1;
		}
		
	} else if(name == "if") {
		if(parameters[0].toBoolean()) {
			pair<int, int> frame= functions[functionName].blockFrames[make_pair(namePos, 0)];
			runCode(functionName, frame.first, frame.second);
		}

		iterator = functions[functionName].blockFrames[make_pair(namePos, 0)].second + 1;
		
	} else if(name == "repeat") {
		pair<int, int> frame= functions[functionName].blockFrames[make_pair(namePos, 0)];
		for(int j = 0; j < (int)parameters[0].toDouble(); j++) {	
			runCode(functionName, frame.first, frame.second);
		}
		iterator = functions[functionName].blockFrames[make_pair(namePos, 0)].second + 1;
		cout << "After repeat iterator = " << iterator << endl;
	
	} else if(name == "penup") {
		World::instance()->penUp();

	} else if(name == "pendown") {
		World::instance()->penDown();
		
	} else if(name == "setpencolor") {
		World::instance()->setPenColor((int)parameters[0].toDouble());
	
	} else if(name == "clearscreen") {
		World::instance()->clearScreen();
	
	} else if(name == "setxy") {
		point p(parameters[0].toDouble(), parameters[1].toDouble());
		World::instance()->moveTurtleTo(p);
	
	} else if(name == "headto") {
		point p(parameters[0].toDouble(), parameters[1].toDouble());
		World::instance()->headTurtleTo(p);
	
	} else if(name == "fill") {
		World::instance()->fill();
	
	} else if(name == "getx") {
		return World::instance()->getTurtlePosition().x;
	
	} else if(name == "gety") {
		return World::instance()->getTurtlePosition().y;
	
	} else if(name == "getangle") {
		return World::instance()->getTurtleAngle();
	
	} else if(name == "random") {
		return random() % (int)round(parameters[0].toDouble());
	
	} else if(name == "sqrt") {
		return sqrt(parameters[0].toDouble());
	
	} else if(name == "return") {
		returnValue = parameters[0];
	
//--------------------TRANSFORMATIONS------------------------------//
		
	} else if(name == "moebius") {
		vector<comp> var;
		for(int j = 0; j < 3; j++) {
			var.push_back(comp(parameters[2 * j].toDouble(), parameters[2 * j + 1].toDouble()));
		}
		vector<comp> fp = World::instance()->addTransform(var[0], var[1], var[2], var[3]);
		
	} else if(name == "mirror") {
		World::instance()->addTransform(0, 0, 0, 0, true, parameters[0].toDouble());
		
	} else if(name == "translate") {
		World::instance()->addTransform(1, comp(parameters[0].toDouble(), parameters[1].toDouble()), 0, 1);
		
	} else if(name == "inverse") {
		comp p(parameters[0].toDouble(), parameters[1].toDouble());
		double r = parameters[2].toDouble();
		World::instance()->addTransform(p, - p * p + comp(r * r, 0), 1, -p);
		World::instance()->addTransform(0, 0, 0, 0, true, parameters[1].toDouble());
		World::instance()->addCircle(point(p.real(), p.imag()), r);
		
	} else if(name == "rotate") {
		comp p(parameters[0].toDouble(), parameters[1].toDouble());
		double phi = parameters[2].toDouble() / 360.0 * 2 * M_PI;
		comp eToIPhi = exp(comp(0, phi));
		
		vector<comp> fp = World::instance()->addTransform(eToIPhi, p * (comp(1, 0) - eToIPhi), 0, 1);

	} else if(name == "homothety") {
		comp p(parameters[0].toDouble(), parameters[1].toDouble());

		double k = parameters[2].toDouble();
		World::instance()->addTransform(k,  -p * comp(k, 0) + p, 0, 1);
		World::instance()->addCircle(point(p.real(), p.imag()), 2);

	} else if(name == "threepointsmoebius") {
		vector<vector<comp> > pts(2);
		for(int i = 0; i < 2; i++) {
			pts[i].resize(3);
			for(int j = 0; j < 3; j++) {
				pts[i][j] = comp(parameters[i * 6 + j * 2].toDouble(), parameters[i * 6 + j * 2 + 1].toDouble());
				if(i == 0) World::instance()->addCircle(point(pts[i][j].real(), pts[i][j].imag()), 2);
			}
		}
		vector<vector<comp> > tr1, tr2, v;
		tr1.resize(2);
		tr2.resize(2);
		vector<comp> z = pts[0], w = pts[1];
		
		tr1[0].push_back(z[1] - z[2]);
		tr1[0].push_back(-z[0] * (z[1] - z[2]));
		tr1[1].push_back(z[1] - z[0]);
		tr1[1].push_back(-z[2] * (z[1] - z[0]));
		
		tr2[0].push_back(-w[2] * (w[1] - w[0]));
		tr2[0].push_back(w[0] * (w[1] - w[2]));
		tr2[1].push_back(-w[1] + w[0]);
		tr2[1].push_back(w[1] - w[2]);
		
		v = matrixMul(tr1, tr2);
//		World::instance()->addTransform(tr1[0][0], tr1[0][1], tr1[1][0], tr1[1][1]);
		vector<comp> fp = World::instance()->addTransform(v[0][0], v[0][1], v[1][0], v[1][1]);

	} else if(name == "cleartransform") {
		World::instance()->clearTransform();
		
	} else if(name == "toggle") {
		World::instance()->toggleTurtle();
	}

	return LogoData();
}

LogoData Interpreter::runCode(string functionName, int firstToken, int lastToken, vector<LogoData> parameters) {
//	if(myFunctions.find(name) != myFunctions.end()) return runMyFunction(name, firstToken, parameters);
//	int lastToken = functionFrames[name].second;
//	if(functionName == "spiro") return LogoData();
	vector<string> &code = functions[functionName].body;
	variables.push_back(map<string, LogoData>());
	cout << "Setting parameters" << endl;
	for(int i = 0; i < (int)parameters.size(); i++) {
		variables.back()[functions[functionName].args[i]] = parameters[i];
		cout << parameters[i].toString() << " ";
	} 
	cout << endl;
	
	vector<int> valuesNeeded;
	valuesNeeded.push_back(0); // guardian
	vector<int> valuesAvailable;
	valuesAvailable.push_back(0); // guardian
	
	vector<LogoData> values;
	vector<int> stack; // Keeps function / operator names
	
	cout << "Executing code " << functionName << " " << firstToken << " " << lastToken << endl;
	
//	for(vector<string>::iterator it = script.begin() + firstToken; it != script.begin() + lastToken; it++) {
	int iterator = firstToken;
	for(; iterator <= lastToken; iterator++) {
//		vector<string>::iterator it = script.begin() + iterator;
		string token = code[iterator];
		cout << "token = " << token << endl;
		while(valuesNeeded.size() > 1 && valuesAvailable.back() == valuesNeeded.back() && !isOperator(token) && token != ")") {
			executeLast(values, stack, valuesNeeded, valuesAvailable, functionName, iterator);
		}
		
		token = code[iterator];
		
		if(functions.find(token) != functions.end()) {
			stack.push_back(iterator);
			valuesAvailable.push_back(0);
			valuesNeeded.push_back(functions[token].argNum);
			if(functions[token].argNum == 0) {
				executeLast(values, stack, valuesNeeded, valuesAvailable, functionName, iterator);
			}
		} else if(isData(token)) {
			values.push_back(getData(token));
			valuesAvailable.back()++;
		} else if(isOperator(token)) {
			cout << token << " is an operator" << endl;

			while(!stack.empty() && isOperator(code[stack.back()]) && precedence[token] <= precedence[code[stack.back()]]) {
				cout << "precedence " << token << "<= precedence " << code[stack.back()] << endl;
				executeLast(values, stack, valuesNeeded, valuesAvailable, functionName, iterator);
			}
			if(!stack.empty() && ((!isOperator(code[stack.back()]) && valuesAvailable.back() == 0)
					|| (iterator > 0 && code[iterator - 1] == "("))) {
				values.push_back(LogoData("0")); // special case for leading "-"
				cout << "Pushing magic 0" << endl;
				stack.push_back(iterator);
				valuesAvailable.push_back(1);
				valuesNeeded.push_back(2);
			} else {
				stack.push_back(iterator);
				valuesAvailable.back()--;
				valuesAvailable.push_back(1);
				valuesNeeded.push_back(2);
			}
		} else if(token == "(") {
			stack.push_back(iterator);
		} else if(token == ")") {
//			cout << "token == )" << endl;
			while(code[stack.back()] != "(") {
				executeLast(values, stack, valuesNeeded, valuesAvailable, functionName, iterator);
//				cout << "stack back = " << stack.back() << endl;
			}
//			cout << "And now I should pop back" << endl;
			stack.pop_back();
		}
	}
	
	while(valuesNeeded.size() > 1 && valuesAvailable.back() == valuesNeeded.back()) {
		executeLast(values, stack, valuesNeeded, valuesAvailable, functionName, iterator);
	}
	cout << "Exit " << functionName << endl; 

	if(!returnValue.empty()) cout << "I return: " << returnValue.toString() << endl;
	
	LogoData tmp = returnValue;
	returnValue = LogoData();
	variables.pop_back();
	return tmp;
}
