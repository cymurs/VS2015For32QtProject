#pragma once

#include <QObject>
#include <QUdpSocket>
#include "RockelecProtocol.h"


class RockelecComm : public QObject
{
	Q_OBJECT

public:
	RockelecComm(QObject *parent);
	virtual ~RockelecComm();

	// 设置源地址
	virtual void SetSourceAddr(int iAddr, int iPort, int iResv);

	// 设置目的地址
	virtual void SetTargetAddr(int iAddr, int iPort, int iResv);

	// 设置命令字
	virtual void SetCommand(int iFrameNo, int iCmd);

	// 收发数据是否有帧结构
	virtual void SetDataHasFrame(bool bHasFrame);

	// 收发数据是否是16进制
	virtual void SetSendDataIs16Hex(bool bSendIs16Hex);
	virtual void SetRecvDataIs16Hex(bool bRecvIs16Hex);

	// 设置帧数据处理函数
	virtual void SetFrameHandleFunc(FRAMEHANDLEFUNC pFrameHandleFunc);

	// 配置串口
	void SetComPort(char* pszComName, int iComBaud, int iComDataBits, int iComStopBits, int iComParity);
	// 打开串口
	bool OpenComPort();
	// 关闭串口
	void CloseComPort();

	// 配置网口
	void SetNetPort(char* pszIPAddr, ushort port);
	// 打开网口
	bool OpenNetPort();
	// 关闭网口
	void CloseNetPort();

	// 读串口/网口
	void ReceiveComNetData();
	// 写串口/网口。pszDataBuf仅仅是数据内容，不包括源地址等其他信息；
	//              chCommand = 0，使用输入框的命令；iDataLength_in = 0，表示是字符串形的数据
	void SendComNetData(unsigned char chCmd, const char* pszBuf, int iBufSize);
	void ResendComNetData();

protected:
	void SendData();

private:
	RockelecProtocol *m_proto;

	// 缓冲区
	unsigned char *m_pSendBuf;
	unsigned char *m_pRecvBuf;
	unsigned char *m_pParseBuf;
	int m_iSendLength;
	int m_iParseLength;

	bool m_currentIsCom;	
	QSerialPort *m_com;
	QString m_comName;   // 串口号
	int m_iComBaud;			// 波特率
	int m_iComDataBits;      // 数据位
	int m_iComStopBits;      // 停止位
	int m_iComParity;          // 校验位
	QUdpSocket *m_udp;
	QString m_serverAddr;
	ushort m_serverPort;
};
