#include "TimeConvert.h"

structTime	TimeStampToTime(uint32_t TimeStamp, bool hasTimezone)
{
	structTime	Time;
	struct tm 	*tm_time;
	time_t      t;
	
	t = TimeStamp;
	if (hasTimezone)
		tm_time = localtime(&t);  // 带时区
	else
		tm_time = gmtime(&t);    // 不带时区
	Time.Year   = tm_time->tm_year+1900;
	Time.Month  = tm_time->tm_mon+1;
	Time.Date   = tm_time->tm_mday;
	Time.Hour   = tm_time->tm_hour;
	Time.Minute = tm_time->tm_min;
	Time.Second = tm_time->tm_sec;
	//Time.Day    = tm_time->tm_yday+1;
	
	return(Time);
}

uint32_t TimeToTimeStamp(structTime TIME)
{
	time_t 		timestamp;
	struct tm 	cal_time;
	uint32_t 	ret_timestamp;

	cal_time.tm_year = TIME.Year-1900;
	cal_time.tm_mon  = TIME.Month-1;
	cal_time.tm_mday = TIME.Date;
	cal_time.tm_hour = TIME.Hour;
	cal_time.tm_min  = TIME.Minute;
	cal_time.tm_sec  = TIME.Second;
	cal_time.tm_isdst= 0;
	timestamp = mktime(&cal_time);
	ret_timestamp = (uint32_t)timestamp;

	return ret_timestamp;
}
