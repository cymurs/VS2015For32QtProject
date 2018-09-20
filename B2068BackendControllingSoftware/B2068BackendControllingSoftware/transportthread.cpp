#include "stdafx.h"
#include "transportthread.h"
#include "Common/HandyTool.h"
#include "Common/singlelogger.h"

/****************************************************************************************/
QMap<QString, QString> TamingState;
const char SepCharSpace = 0x20; // �ո�: ' '
const char SepCharComma = 0x2C; // ����: ','
const char *RefSource[] = {
	"δ��λ",
	"GPS",
	"GLONASS",
	"����",
	"BAC",
	"BDC",
	"", "",  // ռλ
	"��ʱ"
};
const char *RefDTName[] = {
	"",
	"����ʱ��",
	"����ʱ��",
	"����ʱ��",
	"����B��ʱ��",
	"ֱ��B��ʱ��",
	"", "",
	"����ʱ��"
};
const char *g_CmdKeywords[] = {
	"b2068",
	"-main:",
	"-net1:",
	"-net2:",
	"-ref:",
	"-view:",
	"fpga ver:",
	"hardware number:",
	"\r\n",
};

 
/****************************************ȫ�ֺ���****************************************/
void FrameHanleFunc(const st_FrameData* pFrameData);

TransportThread *TransportThread::Get()
{
	static TransportThread transThread;
	return &transThread;
}

TransportThread::~TransportThread()
{	
	SingleLogger::instance().stop();

	if (nullptr != m_pMyComBasic) {
		delete m_pMyComBasic;
		m_pMyComBasic = nullptr;
	}
}

TransportThread::TransportThread(QObject *parent)
	: QThread(parent)
	, m_frameQueue(50)
	, m_sendingData()
	, m_multiFrameBuffer()
{
	m_pMyComBasic = CreateComBasic();

	m_pMyComBasic->SetFrameHandleFunc(::FrameHanleFunc);

	m_counter = 0;
	m_counterTimer = new QTimer(this);
	m_counterTimer->setInterval(ResendInterval);
	connect(m_counterTimer, SIGNAL(timeout()), this, SLOT(ResendData()));	

	m_isMultiFrame = false;
	m_comIsOpened = false;
	m_netIsOpened = false;

	TamingState.insert("0x00", QObject::tr("���ٲ���"));
	TamingState.insert("0x01", QObject::tr("����"));
	TamingState.insert("0x02", QObject::tr("����"));	
	TamingState.insert("0x10", QObject::tr("�ϵ�"));
	TamingState.insert("0x11", QObject::tr("����Ƶƫ"));
	TamingState.insert("0x12", QObject::tr("����"));

	SingleLogger::instance().start();
}

void TransportThread::SetComPort(const QString &name, int baud, int dataBit /*= 8*/, int stopBit /*= 0*/, int check /*= 0*/)
{
	char szComName[1024] = { '\0' };
	sprintf_s(szComName, sizeof(szComName), "\\\\.\\%s", name.toStdString().c_str());
	m_pMyComBasic->SetComPort(szComName, baud, dataBit, stopBit, check);
}

bool TransportThread::OpenCom()
{
	if (m_comIsOpened) {
		CloseCom();
	}

	CloseNet();
	if (m_pMyComBasic->OpenComPort()) {
		m_pMyComBasic->SetCurrentIsCom(true);
		m_comIsOpened = true;
	}

	return m_comIsOpened;
}

void TransportThread::CloseCom()
{
	m_pMyComBasic->CloseComPort();
	m_comIsOpened = false;
}

void TransportThread::SetNetPort(const QString &addr, int port)
{
	m_pMyComBasic->SetNetPort((char *)(addr.toStdString().c_str()), port);
}

bool TransportThread::OpenNet()
{
	if (m_netIsOpened) {
		CloseNet();
	}

	CloseCom();
	if (m_pMyComBasic->OpenNetPort()) {
		m_pMyComBasic->SetCurrentIsCom(false);
		m_netIsOpened = true;
	}

	return m_netIsOpened;
}

