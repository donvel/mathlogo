#include "logodata.h"

LogoData::LogoData() : valid(false) {}

bool LogoData::empty() {
	return !valid;
}

LogoData::LogoData(const string& _value) : valid(true), value(_value) {}

LogoData::LogoData(double num) : valid(true) {
	stringstream output;
	if(abs(num - round(num)) < EPS) {
		output << (int)round(num);
	} else {
		cout.precision(10);
		output << fixed << num;
	}
	value = output.str();
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
	cout << res << endl;
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

LogoData LogoData::operator%(const LogoData& ld) const {
	int res = (int)round(strtod(value.c_str(), NULL)) % (int)round(strtod(ld.value.c_str(), NULL));
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