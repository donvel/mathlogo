#include "LogoData.h"

LogoData::LogoData(const string& value, bool isFunction) {
	if(isFunction) {
		dataType = FUNCTION;
		functionID = value;
	} else {
		stringData = value;
	}
}


