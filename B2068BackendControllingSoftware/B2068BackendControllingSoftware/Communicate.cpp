#include "Communicate.h"

#include "Protocol.h"

#include <QNetworkDatagram>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include <stdio.h>      // for sprintf_s()


/****************************************************************************************/
void ComWorkerThread(void *pParam);


/****************************************************************************************
    ͨ��ģ��
*****************************************************************************************/
CMyComm::CMyComm()
{


    strcpy_s(m_chComName, sizeof(m_chComName), "COM1");
    m_iComBaud = QSerialPort::Baud9600;
    m_iComDataBits = QSerialPort::Data8;
    m_iComStopBits = QSerialPort::OneStop;
    m_iComParity = QSerialPort::NoParity;

    strcpy_s(m_chNetIPAddr, sizeof(m_chNetIPAddr), "127.0.0.1");
    m_iNetPort = 0;

    m_ComWorkerParams.m_pComm = this;

    m_pDownloadTimeoutFunc = NULL;

    // COM Buf
    m_pSaveBuf = new unsigned char[MAX_SIZE_OF_SAVE_BUF];
    m_pRecvBuf = new unsigned char[MAX_SIZE_OF_RECV_BUF];
    m_pSendBuf = new unsigned char[MAX_SIZE_OF_SEND_BUF];
    m_iSaveLength = 0;
    m_iSendDataLength = 0;

    m_bCurrentIsCom = true;

    m_pMyProt = new CMyProt();
}
CMyComm::~CMyComm()
{
    delete[] m_pSaveBuf;
    delete[] m_pRecvBuf;
    delete[] m_pSendBuf;
}

// ��ǰʹ�����ڻ��Ǵ���
void CMyComm::SetCurrentIsCom(bool bIsCom)
{
    m_bCurrentIsCom = bIsCom;
}

// �շ������Ƿ���16����
void CMyComm::SetSendDataIs16Hex(bool bSendIs16Hex)
{
    m_pMyProt->SetSendDataIs16Hex(bSendIs16Hex);
}
void CMyComm::SetRecvDataIs16Hex(bool bRecvIs16Hex)
{
    m_pMyProt->SetRecvDataIs16Hex(bRecvIs16Hex);
}

// �շ������Ƿ���֡�ṹ
void CMyComm::SetDataHasFrame(bool bHasFrame)
{
    m_pMyProt->SetDataHasFrame(bHasFrame);
}

// �Ƿ�������е����ݰ�
void CMyComm::SetRecvAllData(bool bRecvAll)
{
    m_pMyProt->SetRecvAllData(bRecvAll);
}

void CMyComm::SetShowDebug(bool bDebug)
{
    m_pMyProt->SetShowDebug(bDebug);
}

void CMyComm::SetSourceAddr(int iAddr, int iPort, int iResv)
{
    m_pMyProt->SetSourceAddr(iAddr, iPort, iResv);
}

void CMyComm::SetTargetAddr(int iAddr, int iPort, int iResv)
{
    m_pMyProt->SetTargetAddr(iAddr, iPort, iResv);
}

void CMyComm::SetCommand(int iFrameNo, int iCmd)
{
    m_pMyProt->SetCommand(iFrameNo, iCmd);
}

void CMyComm::SetShowRecvDataFunc(SHOWRECVDATAFUNC pShowFunc)
{
    m_pMyProt->SetShowRecvDataFunc(pShowFunc);
}
void CMyComm::SetRecvShowFunc(SETRECVSHOWFUNC pSetShowFunc)
{
    m_pMyProt->SetRecvShowFunc(pSetShowFunc);
}

void CMyComm::SetShowSendDataFunc(SHOWSENDDATAFUNC pShowFunc)
{
    m_pMyProt->SetShowSendDataFunc(pShowFunc);
}

// �����������س�ʱ���ܺ���
void CMyComm::SetDownloadTimeoutFunc(DOWNLOADTIMEOUTFUNC pDownloadTimeoutFunc)
{
    m_pDownloadTimeoutFunc = pDownloadTimeoutFunc;
}

// ����֡���ݴ�����
void CMyComm::SetFrameHandleFunc(FRAMEHANDLEFUNC pFrameHandleFunc)
{
    m_pMyProt->SetFrameHandleFunc(pFrameHandleFunc);
}

// �رմ���
void CMyComm::CloseComPort()
{
    m_ComWorkerParams.m_bComThreadExit = true;      // ���߳��˳�
    if(m_Com.isOpen())
		m_Com.close();
}

