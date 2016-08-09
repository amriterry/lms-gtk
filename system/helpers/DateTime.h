#ifndef _DATE_TIME_H_
#define _DATE_TIME_H_

#include <string>
#include <ctime>
#include "DateTimeParams.h"

using namespace std;

namespace tuber{

class DateTime{
public:
	//DateTime(int year = 1990,int month = 01,int day = 01,int hour = 00,int minute = 00,int seconds = 00);
	static tm* getRawTimeInfo();
	static tm getRawTimeFromString(string timestamp,string format = DEFAULT_FORMAT);
	static double getTimeDiff(DateTimeParams diffIn,string firsttime,string secondtime = "",string format = DEFAULT_FORMAT);
	static double getTimeDiff(tm& firsttime,tm& secondtime);
	static string getCurrentDateTime();
	static string getFutureTime(int timeToAdd,DateTimeParams addType);
	static string parseToString(tm* timeinfo,string format = DEFAULT_FORMAT);

	constexpr static char* DEFAULT_FORMAT = "%Y-%m-%d %H:%M:%S";
};

}

#endif