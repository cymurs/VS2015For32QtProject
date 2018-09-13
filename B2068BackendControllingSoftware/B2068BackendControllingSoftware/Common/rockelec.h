#pragma once

struct st_FrameData;

/****************************************************************************************
������ ��С
****************************************************************************************/
#define MAX_SIZE_OF_SAVE_BUF        (1024*8)
#define MAX_SIZE_OF_TEMP_BUF        (1024*8)
#define MAX_SIZE_OF_RECV_BUF        (1024*4)
#define MAX_SIZE_OF_SEND_BUF        (1024*4)
#define MAX_SIZE_OF_SHOW_BUF        (1024*5*2)


/****************************************************************************************/
typedef void(*FRAMEHANDLEFUNC)(const st_FrameData* pFrameData);            // ֡���ݴ�����


/****************************************************************************************
֡�ṹ
****************************************************************************************/
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
		m_pFrameDataBuf = nullptr;
		m_iFrameDataLength = 0;
		m_bDataIsOK = false;
		m_bDataIs16Hex = false;
	}	

	st_FrameData(const st_FrameData &other) {
		m_chSourceAddr = other.m_chSourceAddr;
		m_chSourcePort = other.m_chSourcePort;
		m_chSourceUse = other.m_chSourceUse;
		m_chTargetAddr = other.m_chTargetAddr;
		m_chTargetPort = other.m_chTargetPort;
		m_chTargetUse = other.m_chTargetUse;
		m_chFrameNo = other.m_chFrameNo;
		m_chFrameCmd = other.m_chFrameCmd;
		m_bDataIsOK = other.m_bDataIsOK;
		m_bDataIs16Hex = other.m_bDataIs16Hex;
		m_iFrameDataLength = other.m_iFrameDataLength;
		if (nullptr == m_pFrameDataBuf) {
			m_pFrameDataBuf = new char[MAX_SIZE_OF_RECV_BUF]();
		}
		memcpy(m_pFrameDataBuf, other.m_pFrameDataBuf, other.m_iFrameDataLength);
		m_pFrameDataBuf[m_iFrameDataLength] = '\0';
	}

	st_FrameData& operator=(const st_FrameData &other) {
		if (this == &other)
			return *this;

		m_chSourceAddr = other.m_chSourceAddr;
		m_chSourcePort = other.m_chSourcePort;
		m_chSourceUse = other.m_chSourceUse;
		m_chTargetAddr = other.m_chTargetAddr;
		m_chTargetPort = other.m_chTargetPort;
		m_chTargetUse = other.m_chTargetUse;
		m_chFrameNo = other.m_chFrameNo;
		m_chFrameCmd = other.m_chFrameCmd;
		m_bDataIsOK = other.m_bDataIsOK;
		m_bDataIs16Hex = other.m_bDataIs16Hex;
		m_iFrameDataLength = other.m_iFrameDataLength;
		if (nullptr == m_pFrameDataBuf) {
			m_pFrameDataBuf = new char[MAX_SIZE_OF_RECV_BUF]();
		}
		memcpy(m_pFrameDataBuf, other.m_pFrameDataBuf, other.m_iFrameDataLength);
		m_pFrameDataBuf[m_iFrameDataLength] = '\0';

		return *this;
	}

	virtual ~st_FrameData() {
		if (nullptr != m_pFrameDataBuf)
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

	char* m_pFrameDataBuf;                 // ֡���ݺͳ���
	int m_iFrameDataLength;

	bool m_bDataIsOK;					      // �����Ƿ������ȷ
	bool m_bDataIs16Hex;                    // �����Ƿ���16����
};