void TransportThread::CloseNet()
{
	m_pMyComBasic->CloseNetPort();
	m_netIsOpened = false;
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
	QString strData(pFrameData->m_pFrameDataBuf); 
	QStringList dataLst = strData.split(SepCharComma);
	if (1 >= dataLst.length()) {
		dataLst = strData.split(SepCharSpace);
		if (1 >= dataLst.length())
			return;
	}
	TRACE_INFO(tr("---READ---%1").arg(strData));

	QString strCmdName = dataLst[0];
	if (m_sendingData.contains(strCmdName)) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();		
	}

	switch (CHandyTool::hash_(qPrintable(dataLst[0]))) {
	case "status"_hash:
	{
		st_Status status;
		status.lockState = dataLst[1];
		for (int i = 2; i < dataLst.length(); ++i) {
			if (0 == dataLst[i].compare("osc")) {
				status.oscType = dataLst[i + 1];
				status.oscLock = dataLst[i + 2].toInt();
				i += 2;
				continue;
			}
			if (0 == dataLst[i].compare("curref")) {
				status.curRef = dataLst[i + 1].toInt();
				++i;
				continue;
			}
			if (0 == dataLst[i].compare("irig-b")) {
				status.irig_b = dataLst[i + 1].toInt();
				++i;
				continue;
			}
			if (0 == dataLst[i].compare("rt")) {
				status.day = dataLst[i + 1].toInt();
				status.hour = dataLst[i + 2].toInt();
				status.minute = dataLst[i + 3].toInt();
				status.second = dataLst[i + 4].toInt();
				i += 4;
				continue;
			}

		}
		emit statusSignal(status);
		break;
	}
	case "gnsstime"_hash:
	{
		st_Gnsstime gnsstime;
		gnsstime.receiverTime = dataLst[3];
		for (int i = 4; i < dataLst.length(); ++i) {
			if (0 == dataLst[i].compare("utc_time_stamp")) {
				gnsstime.UTCTime = dataLst[i + 1];
				++i;
				continue;
			}
			if (0 == dataLst[i].compare("dcb_time_stamp")) {
				gnsstime.dcbTime = dataLst[i + 1];
				++i;
				continue;
			}
			if (0 == dataLst[i].compare("acb_time_stamp")) {
				gnsstime.acbTime = dataLst[i + 1];
				++i;
				continue;
			}
			if (0 == dataLst[i].compare("intput_time_stamp")) {
				gnsstime.inputTime = dataLst[i + 1];
				++i;
				continue;
			}
		}

		emit gnsstimeSignal(gnsstime);
		break;
	}
	case "refuseok"_hash:
	{
		st_RefAvailInfo refavailinfo;
		for (int i = 0; i < dataLst.length(); ++i) {
			if (0 == dataLst[i].compare("bdsuse")) {
				refavailinfo.bdsuse = dataLst[i + 1].toInt();
				++i;
				continue;
			}
			if (0 == dataLst[i].compare("gpsuse")) {
				refavailinfo.gpsuse = dataLst[i + 1].toInt();
				++i;
				continue;
			}
			if (0 == dataLst[i].compare("glouse")) {
				refavailinfo.glouse = dataLst[i + 1].toInt();
				++i;
				continue;
			}
			if (0 == dataLst[i].compare("acbuse")) {
				refavailinfo.acbuse = dataLst[i + 1].toInt();
				++i;
				continue;
			}
			if (0 == dataLst[i].compare("dcbuse")) {
				refavailinfo.dcbuse = dataLst[i + 1].toInt();
				++i;
				continue;
			}
		}
		emit refavailinfoSignal(refavailinfo);
		break;
	}
	case "b2068"_hash:
	{
		int ret = dataLst[1].toInt();
		emit b2068Signal(ret);
		break;
	}
	case "intime"_hash:
	{
		QString res = dataLst.join(",");
		emit intimeSignal(res);
		break;
	}
	case "priority"_hash:
	{
		QString res = dataLst.join(",");
		emit prioritySignal(res);
		break;
	}
	case "baud"_hash:
	{
		QString res = dataLst.join(",");
		emit baudSignal(res);
		break;
	}
	case "IRIG_BACiF"_hash:
	case "IRIG_BACoF"_hash:
	case "IRIG_BACV"_hash:
	case "IRIG_BACM"_hash:
	{
		QString res = dataLst.join(",");
		emit irigbaResultSignal(res);
		break;
	}
	case "IRIG_BDCiF"_hash:
	case "IRIG_BDCoF"_hash:
	{
		QString res = dataLst.join(",");
		emit irigbdResultSignal(res);
		break;
	}
	case "indelayBD"_hash:
	case "indelayGPS"_hash:
	case "indelayGLONASS"_hash:
	case "indelayBAC"_hash:
	case "indelayBDC"_hash:
	{
		QString res = dataLst.join(",");
		emit indelayResultSignal(res);
		break;
	}
	case "ppswidth"_hash:
	case "ppsdelay"_hash:
	{
		QString res = dataLst.join(",");
		emit ppsResultSignal(res);
		break;
	}
	case "timezone"_hash:
	{
		int ret = dataLst[1].toInt();
		emit timezoneSignal(ret);
		break;
	}
	case "addlog"_hash:
	{
		emit addlogSignal(dataLst[1]);
		break;
	}
	case "addalarm"_hash:
	{
		emit addalarmSignal(dataLst[1]);
		break;
	}
	case "hardware"_hash:
	{
		emit hardwareSignal(dataLst[1]);
		break;
	}
	case "ver"_hash:
	{
		for (auto &data : dataLst) {
			int idx(0);
			if (-1 != (idx = data.indexOf("ver:"))) {
				QString ret("ver,");
				ret.append(data.right(data.length() - idx - 4));
				emit masterVerSignal(ret);
				continue;
			}
			if (-1 != (idx = data.indexOf("main:"))) {
				QString ret("main,");
				ret.append(data.right(data.length() - idx - 5));
				emit masterVerSignal(ret);
				continue;
			}
			if (-1 != (idx = data.indexOf("number:"))) {
				QString ret("number,");
				ret.append(data.right(data.length() - idx - 7));
				emit masterVerSignal(ret);
				continue;
			}
		}
		break;
	}
	default:
		break;
	}
}

void TransportThread::HandleFrameFromReceiverBoard(const st_FrameData *pFrameData)
{
	QString strData(pFrameData->m_pFrameDataBuf);
	QStringList dataLst = strData.split(SepCharComma);
	if (1 >= dataLst.length()) {
		dataLst = strData.split(SepCharSpace);
		if (1 >= dataLst.length())
			return;
	}
	TRACE_INFO(tr("---READ---%1").arg(strData));

	if (m_sendingData.contains(dataLst[0])) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();
	}

	switch (CHandyTool::hash_(qPrintable(dataLst[0]))) {
	case "gnsssta"_hash:
	{
		st_Gnsssta gnsssta;
		strcpy_s(gnsssta.satelliteType, 4, qPrintable(dataLst[3]));
		gnsssta.visSatellites = dataLst[4].toInt();
		gnsssta.avlSatellites = dataLst[5].toInt();
		gnsssta.d3PosFlag = dataLst[6].toInt();
		gnsssta.lngValue = dataLst[8].toFloat();
		gnsssta.longitude = (0 == dataLst[9].compare("e")) ? tr("����") : tr("����");
		gnsssta.latValue = dataLst[10].toFloat();
		gnsssta.latitude = (0 == dataLst[11].compare("s")) ? tr("��γ") : tr("��γ");
		gnsssta.elevation = dataLst[12].toFloat();
		gnsssta.pdop = dataLst[13].toFloat();
		gnsssta.rmcStateFlag = dataLst[14].toInt();
		gnsssta.snrFlag = dataLst[15].toInt();

		emit gnssstaSignal(gnsssta);
		break;
	}
	case "gnssleap"_hash:
	{
		uint leapSec = dataLst[3].toUInt();
		structTime dateTime = TimeStampToTime(leapSec);
		QString res = tr("%1-%2-%3 %4:%5:%6")
			.arg(dateTime.Year).arg(dateTime.Month)
			.arg(dateTime.Date).arg(dateTime.Hour)
			.arg(dateTime.Minute).arg(dateTime.Second);
		emit gnssleapSignal(res);
		break;
	}
	case "gnssgsv"_hash:
	{
		st_Gnssgsv gnssgsv;
		strcpy_s(gnssgsv.satelliteType, 4, qPrintable(dataLst[3]));
		gnssgsv.frameTotal = dataLst[4].toInt();
		gnssgsv.frameNo = dataLst[5].toInt();
		gnssgsv.satellitesTotal = dataLst[6].toInt();
		int cnt = (dataLst.length() - 7) / 4;
		for (int i = 0, start = 7; i < cnt; ++i, start += 4) {
			gnssgsv.satelliteNo[i] = dataLst[start].toInt();
			gnssgsv.elevation[i] = dataLst[start + 1].toInt();
			gnssgsv.azimuth[i] = dataLst[start + 2].toInt();
			gnssgsv.snr[i] = dataLst[start + 3].toInt();
		}
		gnssgsv.count = cnt;
		emit gnssgsvSignal(gnssgsv);
		break;
	}
	case "ver"_hash:
	{
		for (auto &data : dataLst) {
			int idx(0);
			if (-1 != (idx = data.indexOf("ref:"))) {
				QString ret(data.right(data.length() - idx - 4));
				emit receiverVerSignal(ret);
				break;
			}
		}
		break;
	}
	default:
		break;
	}
}

