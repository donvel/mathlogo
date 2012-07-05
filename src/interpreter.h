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
#include "LogoData.h"
#include <ofMain.h>

using namespace std;

class Interpreter {
private:
    Interpreter(); // Interpreter is a singleton
    static Interpreter *interpreterInstance;
    
    bool running;
    fstream scriptFile;
    vector<string> script;
    
   
    void parseScriptFile();
    void randomMoves();
    bool isData();
    LogoData getData();
    LogoData runFunction(string name, vector<LogoData> parameters = vector<LogoData>(), bool setParameters = false);
    LogoData runMyFunction(string name, int &fisrtToken, vector<LogoData> parameters = vector<LogoData>());
    void executeLast(vector<LogoData> &values, vector<string> &stack, vector<int> valuesNeeded, vector<int> valuesAvailable);
    void initiateParameters(int &FirstToken, vector<LogoData> parameters);
    
    vector<map<string, LogoData> > variables;
    set<string> operators;
    set<string> keywords;
    map<string, int> functions; // Number of values needed
    map<string, int> myFunctions;
    map<string, int> precedence;
    map<string, pair<int, int> > functionFrames;
    map<pair<int, int>, pair<int, int> > blockFrames;
    


    
    

public:
    void loadScript(char* filename);
    void execute();
    static Interpreter* instance();
    void toggleRunning();

};
