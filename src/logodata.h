#pragma once
#include <string>
#include <cstdlib>
#include <sstream>

#include "geometry.h"

using namespace std;

class LogoData {
public:
	LogoData();
	LogoData(const string& _value);
	LogoData(double num);
//	~LogoData();
	double toDouble();
	string toString();
	bool toBoolean();
	LogoData operator< (const LogoData &ld) const;
	LogoData operator> (const LogoData &ld) const;
	LogoData operator+ (const LogoData &ld) const;
	LogoData operator- (const LogoData &ld) const;
	LogoData operator* (const LogoData &ld) const;
	LogoData operator/ (const LogoData &ld) const;
	LogoData operator== (const LogoData &ld) const;
private:
    string value;
};


