#include "stdafx.h"
#include "transportthread.h"

TransportThread *TransportThread::Get()
{
	static TransportThread transThread;
	return &transThread;
}

TransportThread::~TransportThread()
{
}

TransportThread::TransportThread(QObject *parent)
	: QThread(parent)
	, m_frameQueue(50)
{
	m_pMyComBasic = CreateComBasic();
}

void TransportThread::stop()
{
	requestInterruption();
}

void TransportThread::FrameHandleFunc(const st_FrameData &frameData)
{
	m_frameQueue.put(frameData);
}

void TransportThread::run()
{
	while (!isInterruptionRequested()) {

	}
}


