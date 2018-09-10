#pragma once

#include "stdint.h"
#include "time.h"

typedef struct{
	unsigned char	Second;
	unsigned char	Minute;
	unsigned char	Hour;
	unsigned char	Month;
	unsigned char	Date;//��
	//unsigned short Day;//һ��ĵڼ���
	unsigned short Year;
}structTime;

structTime TimeStampToTime(uint32_t TimeStamp);//����ת������
uint32_t TimeToTimeStamp(structTime TIME);//������ת����
