#ifndef _LIN_PROTOCOL_H_
#define _LIN_PROTOCOL_H_

#include "Libase.h"

/****************************************************************************************/
// Com ������ ��С
#define MAX_SIZE_OF_SAVE_BUF        (1024*8)
#define MAX_SIZE_OF_TEMP_BUF        (1024*8)
#define MAX_SIZE_OF_RECV_BUF        (1024*4)
#define MAX_SIZE_OF_SEND_BUF        (1024*4)
#define MAX_SIZE_OF_SHOW_BUF        (1024*5*2)

/****************************************************************************************
    Э��ģ��
*****************************************************************************************/
class CMyProt
{
public:
    CMyProt();
    ~CMyProt();

public:
    // ��������(��)
    void HandleData(unsigned char* pszDataBuf, int& iDataLength);

    // ��֡: chCommand = 0��ʹ�����������iDataLength_in = 0����ʾ���ַ����ε�����
    void WriteOneFrame(unsigned char chCommand, const char* pszData_in, int iDataLength_in, char* pszDataBuf, int iBufSize, int& iLength);

    // �շ������Ƿ���16����
    void SetSendDataIs16Hex(bool bSendIs16Hex);
    void SetRecvDataIs16Hex(bool bRecvIs16Hex);

    // �շ������Ƿ���֡�ṹ
    void SetDataHasFrame(bool bHasFrame);

    // �Ƿ�������е����ݰ�
    void SetRecvAllData(bool bRecvAll);

    // �Ƿ���ʾ������Ϣ
    void SetShowDebug(bool bDebug);

    // ����Դ��ַ
    void SetSourceAddr(int iAddr, int iPort, int iResv);

    // ����Ŀ�ĵ�ַ
    void SetTargetAddr(int iAddr, int iPort, int iResv);

    // ����������
    void SetCommand(int iFrameNo, int iCmd);

    // ������ʾ���հ��ĺ���
    void SetShowRecvDataFunc(SHOWRECVDATAFUNC pShowFunc);
    void SetRecvShowFunc(SETRECVSHOWFUNC pSetShowFunc);

    // ������ʾ���Ͱ��ĺ���
    void SetShowSendDataFunc(SHOWSENDDATAFUNC pShowFunc);

    // ����֡���ݴ�����
    void SetFrameHandleFunc(FRAMEHANDLEFUNC pFrameHandleFunc);
    
public:
    

private:
    // ��һ��16���Ƶ�ֵת��ASCII������һ���ո�
    void Change16ToAscii(int iData, unsigned char* pszResult, int iSize);

    unsigned short GetCRC16(unsigned char* cp,unsigned int leng);

    // �����ַ������Կո�ָ���ַ��ĸ���
    int StringNumOnSplit(const char* pszBuf);

private:
    int m_iSaveIndex_Start;
    int m_iSaveIndex_End;

    unsigned char* m_pCheckBuf;
    unsigned char* m_pDataUseBuf;
    unsigned char* m_pTempBuf;

    char* m_pShowBuf;
    char* m_pShowTempBuf;
    unsigned char m_chOne[64];

    unsigned char m_chWriteUseOne[64];

private:
    bool m_bDataHasFrame;
    bool m_bSendDataIs16Hex;
    bool m_bRecvDataIs16Hex;
    bool m_bShowDebug;
    bool m_bShowColor;
    bool m_bRecvAll;

    SHOWRECVDATAFUNC m_pShowRecvFunc;
    SETRECVSHOWFUNC m_pSetRecvShowFunc;
    SHOWSENDDATAFUNC m_pShowSendFunc;
    FRAMEHANDLEFUNC m_pFrameHandleFunc;

    // Դ��ַ
    int m_iSourceAddr;
    int m_iSourcePort;
    int m_iSourceResv;

    // Ŀ�ĵ�ַ
    int m_iTargetAddr;
    int m_iTargetPort;
    int m_iTargetResv;

    // ������
    int m_iFrameNo;
    int m_iCmd;

private:
    int m_iCountUse;        // ���ڴ���֡��СΪ270��ʱ�����ʾ��ʽ����: 0, 1, 2
};


#endif