void TransportThread::HandleFrameFromNetBoard(const st_FrameData *pFrameData)
{
	QString strData(pFrameData->m_pFrameDataBuf);
	QStringList dataLst = strData.split(SepCharComma);
	if (1 >= dataLst.length()) {
		dataLst = strData.split(SepCharSpace);
		if (1 >= dataLst.length())
			return;
	}
	TRACE_INFO(tr("---READ---%1").arg(strData));

	if (m_sendingData.contains(dataLst[0])) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();
	}

	QString res = dataLst.join(",");
	switch (CHandyTool::hash_(qPrintable(dataLst[0]))) {
	case "macaddr"_hash:
	{
		emit macaddrSignal(res);
		break;
	}
	case "ver"_hash:
	{
		for (auto &data : dataLst) {
			int idx(0);
			if (-1 != (idx = data.indexOf("net1:"))) {
				QString ret("net1,");
				ret.append(data.right(data.length() - idx - 5));
				emit netVerSignal(ret);
				continue;
			}
			if (-1 != (idx = data.indexOf("net2:"))) {
				QString ret("net2,");
				ret.append(data.right(data.length() - idx - 5));
				emit netVerSignal(ret);
				continue;
			}
		}
		break;
	}
	default:
	{
		emit netResultSignal(res);
		break;
	}
	}
}

void TransportThread::HandleFrameFromDisplayBoard(const st_FrameData *pFrameData)
{
	QString strData(pFrameData->m_pFrameDataBuf);
	QStringList dataLst = strData.split(SepCharComma);
	if (1 >= dataLst.length()) {
		dataLst = strData.split(SepCharSpace);
		if (1 >= dataLst.length())
			return;
	}
	TRACE_INFO(tr("---READ---%1").arg(strData));
	
	if (m_sendingData.contains(dataLst[0])) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();
	}

	switch (CHandyTool::hash_(qPrintable(dataLst[0]))) {
	case "sn"_hash:
	{
		emit snResultSignal(dataLst[1]);
		break;
	}
	case "reset"_hash:
	{
		emit resetSignal(dataLst[1]);
		break;
	}
	case "ver"_hash:
	{
		for (auto &data : dataLst) {
			int idx(0);
			if (-1 != (idx = data.indexOf("view:"))) {
				QString ret(data.right(data.length() - idx - 5));
				emit displayVerSignal(ret);
				break;
			}						
		}		
		break;
	}
	default:
		break;
	}

}