bool CMyComm::TaskIsRunning()
{
    return (m_ComWorkerParams.m_bThreadWorking);
}

// ���ô���
void CMyComm::SetComPort(char* pszComName, int iComBaud, int iComDataBits, int iComStopBits, int iComParity)
{
    strcpy_s(m_chComName, sizeof(m_chComName), pszComName); // ���ں�
    m_iComBaud = iComBaud;                                  // ������
    m_iComDataBits = iComDataBits;                          // ����λ
    m_iComStopBits = iComStopBits;                          // ֹͣλ
    m_iComParity = iComParity;                              // У��λ
}

// �򿪴���
bool CMyComm::OpenComPort()
{
    CloseComPort();
    

    // ��������
    if(true == m_ComWorkerParams.m_bThreadWorking)
    {
        //("������æ�����Ժ�����");
		m_strError.assign("The com task is busy, please try again later");
        return false;
    }

    // ���ô���

	m_Com.setPortName(m_chComName);     // ������
	m_Com.setBaudRate(m_iComBaud);			// ������
	m_Com.setDataBits(static_cast<QSerialPort::DataBits>(m_iComDataBits));	// ����λ
	m_Com.setParity(static_cast<QSerialPort::Parity>(m_iComParity));				// У��λ
	m_Com.setStopBits(static_cast<QSerialPort::StopBits>(m_iComStopBits));	// ֹͣλ
	m_Com.setFlowControl(QSerialPort::NoFlowControl);									// ��������
	m_Com.setReadBufferSize(MAX_SIZE_OF_RECV_BUF);									// ���뻺�����Ĵ�С	


	// �򿪴���
	bool bIsOpen = m_Com.open(QIODevice::ReadWrite);
	if (false == bIsOpen)
	{
		//("�򿪴���ʧ��");
		m_strError.assign(m_Com.errorString().toStdString());
		return false;
	}

    // �����߳�
    m_ComWorkerParams.m_bComThreadExit = false;
    m_ComWorkerParams.m_bThreadWorking = true;
	QtConcurrent::run(ComWorkerThread, &m_ComWorkerParams);    

    return true;
}


// �ر�����
void CMyComm::CloseNetPort()
{
    m_ComWorkerParams.m_bComThreadExit = true;      // ���߳��˳�
    if(m_UDP.isOpen()) 
		m_UDP.close();
}

// ��������
void CMyComm::SetNetPort(char* pszIPAddr, int iPort)
{
    strcpy_s(m_chNetIPAddr, sizeof(m_chNetIPAddr), pszIPAddr);
    m_iNetPort = iPort;
}

// ������
bool CMyComm::OpenNetPort()
{
    if(m_UDP.isValid())
    {
        CloseNetPort();
    }

    // ��������
    if(true == m_ComWorkerParams.m_bThreadWorking)
    {
        //("������æ�����Ժ�����");
		m_strError.assign("The net task is busy, please try again later");
        return false;
    }

    // ������
	int iPort(2018);
	// ��Unix�ϣ�ShareAddress��ͬ��SO_REUSEADDR��Windows����
	// ��Windows�ϣ�ReuseAddressHint��ͬ��SO_REUSEADDR��Unix����
	while (!m_UDP.bind(QHostAddress::AnyIPv4, iPort,
		QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint))
	{
		m_strError.assign("The UDP failed on binding port:" + std::to_string(iPort));
		++iPort;
		if (iPort >= 65536) return false;
	}
	//m_UDP.setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, MAX_SIZE_OF_SEND_BUF);
	//m_UDP.setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, MAX_SIZE_OF_RECV_BUF);

    // �����߳�    
    m_ComWorkerParams.m_bComThreadExit = false;
    m_ComWorkerParams.m_bThreadWorking = true;
	QtConcurrent::run(ComWorkerThread, &m_ComWorkerParams);

    return true;
}

