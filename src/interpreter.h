#pragma once

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <boost/thread.hpp>

#include "world.h"
#include "logodata.h"
#include "ofMain.h"

using namespace std;

struct Function {
	string name;
	int argNum;
	bool builtin;
	vector<string> args;
	vector<string> body;
	map<pair<int, int>, pair<int, int> > blockFrames; // if, when, etc.
	void parseBody();

	Function(int _argNum = 0, bool _builtin = true, vector<string> _args = vector<string>(), 
		vector<string> _body = vector<string>());
};

class Interpreter {
private:
    Interpreter(); // Interpreter is a singleton
    static Interpreter *interpreterInstance;
    
    bool running;
	bool useBareCommands;
    fstream scriptFile;
    vector<string> script;
   
    void parseScriptFile();
    void randomMoves();
	void runCommands();
	
	void lowerCase(string &token);
	void extend(string &token);
	
    bool isData(string token);
	bool isOperator(string token);
	bool isBracket(string token);
    LogoData getData(string token);
    LogoData runCode(string functionName, int firstToken, int lastToken, vector<LogoData> parameters = vector<LogoData>());
    LogoData runMyFunction(int namePos, vector<LogoData> parameters, string functionName, int &iterator);

    void executeLast(vector<LogoData> &values, vector<int> &stack, vector<int> &valuesNeeded, 
		vector<int> &valuesAvailable, string functionName, int &iterator);
    void initiateParameters(int &firstToken, vector<LogoData> parameters);

    vector<map<string, LogoData> > variables;
 //   set<string> keywords;
    map<string, Function> functions;
    map<string, int> myFunctions;
    map<string, int> precedence;
    
   
public:
	static Interpreter* instance();
    void loadScript(char* filename);
    void execute();
    void toggleRunning();

};