void TransportThread::HandleFrameFromMasterBoardEx(const st_FrameData *pFrameData)
{
	QString strData;
	if (m_isMultiFrame) {
		strData = m_multiFrameBuffer;
		m_multiFrameBuffer.clear();
		m_isMultiFrame = false;
	}
	else {
		strData = pFrameData->m_pFrameDataBuf;
	}	
	TRACE_INFO(tr("---READ---%1").arg(strData));
	strData = strData.toLower();

	if (strData.contains(m_sendingData.toLower())) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();
	}

	int found(-1);
	if (strData.contains("B2068 Main_Control", Qt::CaseInsensitive)) {
		emit qumarkSignal(strData);
		return;
	}
	if (-1 != (found = strData.indexOf(g_CmdKeywords[Main], Qt::CaseInsensitive))) {
		st_MasterVer masterver;		
		masterver.mainv = mid(strData, g_CmdKeywords[Main], 2, g_CmdKeywords[RN]).toFloat();
		masterver.net1v = mid(strData, g_CmdKeywords[Net1], 2, g_CmdKeywords[RN]).toFloat();
		masterver.net2v = mid(strData, g_CmdKeywords[Net2], 2, g_CmdKeywords[RN]).toFloat();
		masterver.refv = mid(strData, g_CmdKeywords[Ref_], 2, g_CmdKeywords[RN]).toFloat();
		masterver.viewv = mid(strData, g_CmdKeywords[View], 2, g_CmdKeywords[RN]).toFloat();
		masterver.firmware = mid(strData, g_CmdKeywords[FPGA], 1, g_CmdKeywords[RN]);
		masterver.hardware = mid(strData, g_CmdKeywords[Hard], 2, g_CmdKeywords[RN]);
		emit verSignal(masterver);
		return;
	}
	if (-1 != (found = strData.indexOf(g_CmdKeywords[B2068], Qt::CaseInsensitive))) {
		if (strData.contains("2d", Qt::CaseInsensitive))
			emit b2068Signal(2);
		if (strData.contains("-3"))
			emit b2068Signal(3);
		return;
	}
}

void TransportThread::HandleFrameFromReceiverBoardEx(const st_FrameData *pFrameData)
{
	QString strData(pFrameData->m_pFrameDataBuf);
	TRACE_INFO(tr("---READ---%1").arg(strData));
	strData = strData.toLower();

	if (strData.contains(m_sendingData.toLower())) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();
	}
}

void TransportThread::HandleFrameFromNetBoardEx(const st_FrameData *pFrameData)
{
	QString strData(pFrameData->m_pFrameDataBuf);
	TRACE_INFO(tr("---READ---%1").arg(strData));
	strData = strData.toLower();

	if (strData.contains(m_sendingData.toLower())) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();
	}
}

