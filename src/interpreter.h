#pragma once

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <set>
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
    void runScript();
    
    vector<map<string, LogoData> > variables;
    set<string> operators;
    set<string> keywords;
    set<string> functions;
    
    bool isData();
    LogoData getData();
    
    

public:
    void loadScript(char* filename);
    void execute();
    static Interpreter* instance();
    void toggleRunning();

};