// ������/����
void CMyComm::ReadComNetData()
{
    // Read
    int iReaded = 0;
    if(true == m_bCurrentIsCom)                 // ����
    {
        if(!m_Com.isOpen()) return;

		QByteArray dataRecv = m_Com.readAll();
		if (dataRecv.isEmpty())
		{
			// û�����ݣ����Ƿ�������
			m_strError.assign("There was no data currently on COM");
			return;
		}
		iReaded = std::min(MAX_SIZE_OF_RECV_BUF, dataRecv.length());
		memcpy(m_pRecvBuf, dataRecv.data(), iReaded);
    }
    else                                        // ����
    {
        if(!m_UDP.isValid()) return;

		QNetworkDatagram datagram = m_UDP.receiveDatagram();
		if (datagram.isNull())
		{
			// receive failure
			m_strError.assign("It failed on receiving data via UDP");
			return;
		}
		QByteArray dataRecv = datagram.data();
		iReaded = std::min(MAX_SIZE_OF_RECV_BUF, dataRecv.length());
		memcpy(m_pRecvBuf, dataRecv.data(), iReaded);
    }

    // Copy �������������
    if((m_iSaveLength+iReaded) >= MAX_SIZE_OF_SAVE_BUF)
    {
        // ��֤�����ݿ���
		m_iSaveLength -= iReaded+1;
        if(m_iSaveLength < 0) m_iSaveLength = 0;
		// ����������
		iReaded = 0;
    }
    memcpy(m_pSaveBuf+m_iSaveLength, m_pRecvBuf, iReaded);
    m_iSaveLength += iReaded;

    // ��������
    m_pMyProt->HandleData(m_pSaveBuf, m_iSaveLength);
}

// д����/����
void CMyComm::SendComNetData(unsigned char chCommand, const char* pszDataBuf, int iDataLength_in)
{
	QMutexLocker lock(&m_Mutex);             // Lock

    // ��֡
    m_iSendDataLength = 0;
    m_pMyProt->WriteOneFrame(chCommand, pszDataBuf, iDataLength_in, (char*)m_pSendBuf, MAX_SIZE_OF_SEND_BUF, m_iSendDataLength);

    // ����
    int iBytesWrite = m_iSendDataLength;
    if(true == m_bCurrentIsCom)
    {
		m_Com.clear();
		m_Com.clearError();
		iBytesWrite = m_Com.write((char *)m_pSendBuf, m_iSendDataLength);
		if (-1 == iBytesWrite)
		{
			m_strError.assign("It failed on sending data via COM");			
		}		
    }
    else
    {
		if (-1 == m_UDP.writeDatagram((const char*)m_pSendBuf, m_iSendDataLength, QHostAddress(m_chNetIPAddr), m_iNetPort))
		{
			m_strError.assign("It failed on writing datagram via UDP");
		}
		
    }
}

// �ط���ǰ֡
void CMyComm::ReSendCurComNetData()
{
    QMutexLocker locker(&m_Mutex);             // Lock

    // ����
    int iBytesWrite = m_iSendDataLength;
    if(true == m_bCurrentIsCom)
    {
		m_Com.clear();
		m_Com.clearError();
		iBytesWrite = m_Com.write((char *)m_pSendBuf, m_iSendDataLength);
		if (-1 == iBytesWrite)
		{
			// д����ʧ��
			m_strError.assign("It failed on sending data via COM");
		}
    }
    else
    {
		if (-1 == m_UDP.writeDatagram((const char*)m_pSendBuf, m_iSendDataLength, QHostAddress(m_chNetIPAddr), m_iNetPort))
		{
			m_strError.assign("It failed on writing datagram via UDP");
		}
    }
}

// �������ع��ܳ�ʱ����
void CMyComm::DownLoadTimeoutFunc()
{
    if(NULL != m_pDownloadTimeoutFunc) m_pDownloadTimeoutFunc();
}

// ��ǰ������Ϣ
string CMyComm::ErrorString() const
{
	return m_strError;
}

/****************************************************************************************
    ȫ�ֺ���
*****************************************************************************************/
// ʹ�����������������ָ���ɵ������ͷ�
CMyComBasic* CreateComBasic()
{
    return new CMyComm();
}

// �̺߳���
void ComWorkerThread(void *pParam)
{
    Lin_Com_Use::COMWORKER_PARAMS* pComWorkerParams = (Lin_Com_Use::COMWORKER_PARAMS*)pParam;
    CMyComm* pCommunicate =pComWorkerParams->m_pComm;

    // Loop
    while(true)
    {
        if(true == pComWorkerParams->m_bComThreadExit)          // Exit while
        {
            break;
        }

        pCommunicate->ReadComNetData();
        pCommunicate->DownLoadTimeoutFunc();

        QThread::msleep(100);
    }

    pComWorkerParams->m_bComThreadExit = false;
    pComWorkerParams->m_bThreadWorking = false;    
}

