#include "stdafx.h"
#include "transportthread.h"
#include "Common/HandyTool.h"
#include "Common/singlelogger.h"

/****************************************************************************************/
QMap<QString, QString> TamingState;
const char SepCharSpace = 0x20; // 空格: ' '
const char SepCharComma = 0x2C; // 逗号: ','
const char *SepCrLf = "\r\n";

const char *RefSource[] = {
	"未定位",
	"GPS",
	"GLONASS",
	"北斗",
	"BAC",
	"BDC",
	"", "",  // 占位
	"置时"
};
const char *RefDTName[] = {
	"",
	"北京时间",
	"北京时间",
	"北京时间",
	"交流B码时间",
	"直流B码时间",
	"", "",
	"输入时间"
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

 
/****************************************全局函数****************************************/
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

	TamingState.insert("0x00", QObject::tr("快速捕获"));
	TamingState.insert("0x01", QObject::tr("捕获"));
	TamingState.insert("0x02", QObject::tr("锁定"));	
	TamingState.insert("0x10", QObject::tr("上电"));
	TamingState.insert("0x11", QObject::tr("估算频偏"));
	TamingState.insert("0x12", QObject::tr("保持"));

	registerMetaType();

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
		status.lockState = dataLst[1].toInt();
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
		gnsssta.longitude = (0 == dataLst[9].compare("e")) ? tr("东经") : tr("西经");
		gnsssta.latValue = dataLst[10].toFloat();
		gnsssta.latitude = (0 == dataLst[11].compare("s")) ? tr("南纬") : tr("北纬");
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
	QString strData(pFrameData->m_pFrameDataBuf);
	if (m_isMultiFrame) {
		strData = m_multiFrameBuffer;
		m_multiFrameBuffer.clear();
		m_isMultiFrame = false;
	}
	TRACE_INFO(tr("---READ---%1").arg(strData));
	strData = strData.toLower();

	if (strData.contains(m_sendingData.toLower())) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();
	}

	const char *strKeys[] = {
		"b2068",				// 0
		"-main:",             // 1
		"-net1:",              // 2
		"-net2:",              // 3
		"-ref:",
		"-view:",
		"fpga ver:",         // 6
		"hardware number:",  // 7
		"current serial port rate is:",  // 8
		"test com:",
		"user com1:",
		"user com2:",
		"set baud rate to be successful", // 12
		"status,",  // 13
		"gnsstime,", // 14
		"current priority is:",
		"refuseok,",
		"",
		"",
		"",
	};

	int found(-1);
	if (strData.startsWith(strKeys[13])) {
		st_Status status;
		QStringList dataLst = strData.split(SepCharComma);
		status.lockState = dataLst.at(1).toInt();
		status.curRef = dataLst.at(6).toInt();

		emit statusSignal(status);
		return;
	}
	if (strData.contains(strKeys[14])) {
		st_Gnsstime gnsstime;
		QStringList dataLst = strData.split(SepCharComma);		
		gnsstime.receiverTime = dataLst.at(3);
		for (int i = 5; i < dataLst.length() - 1; ++i) {
			if (0 == dataLst.at(i).compare("utc_time_stamp")) {
				gnsstime.UTCTime = dataLst.at(i + 1);
				++i;
				continue;
			}
			if (0 == dataLst.at(i).compare("dcb_time_stamp")) {
				gnsstime.dcbTime = dataLst.at(i + 1);
				++i;
				continue;
			}
			if (0 == dataLst.at(i).compare("acb_time_stamp")) {
				gnsstime.acbTime = dataLst.at(i + 1);
				++i;
				continue;
			}
			if (0 == dataLst.at(i).compare("intput_time_stamp")) {
				gnsstime.inputTime = dataLst.at(i + 1);
				++i;
				continue;
			}
		}
		emit gnsstimeSignal(gnsstime);
		return;
	}
	if (strData.contains(strKeys[15])) {
		QString ret = mid(strData, strKeys[15], 1);
		emit prioritySignal(ret);
		return;
	}
	if (strData.contains(strKeys[16])) {
		st_RefAvailInfo refAvailInfo;
		QStringList dataLst = strData.split(SepCharComma);
		for (int i = 1; i < dataLst.size() - 1; ++i) {
			if (0 == dataLst.at(i).compare("bdsuse")) {
				refAvailInfo.bdsuse = dataLst.at(i + 1).toInt();
				++i;
				continue;
			}
			if (0 == dataLst.at(i).compare("gpsuse")) {
				refAvailInfo.gpsuse = dataLst.at(i + 1).toInt();
				++i;
				continue;
			}
			if (0 == dataLst.at(i).compare("glouse")) {
				refAvailInfo.glouse = dataLst.at(i + 1).toInt();
				++i;
				continue;
			}
			if (0 == dataLst.at(i).compare("acbuse")) {
				refAvailInfo.acbuse = dataLst.at(i + 1).toInt();
				++i;
				continue;
			}
			if (0 == dataLst.at(i).compare("dcbuse")) {
				refAvailInfo.dcbuse = dataLst.at(i + 1).toInt();
				++i;
				continue;
			}
		}
		emit refavailinfoSignal(refAvailInfo);
		return;
	}
	if (strData.contains(strKeys[8])) {
		QString test = mid(strData, strKeys[9], 2);
		QString com1 = mid(strData, strKeys[10], 1);
		QString com2 = mid(strData, strKeys[11], 1);
		QString ret = QString("baud,%1,%2,%3")
			.arg(test).arg(com1).arg(com2);
		emit baudSignal(ret);
		return;
	}
	if (strData.contains(strKeys[12])) {
		QStringList retLst = strData.split(SepCrLf);
		emit baudSignal(retLst.at(0));
		return;
	}
	if (strData.contains("B2068 Main_Control", Qt::CaseInsensitive) || 
		strData.contains("- look input or output message", Qt::CaseInsensitive)) {
		emit qumarkSignal(strData);
		return;
	}
	if (-1 != (found = strData.indexOf(strKeys[1], Qt::CaseInsensitive))) {
		st_MasterVer masterver;	
		int i = 1;
		masterver.mainv = mid(strData, strKeys[i++], 2).toFloat();
		masterver.net1v = mid(strData, strKeys[i++], 2).toFloat();
		masterver.net2v = mid(strData, strKeys[i++], 2).toFloat();
		masterver.refv = mid(strData, strKeys[i++], 2).toFloat();
		masterver.viewv = mid(strData, strKeys[i++], 2).toFloat();
		masterver.firmware = mid(strData, strKeys[i++], 1);
		masterver.hardware = mid(strData, strKeys[i], 1);
		emit verSignal(masterver);
		return;
	}
	if (-1 != (found = strData.indexOf(strKeys[0], Qt::CaseInsensitive))) {
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
	if (m_isMultiFrame) {
		strData = m_multiFrameBuffer;
		m_multiFrameBuffer.clear();
		m_isMultiFrame = false;
	}
	TRACE_INFO(tr("---READ---%1").arg(strData));
	strData = strData.toLower();

	if (strData.contains(m_sendingData.toLower())) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();
	}

	const char *strKeys[] = {
		"b20682d-ref:",
		"gnssgsv,",
		"gnsssta,",
		"",
		"",
	};

	int found(-1);
	if (strData.contains(strKeys[2])) {
		st_Gnsssta gnsssta;
		QStringList dataLst = strData.split(SepCharComma);
		strcpy_s(gnsssta.satelliteType, 4, qPrintable(dataLst.at(3)));
		gnsssta.visSatellites = dataLst.at(4).toInt();
		gnsssta.avlSatellites = dataLst.at(5).toInt();
		gnsssta.d3PosFlag = dataLst.at(6).toInt();
		gnsssta.lngValue = dataLst.at(8).toFloat();
		gnsssta.longitude = dataLst.at(9);
		gnsssta.latValue = dataLst.at(10).toFloat();
		gnsssta.latitude = dataLst.at(11);
		gnsssta.elevation = dataLst.at(12).toFloat();
		gnsssta.pdop = dataLst.at(13).toFloat();
		gnsssta.rmcStateFlag = dataLst.at(14).toInt();
		gnsssta.snrFlag = dataLst.at(15).toInt();
		emit gnssstaSignal(gnsssta);
		return;
	}
	if (strData.contains(strKeys[1])) {
		st_Gnssgsv gnssgsv;
		QStringList dataLst = strData.split(SepCharComma);
		strcpy_s(gnssgsv.satelliteType, 4, qPrintable(dataLst.at(3)));
		gnssgsv.frameTotal = dataLst.at(4).toInt();
		gnssgsv.frameNo = dataLst.at(5).toInt();
		gnssgsv.satellitesTotal = dataLst.at(6).toInt();
		gnssgsv.count = (dataLst.size() - 7) / 4;
		for (int i = 0; i < gnssgsv.count; ++i) {
			gnssgsv.satelliteNo[i] = dataLst.at(7+i * gnssgsv.count).toInt();
			gnssgsv.elevation[i] = dataLst.at(8+i * gnssgsv.count).toInt();
			gnssgsv.azimuth[i] = dataLst.at(9 + i * gnssgsv.count).toInt();
			gnssgsv.snr[i] = dataLst.at(10 + i * gnssgsv.count).toInt();
		}	
		emit gnssgsvSignal(gnssgsv);
		return;
	}	
	if (strData.contains(strKeys[0])) {
		QString ret = mid(strData, strKeys[0], 2);
		emit receiverVerSignal(ret);
		return;
	}
}