void TransportThread::HandleFrameFromDisplayBoardEx(const st_FrameData *pFrameData)
{
	QString strData(pFrameData->m_pFrameDataBuf);
	TRACE_INFO(tr("---READ---%1").arg(strData));
	strData = strData.toLower();

	if (strData.contains(m_sendingData.toLower())) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();
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

			if (!pFrame->m_bDataIsOK) continue;
			if (0 == pFrame->m_iFrameDataLength
				&& 0 == strlen(pFrame->m_pFrameDataBuf)) continue;
			// ���� 60 00, �����봦�� 00 00 ������
			if (g_BoardAddr[PCAddr][0] != pFrame->m_chTargetAddr
				&& g_BoardAddr[GeneralAddr][0] != pFrame->m_chTargetAddr) continue;
			// �����֡
			if (0x11 == pFrame->m_chTargetUse) {
				m_multiFrameBuffer.append(pFrame->m_pFrameDataBuf);
				if (260 == pFrame->m_iFrameDataLength) {
					/*if (!m_multiFrameBuffer.isEmpty()) {
						m_multiFrameBuffer.append(pFrame->m_pFrameDataBuf);
					}
					else {
						m_multiFrameBuffer = pFrame->m_pFrameDataBuf;
					}*/					
					continue;
				}
				else {					
					m_isMultiFrame = true;
				}
			}

			if (g_BoardAddr[PCAddr][0] == pFrame->m_chTargetAddr) {
				m_counterMutex.lock();
				m_counter = 0;
				m_counterMutex.unlock();
			}
			
			if (pFrame->m_chSourceAddr == g_BoardAddr[MasterControlAddr][0]) {
				HandleFrameFromMasterBoardEx(pFrame);
				continue;
			}

			if (pFrame->m_chSourceAddr == g_BoardAddr[Net1Addr][0]) {
				HandleFrameFromNetBoard(pFrame);
				continue;
			}

			if (pFrame->m_chSourceAddr == g_BoardAddr[Net2Addr][0]) {
				HandleFrameFromNetBoard(pFrame);
				continue;
			}

			if (pFrame->m_chSourceAddr == g_BoardAddr[ReceiverAddr][0]) {
				HandleFrameFromReceiverBoard(pFrame);
				continue;
			}
			
			if (pFrame->m_chSourceAddr == g_BoardAddr[DisplayAddr][0]) {
				HandleFrameFromDisplayBoard(pFrame);
				continue;				
			}
						
		}
	}
}

QString TransportThread::mid(const QString &str, const QString &head, int offset /*= 0*/, const QString &tail /*= "\r\n"*/)
{
	QString ret;
	int found = str.indexOf(head);
	if (-1 == found) {
		return ret;
	}

	found += head.size() + offset;
	int lastPos = str.indexOf(tail, found);
	if (-1 != lastPos)
		ret = str.mid(found, lastPos - found);
	else
		ret = str.mid(found);

	return ret;
}

void TransportThread::SetSourceAddr(int iAddr, int iPort, int iResv)
{
	m_pMyComBasic->SetSourceAddr(iAddr, iPort, iResv);
}

void TransportThread::SetTargetAddr(int iAddr, int iPort, int iResv)
{
	m_pMyComBasic->SetTargetAddr(iAddr, iPort, iResv);
}

void TransportThread::SetCommand(int iCmd)
{
	int iFrameNo(0x01);
	m_pMyComBasic->SetCommand(iFrameNo, iCmd);
}

void TransportThread::SetFrameDataFormat(bool bSendIs16Hex, bool bRecvIs16Hex, bool bHasFrame)
{
	m_pMyComBasic->SetSendDataIs16Hex(bSendIs16Hex);
	m_pMyComBasic->SetRecvDataIs16Hex(bRecvIs16Hex);
	m_pMyComBasic->SetDataHasFrame(bHasFrame);
}

void TransportThread::SendComNetData(unsigned char chCommand, const char* pszDataBuf, int iDataLength)
{
	if (0 != m_counter) {
		emit exceptionSignal(BlockException);
		return;
	}
	
	m_pMyComBasic->SendComNetData(chCommand, pszDataBuf, iDataLength);
	m_sendingData = QString::fromLatin1(pszDataBuf, iDataLength);
	m_counterTimer->start();
	++m_counter;

	TRACE_INFO(tr("---WRITE---%1").arg(m_sendingData));
}

void TransportThread::ResendData()
{
	m_counterTimer->stop();	
	QMutexLocker locker(&m_counterMutex);
	if (0 != m_counter) {
		if (m_counter < 3) {
			m_pMyComBasic->SendComNetData(0, m_sendingData.toStdString().c_str(), m_sendingData.length());
			m_counterTimer->start();
			++m_counter;
		}
		else {
			m_counter = 0;
			emit exceptionSignal(SendFailedException);
		}
	}
}


/****************************************ȫ�ֺ���****************************************/
void FrameHanleFunc(const st_FrameData* pFrameData)
{
	TransportThread::Get()->FrameHandleFunc(*pFrameData);
}