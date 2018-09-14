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

	// ����Դ��ַ
	virtual void SetSourceAddr(int iAddr, int iPort, int iResv);

	// ����Ŀ�ĵ�ַ
	virtual void SetTargetAddr(int iAddr, int iPort, int iResv);

	// ����������
	virtual void SetCommand(int iFrameNo, int iCmd);

	// �շ������Ƿ���֡�ṹ
	virtual void SetDataHasFrame(bool bHasFrame);

	// �շ������Ƿ���16����
	virtual void SetSendDataIs16Hex(bool bSendIs16Hex);
	virtual void SetRecvDataIs16Hex(bool bRecvIs16Hex);

	// ����֡���ݴ�����
	virtual void SetFrameHandleFunc(FRAMEHANDLEFUNC pFrameHandleFunc);

	// ���ô���
	void SetComPort(char* pszComName, int iComBaud, int iComDataBits, int iComStopBits, int iComParity);
	// �򿪴���
	bool OpenComPort();
	// �رմ���
	void CloseComPort();

	// ��������
	void SetNetPort(char* pszIPAddr, ushort port);
	// ������
	bool OpenNetPort();
	// �ر�����
	void CloseNetPort();

	// ������/����
	void ReceiveComNetData();
	// д����/���ڡ�pszDataBuf�������������ݣ�������Դ��ַ��������Ϣ��
	//              chCommand = 0��ʹ�����������iDataLength_in = 0����ʾ���ַ����ε�����
	void SendComNetData(unsigned char chCmd, const char* pszBuf, int iBufSize);
	void ResendComNetData();

protected:
	void SendData();

private:
	RockelecProtocol *m_proto;

	// ������
	unsigned char *m_pSendBuf;
	unsigned char *m_pRecvBuf;
	unsigned char *m_pParseBuf;
	int m_iSendLength;
	int m_iParseLength;

	bool m_currentIsCom;	
	QSerialPort *m_com;
	QString m_comName;   // ���ں�
	int m_iComBaud;			// ������
	int m_iComDataBits;      // ����λ
	int m_iComStopBits;      // ֹͣλ
	int m_iComParity;          // У��λ
	QUdpSocket *m_udp;
	QString m_serverAddr;
	ushort m_serverPort;
};
