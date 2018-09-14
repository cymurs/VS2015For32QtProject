#include "stdafx.h"
#include "../Common/rockeleccomm.h"

RockelecComm::RockelecComm(QObject *parent)
	: QObject(parent)
	, m_proto(new RockelecProtocol())
{
	m_pSendBuf = new unsigned char[MAX_SIZE_OF_SEND_BUF];
	m_pRecvBuf = new unsigned char[MAX_SIZE_OF_RECV_BUF];
	m_pParseBuf = new unsigned char[MAX_SIZE_OF_SAVE_BUF];
	m_iSendLength = 0;
	m_iParseLength = 0;

	m_currentIsCom = true;
	m_com = nullptr;
	m_udp = nullptr;
	m_iComBaud = QSerialPort::UnknownBaud;
	m_iComDataBits = QSerialPort::UnknownDataBits;
	m_iComStopBits = QSerialPort::UnknownStopBits;
	m_iComParity = QSerialPort::UnknownParity;
	m_serverPort = 0;
}

RockelecComm::~RockelecComm()
{
	if (nullptr != m_proto)
		delete m_proto;
}

void RockelecComm::SetSourceAddr(int iAddr, int iPort, int iResv)
{
	m_proto->SetSourceAddr(iAddr, iPort, iResv);
}

void RockelecComm::SetTargetAddr(int iAddr, int iPort, int iResv)
{
	m_proto->SetTargetAddr(iAddr, iPort, iResv);
}

void RockelecComm::SetCommand(int iFrameNo, int iCmd)
{
	m_proto->SetCommand(iFrameNo, iCmd);
}

void RockelecComm::SetDataHasFrame(bool bHasFrame)
{
	m_proto->SetDataHasFrame(bHasFrame);
}

void RockelecComm::SetSendDataIs16Hex(bool bSendIs16Hex)
{
	m_proto->SetSendDataIs16Hex(bSendIs16Hex);
}

void RockelecComm::SetRecvDataIs16Hex(bool bRecvIs16Hex)
{
	m_proto->SetRecvDataIs16Hex(bRecvIs16Hex);
}

void RockelecComm::SetFrameHandleFunc(FRAMEHANDLEFUNC pFrameHandleFunc)
{
	m_proto->SetFrameHandleFunc(pFrameHandleFunc);
}

void RockelecComm::SetComPort(char* pszComName, int iComBaud, int iComDataBits, int iComStopBits, int iComParity)
{
	m_comName = pszComName;
	m_iComBaud = iComBaud;
	m_iComDataBits = iComDataBits;
	m_iComStopBits = iComStopBits;
	m_iComParity = iComParity;
}

bool RockelecComm::OpenComPort()
{
	if (nullptr != m_com)
		CloseComPort();

	if (nullptr == m_com)
		m_com = new QSerialPort;

	m_com->setPortName(m_comName);
	m_com->setBaudRate(m_iComBaud);
	m_com->setDataBits((QSerialPort::DataBits)m_iComDataBits);
	m_com->setStopBits((QSerialPort::StopBits)m_iComStopBits);
	m_com->setParity((QSerialPort::Parity)m_iComParity);

	if (!m_com->isOpen()) {
		if (!m_com->open(QIODevice::ReadWrite)) {
			return false;
		}
	}

	connect(m_com, &QSerialPort::readyRead, this, &RockelecComm::ReceiveComNetData);

	m_currentIsCom = true;

	return true;
}

void RockelecComm::CloseComPort()
{
	if (m_com->isOpen()) {
		disconnect(m_com, &QSerialPort::readyRead, this, &RockelecComm::ReceiveComNetData);
		m_com->clear();
		m_com->close();
	}		
}

void RockelecComm::SetNetPort(char* pszIPAddr, ushort port)
{
	m_serverAddr = pszIPAddr;
	m_serverPort = port;
}

bool RockelecComm::OpenNetPort()
{
	CloseNetPort();

	if (nullptr == m_udp) {
		m_udp = new QUdpSocket;
	}

	m_udp->bind(45454, QAbstractSocket::ShareAddress);
	connect(m_udp, &QUdpSocket::readyRead, this, &RockelecComm::ReceiveComNetData);

	m_currentIsCom = false;

	return true;
}

void RockelecComm::CloseNetPort()
{
	if (nullptr != m_udp) {
		disconnect(m_udp, &QUdpSocket::readyRead, this, &RockelecComm::ReceiveComNetData);
		m_udp->close();
	}
}

void RockelecComm::ReceiveComNetData()
{
	int rcvd = 0;
	if (m_currentIsCom) {
		QByteArray recvData = m_com->readAll();
		if (!recvData.length())  return;
		rcvd = recvData.length();
		memcpy(m_pRecvBuf, recvData.data(), rcvd);
		m_pRecvBuf[rcvd] = '\0';
	}
	else {
		if (m_udp->hasPendingDatagrams()) {
			rcvd = m_udp->readDatagram((char *)m_pRecvBuf, MAX_SIZE_OF_RECV_BUF);
			if (-1 == rcvd) return;
		}
		else {
			return;
		}
	}

	// 避免缓存区溢出
	if ((m_iParseLength + rcvd) >= MAX_SIZE_OF_SAVE_BUF) {
		m_iParseLength -= rcvd + 1;
		if (m_iParseLength < 0) m_iParseLength = 0;
	}
	memcpy(m_pParseBuf + m_iParseLength, m_pRecvBuf, rcvd);
	m_iParseLength += rcvd;

	// 对读取数据进行解析处理
	m_proto->ParseFrames(m_pParseBuf, m_iParseLength);
}

void RockelecComm::SendComNetData(unsigned char chCmd, const char* pszBuf, int iBufSize)
{	
	m_iSendLength = 0;
	m_proto->PackOneFrame(chCmd, pszBuf, iBufSize, (char *)m_pSendBuf, MAX_SIZE_OF_SEND_BUF, m_iSendLength);

	SendData();
}

void RockelecComm::ResendComNetData()
{
	SendData();
}

void RockelecComm::SendData()
{
	int sent = 0;
	if (m_currentIsCom) {
		sent = m_com->write((const char *)m_pSendBuf, m_iSendLength);
	}
	else {
		sent = m_udp->writeDatagram((const char *)m_pSendBuf, m_iSendLength, QHostAddress(m_serverAddr), m_serverPort);
	}
}
