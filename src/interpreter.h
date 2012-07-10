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
#include <ofMain.h>

using namespace std;

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
	
//    bool isData();
//    LogoData getData();
//    LogoData runFunction(int firstToken, int lastToken, vector<LogoData> parameters = vector<LogoData>(), bool setParameters = false);
//    LogoData runMyFunction(string name, vector<LogoData> parameters, int &iterator);
//
//    void executeLast(vector<LogoData> &values, vector<string> &stack, vector<int> valuesNeeded, vector<int> valuesAvailable, int &iterator);
//    void initiateParameters(int &firstToken, vector<LogoData> parameters);
//
//    vector<map<string, LogoData> > variables;
//    set<string> operators;
//    set<string> keywords;
//    map<string, int> functions; // Number of values needed
//    map<string, int> myFunctions;
//    map<string, int> precedence;
//    map<string, pair<int, int> > functionFrames;
//    map<pair<int, int>, pair<int, int> > blockFrames;
    
   

public:
    void loadScript(char* filename);
    void execute();
    static Interpreter* instance();
    void toggleRunning();

};