void TransportThread::HandleFrameFromNetBoardEx(const st_FrameData *pFrameData)
{
	QString strData(pFrameData->m_pFrameDataBuf);
	if (m_isMultiFrame) {
		strData = m_multiFrameBuffer;
		m_multiFrameBuffer.clear();
		m_isMultiFrame = false;
	}
	TRACE_INFO(tr("---READ---%1").arg(strData));
	strData = strData.toLower();

	if (strData.contains(m_sendingData.toLower())) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();
	}

	const char *strKeys[] = {
		"ip,1,",   // 0
		"submask,1,",
		"gateway,1,",
		"workport,1,",
		"remoteip,1,",
		"groupipaddr,1,",
		"groupport,1,",   // 6
		"broadcastipaddr,1,",
		"broadcastport,1,",
		"debugport,1,",
		"delay,1,",
		"macaddr,1,",		
		"ip,2,",  // 12
		"submask,2,",
		"gateway,2,",
		"workport,2,",
		"remoteip,2,",
		"groupipaddr,2,",
		"groupport,2,", // 18
		"broadcastipaddr,2,",
		"broadcastport,2,",
		"debugport,2,",
		"delay,2,",
		"macaddr,2,",
		"current groupport is:",  // 24
		"b20682d-net1:",  // 25
		"b20682d-net2:",  // 26
		"current group remote ip is:", // 27
		"remotegroupipaddr,1,", // 28
		"remotegroupipaddr,2,",
		"addrip,1,",
		"addrip,2,",
		"",
		"",
		"",
		"",
	};
	int found(-1);
	if (strData.startsWith(strKeys[0])) {
		st_NetInfo net1;
		int i = 0;
		net1.ip = mid(strData, strKeys[i++]);
		net1.submask = mid(strData, strKeys[i++]);
		net1.gateway = mid(strData, strKeys[i++]);
		QString workport = mid(strData, strKeys[i++]);
		int pos = workport.indexOf(SepCharComma);
		net1.worklocal = workport.mid(0, pos).toUShort();
		net1.worktarget = workport.mid(pos + 1).toUShort();
		net1.remoteip = mid(strData, strKeys[i++]);
		net1.groupip = mid(strData, strKeys[i++]);
		net1.grouplocal = mid(strData, strKeys[i++]).toUShort();
		net1.broadcastip = mid(strData, strKeys[i++]);
		net1.broadcastport = mid(strData, strKeys[i++]).toUShort();
		QString debugport = mid(strData, strKeys[i++]);
		pos = debugport.indexOf(SepCharComma);
		net1.debugrecv = debugport.mid(0, pos).toUShort();
		net1.debugsend = debugport.mid(pos + 1).toUShort();
		net1.delay = mid(strData, strKeys[i++]).toInt();
		net1.macaddr = mid(strData, strKeys[i]);

		emit net1InfoSignal(net1);
		return;
	}
	if (strData.startsWith(strKeys[12])) {
		st_NetInfo net2;
		int i = 12;
		net2.ip = mid(strData, strKeys[i++]);
		net2.submask = mid(strData, strKeys[i++]);
		net2.gateway = mid(strData, strKeys[i++]);
		QString workport = mid(strData, strKeys[i++]);
		int pos = workport.indexOf(SepCharComma);
		net2.worklocal = workport.mid(0, pos).toUShort();
		net2.worktarget = workport.mid(pos + 1).toUShort();
		net2.remoteip = mid(strData, strKeys[i++]);
		net2.groupip = mid(strData, strKeys[i++]);
		net2.grouplocal = mid(strData, strKeys[i++]).toUShort();
		net2.broadcastip = mid(strData, strKeys[i++]);
		net2.broadcastport = mid(strData, strKeys[i++]).toUShort();
		QString debugport = mid(strData, strKeys[i++]);
		pos = debugport.indexOf(SepCharComma);
		net2.debugrecv = debugport.mid(0, pos).toUShort();
		net2.debugsend = debugport.mid(pos + 1).toUShort();
		net2.delay = mid(strData, strKeys[i++]).toInt();
		net2.macaddr = mid(strData, strKeys[i]);

		emit net2InfoSignal(net2);
		return;
	}
	if (-1 != (found = strData.indexOf(strKeys[24]))) {
		QString ports = mid(strData, strKeys[24]);
		QString rcv = mid(ports, "rcv:", 0, ",");
		QString snd = mid(ports, "snd:");
		int netNum = strData.contains(strKeys[18]) ? 2 : 1;
		QString ret = QString("groupport,%1,%2,%3")
			.arg(netNum).arg(rcv).arg(snd);

		emit netResultSignal(ret);
		return;
	}
	if (strData.contains(strKeys[27])) {
		QString remoteIP = mid(strData, strKeys[27]);
		int netNum = strData.contains(strKeys[28]) ? 1 : 2;
		QString ret = QString("remotegroupipaddr,%1,%2")
			.arg(netNum).arg(remoteIP);

		emit netResultSignal(ret);
		return;
	}
	{
		do 
		{
			bool quit(false);
			for (int i = 1; i < 11; ++i) {
				if (strData.startsWith(strKeys[i])) {
					QString value = mid(strData, strKeys[i]);
					QString ret = QString("%1%2").arg(strKeys[i]).arg(value);
					emit netResultSignal(ret);
					quit = true;
					break;
				}
			}
			if (quit) break;
			for (int i = 13; i < 23; ++i) {
				if (strData.startsWith(strKeys[i])) {
					QString value = mid(strData, strKeys[i]);
					QString ret = QString("%1%2").arg(strKeys[i]).arg(value);
					emit netResultSignal(ret);
					quit = true;
					break;
				}
			}
			if (quit) break;
			for (int i = 28; i < 32; ++i) {
				if (strData.startsWith(strKeys[i])) {
					QString value = mid(strData, strKeys[i]);
					QString ret = QString("%1%2").arg(strKeys[i]).arg(value);
					emit netResultSignal(ret);
					break;
				}
			}
		} while (false);		
	}
	if (strData.contains(strKeys[25])) {
		QString ver("net1,");
		ver.append(mid(strData, strKeys[25], 2));
		emit netVerSignal(ver);
		return;
	}
	if (strData.contains(strKeys[26])) {
		QString ver("net2,");
		ver.append(mid(strData, strKeys[26], 2));
		emit netVerSignal(ver);
		return;
	}
}

