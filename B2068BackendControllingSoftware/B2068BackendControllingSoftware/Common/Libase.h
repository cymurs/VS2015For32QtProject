#ifndef _LIN_LIBASE_H_
#define _LIN_LIBASE_H_

#include <memory.h>

#if defined MCB_EXPORT
	#define MCB_API __declspec(dllexport)
#else
	#define MCB_API __declspec(dllimport)
#endif

/****************************************************************************************/
// ֡����
struct st_FrameData
{
    st_FrameData()
    {
        m_chSourceAddr = 0;
        m_chSourcePort = 0;
        m_chSourceUse = 0;
        m_chTargetAddr = 0;
        m_chTargetPort = 0;
        m_chTargetUse = 0;
        m_chFrameNo = 0;
        m_chFrameCmd = 0;
        m_pFrameDataBuf = 0;
        m_iFrameDataLength = 0;
        m_bDataIsOK = false;
        m_bDataIs16Hex = false;
    }

    void Clone(st_FrameData* pOther)
    {
        pOther->m_chSourceAddr = m_chSourceAddr;
        pOther->m_chSourcePort = m_chSourcePort;
        pOther->m_chSourceUse = m_chSourceUse;
        pOther->m_chTargetAddr = m_chTargetAddr;
        pOther->m_chTargetPort = m_chTargetPort;
        pOther->m_chTargetUse = m_chTargetUse;
        pOther->m_chFrameNo = m_chFrameNo;
        pOther->m_chFrameCmd = m_chFrameCmd;
        //m_pFrameDataBuf = 0;
        pOther->m_iFrameDataLength = m_iFrameDataLength;
        pOther->m_bDataIsOK = m_bDataIsOK;
        pOther->m_bDataIs16Hex = m_bDataIs16Hex;
        memcpy((void*)(pOther->m_pFrameDataBuf), m_pFrameDataBuf, m_iFrameDataLength);
        (pOther->m_pFrameDataBuf)[m_iFrameDataLength] = '\0';
    }

	void Clone(st_FrameData* pOther) const {
		pOther->m_chSourceAddr = m_chSourceAddr;
		pOther->m_chSourcePort = m_chSourcePort;
		pOther->m_chSourceUse = m_chSourceUse;
		pOther->m_chTargetAddr = m_chTargetAddr;
		pOther->m_chTargetPort = m_chTargetPort;
		pOther->m_chTargetUse = m_chTargetUse;
		pOther->m_chFrameNo = m_chFrameNo;
		pOther->m_chFrameCmd = m_chFrameCmd;
		//m_pFrameDataBuf = 0;
		pOther->m_iFrameDataLength = m_iFrameDataLength;
		pOther->m_bDataIsOK = m_bDataIsOK;
		pOther->m_bDataIs16Hex = m_bDataIs16Hex;
		memcpy((void*)(pOther->m_pFrameDataBuf), m_pFrameDataBuf, m_iFrameDataLength);
		(pOther->m_pFrameDataBuf)[m_iFrameDataLength] = '\0';
	}

	st_FrameData& operator=(const st_FrameData &other) {
		if (this == &other)
			return *this;
		
		if (0 == m_pFrameDataBuf) {
			m_pFrameDataBuf = new char[1024*4]();
		}
		other.Clone(this);

		return *this;
	}

	virtual ~st_FrameData() {
		if (0 != m_pFrameDataBuf)
			delete[] m_pFrameDataBuf;
	}

    unsigned char m_chSourceAddr;       // Դ ��ַ���˿ںͱ���
    unsigned char m_chSourcePort;
    unsigned char m_chSourceUse;

    unsigned char m_chTargetAddr;       // Ŀ�� ��ַ���˿ںͱ���
    unsigned char m_chTargetPort;
    unsigned char m_chTargetUse;

    unsigned char m_chFrameNo;          // ֡�ź�����
    unsigned char m_chFrameCmd;

    char* m_pFrameDataBuf;              // ֡���ݺͳ���
    int m_iFrameDataLength;

    bool m_bDataIsOK;                   // �����Ƿ������ȷ
    bool m_bDataIs16Hex;                // �����Ƿ���16����
};


