#include "Communicate.h"

#include "Protocol.h"

#include <QNetworkDatagram>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include <stdio.h>      // for sprintf_s()


/****************************************************************************************/
void ComWorkerThread(void *pParam);


/****************************************************************************************
    通信模块
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

// 当前使用网口还是串口
void CMyComm::SetCurrentIsCom(bool bIsCom)
{
    m_bCurrentIsCom = bIsCom;
}

// 收发数据是否是16进制
void CMyComm::SetSendDataIs16Hex(bool bSendIs16Hex)
{
    m_pMyProt->SetSendDataIs16Hex(bSendIs16Hex);
}
void CMyComm::SetRecvDataIs16Hex(bool bRecvIs16Hex)
{
    m_pMyProt->SetRecvDataIs16Hex(bRecvIs16Hex);
}

// 收发数据是否有帧结构
void CMyComm::SetDataHasFrame(bool bHasFrame)
{
    m_pMyProt->SetDataHasFrame(bHasFrame);
}

// 是否接收所有的数据包
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

// 设置升级下载超时功能函数
void CMyComm::SetDownloadTimeoutFunc(DOWNLOADTIMEOUTFUNC pDownloadTimeoutFunc)
{
    m_pDownloadTimeoutFunc = pDownloadTimeoutFunc;
}

// 设置帧数据处理函数
void CMyComm::SetFrameHandleFunc(FRAMEHANDLEFUNC pFrameHandleFunc)
{
    m_pMyProt->SetFrameHandleFunc(pFrameHandleFunc);
}

// 关闭串口
void CMyComm::CloseComPort()
{
    m_ComWorkerParams.m_bComThreadExit = true;      // 让线程退出
    if(m_Com.isOpen())
		m_Com.close();
}

bool CMyComm::TaskIsRunning()
{
    return (m_ComWorkerParams.m_bThreadWorking);
}

// 配置串口
void CMyComm::SetComPort(char* pszComName, int iComBaud, int iComDataBits, int iComStopBits, int iComParity)
{
    strcpy_s(m_chComName, sizeof(m_chComName), pszComName); // 串口号
    m_iComBaud = iComBaud;                                  // 波特率
    m_iComDataBits = iComDataBits;                          // 数据位
    m_iComStopBits = iComStopBits;                          // 停止位
    m_iComParity = iComParity;                              // 校验位
}

// 打开串口
bool CMyComm::OpenComPort()
{
    CloseComPort();
    

    // 避免重入
    if(true == m_ComWorkerParams.m_bThreadWorking)
    {
        //("任务正忙，请稍后重试");
		m_strError.assign("The com task is busy, please try again later");
        return false;
    }

    // 配置串口

	m_Com.setPortName(m_chComName);     // 串口名
	m_Com.setBaudRate(m_iComBaud);			// 波特率
	m_Com.setDataBits(static_cast<QSerialPort::DataBits>(m_iComDataBits));	// 数据位
	m_Com.setParity(static_cast<QSerialPort::Parity>(m_iComParity));				// 校验位
	m_Com.setStopBits(static_cast<QSerialPort::StopBits>(m_iComStopBits));	// 停止位
	m_Com.setFlowControl(QSerialPort::NoFlowControl);									// 流控设置
	m_Com.setReadBufferSize(MAX_SIZE_OF_RECV_BUF);									// 输入缓冲区的大小	


	// 打开串口
	bool bIsOpen = m_Com.open(QIODevice::ReadWrite);
	if (false == bIsOpen)
	{
		//("打开串口失败");
		m_strError.assign(m_Com.errorString().toStdString());
		return false;
	}

    // 创建线程
    m_ComWorkerParams.m_bComThreadExit = false;
    m_ComWorkerParams.m_bThreadWorking = true;
	QtConcurrent::run(ComWorkerThread, &m_ComWorkerParams);    

    return true;
}


// 关闭网口
void CMyComm::CloseNetPort()
{
    m_ComWorkerParams.m_bComThreadExit = true;      // 让线程退出
    if(m_UDP.isOpen()) 
		m_UDP.close();
}

// 配置网口
void CMyComm::SetNetPort(char* pszIPAddr, int iPort)
{
    strcpy_s(m_chNetIPAddr, sizeof(m_chNetIPAddr), pszIPAddr);
    m_iNetPort = iPort;
}

// 打开网口
bool CMyComm::OpenNetPort()
{
    if(m_UDP.isValid())
    {
        CloseNetPort();
    }

    // 避免重入
    if(true == m_ComWorkerParams.m_bThreadWorking)
    {
        //("任务正忙，请稍后重试");
		m_strError.assign("The net task is busy, please try again later");
        return false;
    }

    // 打开网口
	int iPort(2018);
	// 在Unix上，ShareAddress等同于SO_REUSEADDR，Windows忽略
	// 在Windows上，ReuseAddressHint等同于SO_REUSEADDR，Unix忽略
	while (!m_UDP.bind(QHostAddress::AnyIPv4, iPort,
		QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint))
	{
		m_strError.assign("The UDP failed on binding port:" + std::to_string(iPort));
		++iPort;
		if (iPort >= 65536) return false;
	}
	//m_UDP.setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, MAX_SIZE_OF_SEND_BUF);
	//m_UDP.setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, MAX_SIZE_OF_RECV_BUF);

    // 创建线程    
    m_ComWorkerParams.m_bComThreadExit = false;
    m_ComWorkerParams.m_bThreadWorking = true;
	QtConcurrent::run(ComWorkerThread, &m_ComWorkerParams);

    return true;
}

// 读串口/网口
void CMyComm::ReadComNetData()
{
    // Read
    int iReaded = 0;
    if(true == m_bCurrentIsCom)                 // 串口
    {
        if(!m_Com.isOpen()) return;

		QByteArray dataRecv = m_Com.readAll();
		if (dataRecv.isEmpty())
		{
			// 没有数据，或是发生错误
			m_strError.assign("There was no data currently on COM");
			return;
		}
		iReaded = std::min(MAX_SIZE_OF_RECV_BUF, dataRecv.length());
		memcpy(m_pRecvBuf, dataRecv.data(), iReaded);
    }
    else                                        // 网口
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

    // Copy 避免数据区溢出
    if((m_iSaveLength+iReaded) >= MAX_SIZE_OF_SAVE_BUF)
    {
        // 保证新数据可用
		m_iSaveLength -= iReaded+1;
        if(m_iSaveLength < 0) m_iSaveLength = 0;
		// 丢弃新数据
		iReaded = 0;
    }
    memcpy(m_pSaveBuf+m_iSaveLength, m_pRecvBuf, iReaded);
    m_iSaveLength += iReaded;

    // 处理数据
    m_pMyProt->HandleData(m_pSaveBuf, m_iSaveLength);
}

// 写串口/网口
void CMyComm::SendComNetData(unsigned char chCommand, const char* pszDataBuf, int iDataLength_in)
{
	QMutexLocker lock(&m_Mutex);             // Lock

    // 组帧
    m_iSendDataLength = 0;
    m_pMyProt->WriteOneFrame(chCommand, pszDataBuf, iDataLength_in, (char*)m_pSendBuf, MAX_SIZE_OF_SEND_BUF, m_iSendDataLength);

    // 发送
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

// 重发当前帧
void CMyComm::ReSendCurComNetData()
{
    QMutexLocker locker(&m_Mutex);             // Lock

    // 发送
    int iBytesWrite = m_iSendDataLength;
    if(true == m_bCurrentIsCom)
    {
		m_Com.clear();
		m_Com.clearError();
		iBytesWrite = m_Com.write((char *)m_pSendBuf, m_iSendDataLength);
		if (-1 == iBytesWrite)
		{
			// 写串口失败
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

// 升级下载功能超时函数
void CMyComm::DownLoadTimeoutFunc()
{
    if(NULL != m_pDownloadTimeoutFunc) m_pDownloadTimeoutFunc();
}

// 当前出错信息
string CMyComm::ErrorString() const
{
	return m_strError;
}

/****************************************************************************************
    全局函数
*****************************************************************************************/
// 使用这个函数创建对象；指针由调用者释放
CMyComBasic* CreateComBasic()
{
    return new CMyComm();
}

// 线程函数
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

