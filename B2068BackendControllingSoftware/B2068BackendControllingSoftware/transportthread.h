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

	void stop();

	void FrameHandleFunc(const st_FrameData &frameData);
	void HandleFrameFromMasterBoard(const st_FrameData *pFrameData);
	void HandleFrameFromReceiverBoard(const st_FrameData *pFrameData);
	void HandleFrameFromNetBoard(const st_FrameData *pFrameData);
	void HandleFrameFromDisplayBoard(const st_FrameData *pFrameData);

protected:
	TransportThread(QObject *parent = nullptr);
	TransportThread(const TransportThread &) = delete;
	TransportThread& operator=(const TransportThread &) = delete;

	virtual void run() override;

signals:
	void b2068Signal(int);
	void statusSignal(const st_Status&);
	void gnsstimeSignal(const st_Gnsstime&);
	void refavailinfoSignal(const st_RefAvailInfo&);
	void irigbaResultSignal(const QString&);
	void irigbdResultSignal(const QString&);
	void indelayResultSignal(const QString&);
	void ppsResultSignal(const QString&);
	void timezoneSignal(int);
	void addlogSignal(const QString&);
	void addalarmSignal(const QString&);
	void hardwareSignal(const QString&);
	void masterVerSignal(const QString&);

	void gnssstaSignal(const st_Gnsssta&);
	void gnssleapSignal(const QString&);
	void gnssgsvSignal(const st_Gnssgsv&);
	void receiverVerSignal(const QString&);

	void macaddrSignal(const QString&);
	void netResultSignal(const QString&);
	void netVerSignal(const QString&);

	void snResultSignal(const QString&);
	void resetSignal(const QString&);
	void displayVerSignal(const QString&);

	void resultSignal(const QString&);
	

private:
	QDoubleBufferedQueue<st_FrameData> m_frameQueue;
	CMyComBasic *m_pMyComBasic;
};
