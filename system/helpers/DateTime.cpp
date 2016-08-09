#include <system/helpers/DateTime.h>

#include <vendor/support/strptime.h>

using namespace std;

namespace tuber{

tm* DateTime::getRawTimeInfo(){
	time_t rawtime;

    time (&rawtime);
    return localtime (&rawtime);
}

tm DateTime::getRawTimeFromString(string timestamp,string format){
    tm timeinfo;
	strptime(timestamp.c_str(),format.c_str(),&timeinfo);
	mktime(&timeinfo);
	return timeinfo;
}

double DateTime::getTimeDiff(DateTimeParams diffIn,string firsttime,string secondtime,string format){
	double diff;
	tm firstTimeInfo = getRawTimeFromString(firsttime,format);
	tm secondTimeInfo;
	if(secondtime == ""){
		secondTimeInfo = (*getRawTimeInfo());
	} else {
		secondTimeInfo = getRawTimeFromString(secondtime,format);
	}
	diff = getTimeDiff(firstTimeInfo,secondTimeInfo);

	switch(diffIn){
		case DateTimeParams::YEAR:
			diff /= (60.0 * 60.0 * 24 * 365);
			break;
		case DateTimeParams::MONTH:
			diff /= (60.0 * 60.0 * 24 * 30);
			break;
		case DateTimeParams::DAY:
			diff /= (60.0 * 60.0 * 24);
			break;
		case DateTimeParams::HOUR:
			diff /= 3600.0;
			break;
		case DateTimeParams::MINUTE:
			diff /= 60.0;
			break;
		case DateTimeParams::SECOND:
			break;
		default:
			break;
	}

	return diff;
}
double DateTime::getTimeDiff(tm& firsttime,tm& secondtime){
	return difftime(mktime(&firsttime),mktime(&secondtime));
}

string DateTime::getCurrentDateTime(){
	return parseToString(getRawTimeInfo());
}

string DateTime::getFutureTime(int timeToAdd,DateTimeParams addType){
	tm* timeInfo = getRawTimeInfo();

	switch(addType){
		case DateTimeParams::YEAR:
			timeInfo->tm_year += timeToAdd;
			break;
		case DateTimeParams::MONTH:
			timeInfo->tm_mon += timeToAdd;
			break;
		case DateTimeParams::DAY:
			timeInfo->tm_mday += timeToAdd;
			break;
		case DateTimeParams::HOUR:
			timeInfo->tm_hour += timeToAdd;
			break;
		case DateTimeParams::MINUTE:
			timeInfo->tm_min += timeToAdd;
			break;
		case DateTimeParams::SECOND:
			timeInfo->tm_sec += timeToAdd;
			break;
		default:
			break;
	}

	mktime(timeInfo);

	return parseToString(timeInfo);
}

string DateTime::parseToString(tm* timeinfo,string format){
	char buffer [80];
	strftime(buffer,80,format.c_str(),timeinfo);
	return string(buffer);
}

}
