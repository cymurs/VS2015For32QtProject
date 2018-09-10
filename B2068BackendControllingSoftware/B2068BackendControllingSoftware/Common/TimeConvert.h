#pragma once

#include "stdint.h"
#include "time.h"

typedef struct{
	unsigned char	Second;
	unsigned char	Minute;
	unsigned char	Hour;
	unsigned char	Month;
	unsigned char	Date;//日
	//unsigned short Day;//一年的第几天
	unsigned short Year;
}structTime;

structTime TimeStampToTime(uint32_t TimeStamp);//整形转年月日
uint32_t TimeToTimeStamp(structTime TIME);//年月日转整形
