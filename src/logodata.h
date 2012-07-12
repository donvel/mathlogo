#pragma once
#include <string>

using namespace std;

class LogoData {
public:
	LogoData();
	LogoData(const string& value);
	~LogoData();
	long double toDouble();
	string toString();
	bool toBoolean();
	LogoData operator+ (const LogoData &ld) const;
	LogoData operator- (const LogoData &ld) const;
	LogoData operator* (const LogoData &ld) const;
	LogoData operator/ (const LogoData &ld) const;
	LogoData operator== (const LogoData &ld) const;
private:
    enum DataType {STRING, FLOAT, INTEGER, LIST, ARRAY};
    DataType dataType;
    string stringData;
    long double floatData;
    long long intData;
};