/****************************************************************************************/
typedef void (*SHOWRECVDATAFUNC)(unsigned char chAddr, unsigned char chPort, const char* pszShowData, bool, int iR, int iG, int iB);  // ��ʾ��������
typedef void (*SETRECVSHOWFUNC)(int iShowType);                             // ��ʾ���յĸ�ʽ����
typedef void (*SHOWSENDDATAFUNC)(unsigned char chAddr, unsigned char chPort, const char* pszShowData, int iLength);     // ��ʾ��������

typedef void (*DOWNLOADTIMEOUTFUNC)();                                      // �������س�ʱ���ܺ���
typedef void (*FRAMEHANDLEFUNC)(const st_FrameData* pFrameData);            // ֡���ݴ�����

/****************************************************************************************
    ��̬��ͷ�ļ�
*****************************************************************************************/
class MCB_API/*_declspec(dllexport)*/ CMyComBasic
{
public:
    CMyComBasic();
    virtual ~CMyComBasic();

public:
    // ��ȡ�汾�ţ�������
    void GetDllLibVerAndDate(int& iLargeVer, int& iSmallVer, int& iYear, int& iMonth, int& iDay);

public:
    // ��ǰʹ�����ڻ��Ǵ���
    virtual void SetCurrentIsCom(bool bIsCom) = 0;

    // �շ������Ƿ���16����
    virtual void SetSendDataIs16Hex(bool bSendIs16Hex) = 0;
    virtual void SetRecvDataIs16Hex(bool bRecvIs16Hex) = 0;

    // �շ������Ƿ���֡�ṹ
    virtual void SetDataHasFrame(bool bHasFrame) = 0;

    // �Ƿ���ʾ������Ϣ
    virtual void SetShowDebug(bool bDebug) = 0;

    // �Ƿ�������е����ݰ�
    virtual void SetRecvAllData(bool bRecvAll) = 0;

    // ����Դ��ַ
    virtual void SetSourceAddr(int iAddr, int iPort, int iResv) = 0;

    // ����Ŀ�ĵ�ַ
    virtual void SetTargetAddr(int iAddr, int iPort, int iResv) = 0;

    // ����������
    virtual void SetCommand(int iFrameNo, int iCmd) = 0;

    // ������ʾ���հ��ĺ���
    virtual void SetShowRecvDataFunc(SHOWRECVDATAFUNC pShowFunc) = 0;
    virtual void SetRecvShowFunc(SETRECVSHOWFUNC pSetShowFunc) = 0;

    // ������ʾ���Ͱ��ĺ���
    virtual void SetShowSendDataFunc(SHOWSENDDATAFUNC pShowFunc) = 0;

    // �����������س�ʱ���ܺ���
    virtual void SetDownloadTimeoutFunc(DOWNLOADTIMEOUTFUNC pDownloadTimeoutFunc) = 0;

    // ����֡���ݴ�����
    virtual void SetFrameHandleFunc(FRAMEHANDLEFUNC pFrameHandleFunc) = 0;

public:                     // ����
    // �رմ���
    virtual void CloseComPort() = 0;

    // ���ô���
    // pszComName = "COM1", "COM2", ...
    // iComBaud = CBR_9600, CBR_14400, ...
    // iComDataBits = 5, 6, 7, 8
    // iComStopBits = ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
    // iComParity = NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY
    virtual void SetComPort(char* pszComName, int iComBaud, int iComDataBits, int iComStopBits, int iComParity) = 0;

    // �򿪴���
    virtual bool OpenComPort() = 0;

    virtual bool TaskIsRunning() = 0;

public:                     // ����
    // �ر�����
    virtual void CloseNetPort() = 0;

    // ��������
    virtual void SetNetPort(char* pszIPAddr, int iPort) = 0;

    // ������
    virtual bool OpenNetPort() = 0;

    // д����/���ڡ�pszDataBuf�������������ݣ�������Դ��ַ��������Ϣ��
    //              chCommand = 0��ʹ�����������iDataLength_in = 0����ʾ���ַ����ε�����
    virtual void SendComNetData(unsigned char chCommand, const char* pszDataBuf, int iDataLength_in) = 0;

    // �ط���ǰ֡
    virtual void ReSendCurComNetData() = 0;

};

/****************************************************************************************
    ȫ�ֺ���
*****************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

    // ʹ�����������������ָ���ɵ������ͷ�
	MCB_API/*_declspec(dllexport)*/ CMyComBasic* CreateComBasic();

#ifdef __cplusplus
}
#endif


#endif
