#include "Libase.h"

CMyComBasic::CMyComBasic()
{

}
CMyComBasic::~CMyComBasic()
{

}

// ��ȡ�汾�ţ�������
// 2015-06-26: "����CRC ��һ��" ������ʾ��Ϣǰ������ \n
void CMyComBasic::GetDllLibVerAndDate(int& iLargeVer, int& iSmallVer, int& iYear, int& iMonth, int& iDay)
{
    iLargeVer = 1;
    iSmallVer = 5;

    iYear = 2015;
    iMonth = 8;
    iDay = 7;
}


