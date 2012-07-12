#include "logodata.h"

LogoData::LogoData() : stringData("undefinded") {}

LogoData::LogoData(const string& value) : stringData(value) {}

string LogoData::toString() {
	return stringData;
}
string LogoData::toBoolean() {
	return intData != 0;
}
