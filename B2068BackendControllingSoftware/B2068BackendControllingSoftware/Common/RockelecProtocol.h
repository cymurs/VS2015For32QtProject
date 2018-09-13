#pragma once
#include "rockelec.h"

using std::vector;
using std::string;

class RockelecProtocol
{
public:
	RockelecProtocol();
	~RockelecProtocol();

	// 设置源地址
	void SetSourceAddr(int iAddr, int iPort, int iResv);

	// 设置目的地址
	void SetTargetAddr(int iAddr, int iPort, int iResv);

	// 设置命令字
	void SetCommand(int iFrameNo, int iCmd);

	// 收发数据是否有帧结构
	void SetDataHasFrame(bool bHasFrame);

	// 收发数据是否是16进制
	void SetSendDataIs16Hex(bool bSendIs16Hex);
	void SetRecvDataIs16Hex(bool bRecvIs16Hex);	

	// 设置帧数据处理函数
	void SetFrameHandleFunc(FRAMEHANDLEFUNC pFrameHandleFunc);
	// 处理读取的数据
	void ParseFrames(unsigned char *pszDataBuf, int &iDataLength);
	// 将待发送数据封装为帧数据
	void PackOneFrame(unsigned char chCmd, const char *pszData, int iDataLength, char *pszDataBuf, int iBufSize, int &iLength);

protected:
	// 把char转换为16进制字符串, 并且加上一个空格
	void CharToHexString(int iData, unsigned char *pszData, int iLength);
	// 把16进制字符串转换为char
	void HexStringToChar(const unsigned char *pszData, int &iData);

	vector<string> SplitHexString(const char *pszData, int iDataLength, const char *pSplitter);

	// 计算CRC
	unsigned short GetCRC16(unsigned char *cp, unsigned int leng);

private:
	// 源地址
	int m_iSourceAddr;
	int m_iSourcePort;
	int m_iSourceResv;

	// 目的地址
	int m_iTargetAddr;
	int m_iTargetPort;
	int m_iTargetResv;

	// 命令字
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

