#include "stdafx.h"
#include "transportthread.h"
#include "HandyTool.h"

/****************************************************************************************/
const char SepCharSpace = 0x20; // ¿Õ¸ñ: ' '
const char SepCharComma = 0x2C; // ¶ººÅ: ','

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

void TransportThread::HandleFrameFromMasterBoard(const st_FrameData *pFrameData)
{
	if (!pFrameData->m_bDataIsOK)
		return;

	QString strData(pFrameData->m_pFrameDataBuf);
	QStringList dataLst = strData.split(SepCharComma);
	if (1 >= dataLst.length())
		dataLst = strData.split(SepCharSpace);
	if (1 >= dataLst.length())
		return;

	QString strCmdName = dataLst[0];
	switch (CHandyTool::hash_(qPrintable(dataLst[0]))) {
	case "status"_hash:
	{

	}
	case "gnsstime"_hash:
	{

	}
	case "refuseok"_hash:
	{

	}
	default:
		break;
	}
}

void TransportThread::run()
{
	while (!isInterruptionRequested()) {
		st_FrameData *pFrameData;
		int num(0);
		num = m_frameQueue.get(&pFrameData);
		for (int i = 0; i < num; ++i) {
			st_FrameData *pFrame = pFrameData + i;

			if (g_BoardAddr[PCAddr][0] != pFrame->m_chTargetAddr) continue;
			
			if (pFrame->m_chSourceAddr == g_BoardAddr[MasterControlAddr][0]) {
				HandleFrameFromMasterBoard(pFrame);
				continue;
			}

			if (pFrame->m_chSourceAddr == g_BoardAddr[Net1Addr][0]) {

				continue;
			}

			if (pFrame->m_chSourceAddr == g_BoardAddr[Net2Addr][0]) {

				continue;
			}

			if (pFrame->m_chSourceAddr == g_BoardAddr[ReceiverAddr][0]) {

				continue;
			}
			
			if (pFrame->m_chSourceAddr == g_BoardAddr[DisplayAddr][0]) {

				
			}
			
			
		}
	}
}


