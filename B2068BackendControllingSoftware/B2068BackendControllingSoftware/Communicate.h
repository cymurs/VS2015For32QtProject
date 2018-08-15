#ifndef _LIN_COMMUNICATE_H_
#define _LIN_COMMUNICATE_H_

//#include <WinSock2.h>
//#include <Windows.h>
#include <QSerialPort>
#include <QUdpSocket>
#include <QMutex>
#include <string>

#include "Libase.h"

using std::string;

/***************************************************************************************/
class CMyProt;
class CMyComm;

/****************************************************************************************
    ͨ��ģ��
*****************************************************************************************/
// �̲߳���
namespace Lin_Com_Use{
typedef struct _ComWorker_Params
{
    _ComWorker_Params()
    {
        m_pComm = NULL;
        m_bComThreadExit = false;
        m_bThreadWorking = false;
    }

    CMyComm* m_pComm;                       // ָ��

    volatile bool m_bComThreadExit;         // �˳���־
    volatile bool m_bThreadWorking;         // ��������

}COMWORKER_PARAMS, *PCOMWORKER_PARAMS;
};

/***************************************************************************************/
class CMyComm : public CMyComBasic
{
public:
    CMyComm();
    ~CMyComm();

public:
    // ��ǰʹ�����ڻ��Ǵ���
    void SetCurrentIsCom(bool bIsCom);

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
    virtual void SetRecvShowFunc(SETRECVSHOWFUNC pSetShowFunc);

    // ������ʾ���Ͱ��ĺ���
    void SetShowSendDataFunc(SHOWSENDDATAFUNC pShowFunc);

    // �����������س�ʱ���ܺ���
    void SetDownloadTimeoutFunc(DOWNLOADTIMEOUTFUNC pDownloadTimeoutFunc);

    // ����֡���ݴ�����
    void SetFrameHandleFunc(FRAMEHANDLEFUNC pFrameHandleFunc);

public:                     // ����
    // �رմ���
    void CloseComPort();

    // ���ô���
    void SetComPort(char* pszComName, int iComBaud, int iComDataBits, int iComStopBits, int iComParity);

    // �򿪴���
    bool OpenComPort();

    bool TaskIsRunning();

public:                     // ����
    // �ر�����
    void CloseNetPort();

    // ��������
    void SetNetPort(char* pszIPAddr, int iPort);

    // ������
    bool OpenNetPort();

    // д����/���ڡ�pszDataBuf�������������ݣ�������Դ��ַ��������Ϣ��
    //              chCommand = 0��ʹ�����������iDataLength_in = 0����ʾ���ַ����ε�����
    void SendComNetData(unsigned char chCommand, const char* pszDataBuf, int iDataLength_in);

    // �ط���ǰ֡
    void ReSendCurComNetData();

public:
    // ������/����
    void ReadComNetData();

    // �������ع��ܳ�ʱ����
    void DownLoadTimeoutFunc();

public:
	// ��ǰ������Ϣ
	string ErrorString() const;

private:
	QSerialPort m_Com;          // ����
	QUdpSocket m_UDP;        // ����

    // ���� ����
    char m_chComName[32];   // ���ں�
    int m_iComBaud;         // ������
    int m_iComDataBits;     // ����λ
    int m_iComStopBits;     // ֹͣλ
    int m_iComParity;       // У��λ

    // ��������
    char m_chNetIPAddr[32]; // IP
    int m_iNetPort;         // �˿�

    bool m_bCurrentIsCom;
	mutable QMutex m_Mutex;
private:
    DOWNLOADTIMEOUTFUNC m_pDownloadTimeoutFunc;

private:
    Lin_Com_Use::COMWORKER_PARAMS m_ComWorkerParams;

    CMyProt* m_pMyProt;

private:    // Com ����
    unsigned char* m_pSaveBuf;
    unsigned char* m_pRecvBuf;
    unsigned char* m_pSendBuf;

    int m_iSaveLength;
    int m_iSendDataLength;

private:
	string m_strError;
};


#endif
