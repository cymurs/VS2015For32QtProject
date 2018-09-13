#pragma once
#include "rockelec.h"

using std::vector;
using std::string;

class RockelecProtocol
{
public:
	RockelecProtocol();
	~RockelecProtocol();

	// ����Դ��ַ
	void SetSourceAddr(int iAddr, int iPort, int iResv);

	// ����Ŀ�ĵ�ַ
	void SetTargetAddr(int iAddr, int iPort, int iResv);

	// ����������
	void SetCommand(int iFrameNo, int iCmd);

	// �շ������Ƿ���֡�ṹ
	void SetDataHasFrame(bool bHasFrame);

	// �շ������Ƿ���16����
	void SetSendDataIs16Hex(bool bSendIs16Hex);
	void SetRecvDataIs16Hex(bool bRecvIs16Hex);	

	// ����֡���ݴ�����
	void SetFrameHandleFunc(FRAMEHANDLEFUNC pFrameHandleFunc);
	// �����ȡ������
	void ParseFrames(unsigned char *pszDataBuf, int &iDataLength);
	// �����������ݷ�װΪ֡����
	void PackOneFrame(unsigned char chCmd, const char *pszData, int iDataLength, char *pszDataBuf, int iBufSize, int &iLength);

protected:
	// ��charת��Ϊ16�����ַ���, ���Ҽ���һ���ո�
	void CharToHexString(int iData, unsigned char *pszData, int iLength);
	// ��16�����ַ���ת��Ϊchar
	void HexStringToChar(const unsigned char *pszData, int &iData);

	vector<string> SplitHexString(const char *pszData, int iDataLength, const char *pSplitter);

	// ����CRC
	unsigned short GetCRC16(unsigned char *cp, unsigned int leng);

private:
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

	bool m_bDataHasFrame;
	bool m_bSendDataIs16Hex;
	bool m_bRecvDataIs16Hex;
	bool m_bShowDebug;

	FRAMEHANDLEFUNC m_pFrameHandleFunc;

	int m_iSaveIndex_Start;
	int m_iSaveIndex_End;
	unsigned char* m_pCheckBuf;
	unsigned char* m_pDataUseBuf;
	unsigned char* m_pTempBuf;
	unsigned char m_chOne[64];
	char *m_pShowBuf;
};