void TransportThread::HandleFrameFromDisplayBoardEx(const st_FrameData *pFrameData)
{
	QString strData(pFrameData->m_pFrameDataBuf);
	if (m_isMultiFrame) {
		strData = m_multiFrameBuffer;
		m_multiFrameBuffer.clear();
		m_isMultiFrame = false;
	}
	TRACE_INFO(tr("---READ---%1").arg(strData));
	strData = strData.toLower();

	if (strData.contains(m_sendingData.toLower())) {
		m_counterMutex.lock();
		m_counter = 0;
		m_counterMutex.unlock();
	}

	const char *strKeys[] = {
		"sn",
		"b20682d-view:",
		"",
		"",
		"",
	};

	int found(-1);
	if (-1 != (found = strData.indexOf(strKeys[0]))) {
		QString ret = mid(strData, strKeys[0], 1);
		emit snResultSignal(ret);
		return;
	}
	if (strData.contains(strKeys[1])) {
		QString ret = mid(strData, strKeys[1], 2);
		emit displayVerSignal(ret);
		return;
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
			// 除了 60 00, 还必须处理 00 00 的数据
			if (g_BoardAddr[PCAddr][0] != pFrame->m_chTargetAddr
				&& g_BoardAddr[GeneralAddr][0] != pFrame->m_chTargetAddr) continue;
			// 处理多帧
			//if (0x11 == pFrame->m_chTargetUse) {
			//	m_multiFrameBuffer.append(pFrame->m_pFrameDataBuf);
			//	if (260 == pFrame->m_iFrameDataLength) {									
			//		continue;
			//	}
			//	else {					
			//		m_isMultiFrame = true;
			//	}
			//}
			if (260 == pFrame->m_iFrameDataLength) {
				m_multiFrameBuffer.append(pFrame->m_pFrameDataBuf);
				m_isMultiFrame = true;
				continue;
			}
			if (m_isMultiFrame) {
				m_multiFrameBuffer.append(pFrame->m_pFrameDataBuf);
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
				HandleFrameFromNetBoardEx(pFrame);
				continue;
			}

			if (pFrame->m_chSourceAddr == g_BoardAddr[Net2Addr][0]) {
				HandleFrameFromNetBoardEx(pFrame);
				continue;
			}

			if (pFrame->m_chSourceAddr == g_BoardAddr[ReceiverAddr][0]) {
				HandleFrameFromReceiverBoardEx(pFrame);
				continue;
			}
			
			if (pFrame->m_chSourceAddr == g_BoardAddr[DisplayAddr][0]) {
				HandleFrameFromDisplayBoardEx(pFrame);
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

void TransportThread::registerMetaType()
{
	qRegisterMetaType<st_Status>();
	qRegisterMetaType<st_Gnsstime>();
	qRegisterMetaType<st_RefAvailInfo>();
	qRegisterMetaType<st_MasterVer>();
	qRegisterMetaType<st_Gnsssta>();
	qRegisterMetaType<st_Gnssgsv>();
	qRegisterMetaType<st_NetInfo>();
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
	int iFrameNo(0x00);
	m_pMyComBasic->SetCommand(iFrameNo, iCmd);
}

void TransportThread::SetFrameDataFormat(bool bSendIs16Hex, bool bRecvIs16Hex, bool bHasFrame)
{
	m_pMyComBasic->SetSendDataIs16Hex(bSendIs16Hex);
	m_pMyComBasic->SetRecvDataIs16Hex(bRecvIs16Hex);
	m_pMyComBasic->SetDataHasFrame(bHasFrame);
}

void TransportThread::SendComNetData(unsigned char chCommand, const char* pszDataBuf, int iDataLength, bool bResend/* = false*/)
{
	if (0 != m_counter) {
		emit exceptionSignal(BlockException);
		return;
	}
	
	m_pMyComBasic->SendComNetData(chCommand, pszDataBuf, iDataLength);
	m_sendingData = QString::fromLatin1(pszDataBuf, iDataLength);
	if (bResend) {
		m_counterTimer->start();
		++m_counter;
	}	

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


/****************************************全局函数****************************************/
void FrameHanleFunc(const st_FrameData* pFrameData)
{
	TransportThread::Get()->FrameHandleFunc(*pFrameData);
}