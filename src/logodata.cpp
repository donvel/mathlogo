#include "logodata.h"

LogoData::LogoData() : value("undefinded") {}

LogoData::LogoData(const string& _value) : value(_value) {}

LogoData::LogoData(double num) {
	stringstream output;
	if(num - (int)num < EPS) {
		output << (int)num;
	} else {
		cout.precision(10);
		output << fixed << num;
	}
	LogoData(output.str());
}

string LogoData::toString() {
	return value;
}
bool LogoData::toBoolean() {
	return value != "false";
}

double LogoData::toDouble() {
	return strtod(value.c_str(), NULL);
}

LogoData LogoData::operator+(const LogoData& ld) const {
	double res = strtod(value.c_str(), NULL) + strtod(ld.value.c_str(), NULL);
	return LogoData(res);
}

LogoData LogoData::operator-(const LogoData& ld) const {
	double res = strtod(value.c_str(), NULL) - strtod(ld.value.c_str(), NULL);
	return LogoData(res);
}

LogoData LogoData::operator/(const LogoData& ld) const {
	double res = strtod(value.c_str(), NULL) / strtod(ld.value.c_str(), NULL);
	return LogoData(res);
}

LogoData LogoData::operator*(const LogoData& ld) const {
	double res = strtod(value.c_str(), NULL) * strtod(ld.value.c_str(), NULL);
	return LogoData(res);
}

LogoData LogoData::operator==(const LogoData& ld) const {
	if(value == ld.value) return LogoData("true");
	return LogoData("false");
}

LogoData LogoData::operator< (const LogoData& ld) const {
	if(strtod(value.c_str(), NULL) < strtod(ld.value.c_str(), NULL)) return LogoData("true");
	return LogoData("false");
}

LogoData LogoData::operator> (const LogoData& ld) const {
	if(strtod(value.c_str(), NULL) > strtod(ld.value.c_str(), NULL)) return LogoData("true");
	return LogoData("false");
}