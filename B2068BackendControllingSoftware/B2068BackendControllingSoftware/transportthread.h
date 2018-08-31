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

protected:
	TransportThread(QObject *parent = nullptr);
	TransportThread(const TransportThread &) = delete;
	TransportThread& operator=(const TransportThread &) = delete;

	virtual void run() override;

private:
	QDoubleBufferedQueue<st_FrameData> m_frameQueue;
	CMyComBasic *m_pMyComBasic;
};
