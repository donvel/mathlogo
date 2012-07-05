#pragma once
#include <string>

using namespace std;

class LogoData {
public:
	LogoData(const string& value, bool isFunction = false);
	~LogoData();
private:
    enum DataType {FUNCTION, STRING, FLOAT, INTEGER, LIST, ARRAY};
    DataType dataType;
    string stringData;
    long double floatData;
    long long intData;
    string functionID;
};


