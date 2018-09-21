#pragma once

#include <QThread>
#include "Common/qdoublebufferedqueue.h"
#include "Common/Libase.h"

class TransportThread : public QThread
{
	Q_OBJECT

public:
	static TransportThread *Get();
	virtual ~TransportThread();

	void SetComPort(const QString &name, int baud, int dataBit = 8, int stopBit = 0, int check = 0);
	bool OpenCom();
	void CloseCom();
	void SetNetPort(const QString &addr, int port);
	bool OpenNet();
	void CloseNet();

	void stop();

	void FrameHandleFunc(const st_FrameData &frameData);
	void HandleFrameFromMasterBoard(const st_FrameData *pFrameData);
	void HandleFrameFromReceiverBoard(const st_FrameData *pFrameData);
	void HandleFrameFromNetBoard(const st_FrameData *pFrameData);
	void HandleFrameFromDisplayBoard(const st_FrameData *pFrameData);

	void HandleFrameFromMasterBoardEx(const st_FrameData *pFrameData);
	void HandleFrameFromReceiverBoardEx(const st_FrameData *pFrameData);
	void HandleFrameFromNetBoardEx(const st_FrameData *pFrameData);
	void HandleFrameFromDisplayBoardEx(const st_FrameData *pFrameData);

	void SetSourceAddr(int iAddr, int iPort, int iResv);  // Step 1
	void SetTargetAddr(int iAddr, int iPort, int iResv);   // Step 2
	void SetCommand(int iCmd);	  // Step 3
	void SetFrameDataFormat(bool bSendIs16Hex, bool bRecvIs16Hex, bool bHasFrame);   // Step 4
	void SendComNetData(unsigned char chCommand, const char* pszDataBuf, int iDataLength, bool bResend = false/* «∑Ò÷ÿ∑¢*/);   // Step 5
	

protected:
	TransportThread(QObject *parent = nullptr);
	TransportThread(const TransportThread &) = delete;
	TransportThread& operator=(const TransportThread &) = delete;

	virtual void run() override;
	virtual QString mid(const QString &str, const QString &head, int offset = 0, const QString &tail = "\r\n");

private slots:
	void ResendData();

signals:
	void b2068Signal(int);
	void statusSignal(const st_Status&);
	void gnsstimeSignal(const st_Gnsstime&);
	void refavailinfoSignal(const st_RefAvailInfo&);
	void intimeSignal(const QString&);
	void prioritySignal(const QString&);
	void baudSignal(const QString&);
	void irigbaResultSignal(const QString&);
	void irigbdResultSignal(const QString&);
	void indelayResultSignal(const QString&);
	void ppsResultSignal(const QString&);
	void timezoneSignal(int);
	void addlogSignal(const QString&);
	void addalarmSignal(const QString&);
	void hardwareSignal(const QString&);
	void masterVerSignal(const QString&);
	void verSignal(const st_MasterVer&);
	void qumarkSignal(const QString &);

	void gnssstaSignal(const st_Gnsssta&);
	void gnssleapSignal(const QString&);
	void gnssgsvSignal(const st_Gnssgsv&);
	void receiverVerSignal(const QString&);

	void macaddrSignal(const QString&);
	void netResultSignal(const QString&);
	void netVerSignal(const QString&);
	void netGroupportSignal(const QString &);
	void net1InfoSignal(const st_NetInfo&);
	void net2InfoSignal(const st_NetInfo&);

	void snResultSignal(const QString&);
	void resetSignal(const QString&);
	void displayVerSignal(const QString&);

	//void resultSignal(const QString&);
	void exceptionSignal(int);	

private:
	QDoubleBufferedQueue<st_FrameData> m_frameQueue;
	CMyComBasic *m_pMyComBasic;
	QTimer *m_counterTimer;
	int m_counter;
	QMutex m_counterMutex;
	QString m_sendingData;
	QString m_multiFrameBuffer;
	bool m_isMultiFrame;
	bool m_comIsOpened;
	bool m_netIsOpened;

};
