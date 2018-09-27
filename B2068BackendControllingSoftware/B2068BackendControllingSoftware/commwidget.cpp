#include "stdafx.h"
#include "commwidget.h"
#include "transportthread.h"
#include "timepositiondatabase.h"
#include <QSerialPortInfo>

// --------------------------------------------------------------------------------------
#pragma comment(lib, "ws2_32.lib")  // WSAStartup  WSACleanup

// --------------------------------------------------------------------------------------
const QString ComOpen = QObject::tr("打开串口");
const QString ComClose = QObject::tr("关闭串口");
const QString NetOpen = QObject::tr("打开网口");
const QString NetClose = QObject::tr("关闭网口");

// --------------------------------------------------------------------------------------
un_ConnectInfo connInfo;

CommWidget::CommWidget(QWidget *parent)
	: QWidget(parent)
{	
	
	createWidgets();
	
	setTextAlignCenter(m_comName);
	setTextAlignCenter(m_comBaud);
	QString objName("comm");
	setObjectName(objName);	
	setStyleSheet(QSS_CommWidget.arg(objName));	
	
	m_bNetUsable = true;
	// For net connecting, init socket
	WSADATA wsaData;
	if (0 != WSAStartup(WINSOCK_VERSION, &wsaData)) {
		m_bNetUsable = false;
	}

	TransportThread::Get()->start();
	m_bConnectFailed = false;
	m_bHasResult = false;
}

CommWidget::~CommWidget()
{
	WSACleanup();

	// 在销毁QThread对象之前, 必须停止线程
	TransportThread::Get()->stop();  
}

void CommWidget::resizeEvent(QResizeEvent *event)
{
	int left = width() / 4;
	int top = (m_netGroup->height() - LblHeight * 5) / 2;
	int wdth = LblWidth * 1.5;
	int hght = LblHeight * 1.5;
	int wdthex = LblWidth * 1.8;

	m_comNameLabel->setGeometry(left, top, LblWidth, hght);
	m_comName->setGeometry(left + LblWidth * 1.5, top, wdth, hght);
	m_comBaudLabel->setGeometry(left, top + LblHeight * 2, LblWidth, hght);
	m_comBaud->setGeometry(left + LblWidth * 1.5, top + LblHeight * 2, wdth, hght);
	m_connCom->setGeometry(left + LblWidth * 2.5, top + LblHeight * 4.5, wdth, hght);

	m_addrLabel->setGeometry(left, top, LblWidth, hght);
	m_addr->setGeometry(left + LblWidth * 1.5, top, wdthex, hght);
	m_recvPortLabel->setGeometry(left, top + LblHeight * 2, LblWidth, hght);
	m_recvPort->setGeometry(left + LblWidth * 1.5, top + LblHeight * 2, wdthex, hght);
	//m_sendPortLabel->setGeometry(left, top + LblHeight * 4, LblWidth, hght);
	//m_sendPort->setGeometry(left + LblWidth * 1.5, top + LblHeight * 4, wdthex, hght);
	m_connNet->setGeometry(left + LblWidth * 2.5, top + LblHeight * 4.5, wdth, hght);
}

void CommWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	//QWidget::paintEvent(event);
}

void CommWidget::setTextAlignCenter(QComboBox *comboBox)
{
	int items = comboBox->count();
	if (0 == items) return;

	for (int i = 0; i < items; ++i) {
		static_cast<QStandardItemModel *>(comboBox->model())->item(i)->setTextAlignment(Qt::AlignCenter);
	}
}

QGroupBox * CommWidget::createComInfo()
{
	QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
	QStringList nameList;
	for (auto &info : infoList) {
		nameList.append(info.portName()); 
	}	
	QStringList baudList = { "9600", "19200", "57600" };

	m_comGroup = new QGroupBox(tr("串口设置"));
	m_comGroup->setCheckable(true);
	m_comGroup->setAlignment(Qt::AlignCenter);	

	m_comNameLabel = new QLabel(tr("串口号"), m_comGroup);
	m_comName = new QComboBox(m_comGroup);
	m_comName->addItems(nameList);	

	m_comBaudLabel = new QLabel(tr("波特率"), m_comGroup);
	m_comBaud = new QComboBox(m_comGroup);
	m_comBaud->addItems(baudList);
	m_connCom = new QPushButton(ComOpen, m_comGroup);	

	return m_comGroup;
}

QGroupBox * CommWidget::createNetInfo()
{
	m_netGroup = new QGroupBox(tr("网口设置"));
	m_netGroup->setCheckable(true);
	m_netGroup->setAlignment(Qt::AlignCenter);	
	
	m_addrLabel = new QLabel(tr("IP地址"), m_netGroup);
	m_addr = new QLineEdit(m_netGroup);	
	m_recvPortLabel = new QLabel(tr("IP端口"), m_netGroup);
	m_recvPort = new QLineEdit(tr("20212"), m_netGroup);		
	//m_sendPortLabel = new QLabel(tr("发送端口"), m_netGroup);
	//m_sendPort = new QLineEdit(m_netGroup);	
	m_connNet = new QPushButton(NetOpen, m_netGroup);	

	return m_netGroup;
}

void CommWidget::connectSlots()
{
	TransportThread *pTransport = TransportThread::Get();
	connect(m_comGroup, SIGNAL(clicked()), this, SLOT(slotOnSelectCom()));
	connect(m_netGroup, SIGNAL(clicked()), this, SLOT(slotOnSelectNet()));
	connect(m_connCom, SIGNAL(clicked()), this, SLOT(slotOnConnectCom()));
	connect(m_connNet, SIGNAL(clicked()), this, SLOT(slotOnConnectNet()));
	connect(pTransport, SIGNAL(qumarkSignal(QString)), this, SLOT(slotOnSendResult(QString)));
	connect(pTransport, SIGNAL(exceptionSignal(int)), this, SLOT(slotOnSendTimeOut(int)));
}

void CommWidget::setValidator()
{
	QRegExp rxIP("^(?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))$");
	QValidator *validatorIP = new QRegExpValidator(rxIP);
	m_addr->setValidator(validatorIP);

	QRegExp rxPort("^(?:102[5-9]|10[3-9]\\d|1[1-9]\\d{2}|[2-9]\\d{3}|[1-5]\\d{4}|6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])$");
	QValidator *validatorPort = new QRegExpValidator(rxPort);
	m_recvPort->setValidator(validatorPort);
	//m_sendPort->setValidator(validatorPort);
}

void CommWidget::createWidgets()
{
	m_statusLabel = new QLabel(tr("测试"));
	m_statusLabel->setAlignment(Qt::AlignCenter);
	m_statusLabel->setProperty("statusFlag", true);
	
	auto baseLayout = new QVBoxLayout(this);
	baseLayout->addWidget(createComInfo(), 6);
	baseLayout->addWidget(createNetInfo(), 6);
	baseLayout->addWidget(m_statusLabel, 1);
	baseLayout->setContentsMargins(0, 10, 0, 0);
	setLayout(baseLayout);

	m_bUseCom = true;
	m_netGroup->setChecked(false);

	connectSlots();
	setValidator();
}

void CommWidget::checkConnect()
{
	m_bConnectFailed = false;
	m_bHasResult = false;
	
	//TransportThread *pTransport = TransportThread::Get();
	//pTransport->SetFrameDataFormat(false, false, true);
	//pTransport->SetSourceAddr(g_BoardAddr[PCAddr][0], g_BoardAddr[PCAddr][1], 0x00);
	//pTransport->SetTargetAddr(g_BoardAddr[MasterControlAddr][0], g_BoardAddr[MasterControlAddr][1], 0x11);
	//pTransport->SetCommand(COMMAND_IS_AT);

	//char chTest[16] = "?";
	//pTransport->SendComNetData(COMMAND_IS_AT, chTest, strlen(chTest));
	TimePositionDatabase db;
	char chTest[16] = "?";
	db.selectFromMasterBoard(COMMAND_IS_AT, chTest);

	// 串口和网口的反应最快为 400ms
	QTimer::singleShot(400, this, &CommWidget::slotOnCheckTimeOut);
}

void CommWidget::slotOnSelectCom()
{
	if (m_bUseCom) {
		m_comGroup->setChecked(true);
		return;
	}

	m_netGroup->setChecked(false);

	m_bUseCom = true;
	TransportThread::Get()->CloseNet();
	m_connCom->setEnabled(true);
}

void CommWidget::slotOnSelectNet()
{
	if (!m_bUseCom) {
		m_netGroup->setChecked(true);
		return;
	}

	m_comGroup->setChecked(false);

	m_bUseCom = false;
	TransportThread::Get()->CloseCom();
	m_connNet->setEnabled(true);
}

void CommWidget::slotOnConnectCom()
{
	TransportThread *pTransport = TransportThread::Get();
	m_statusLabel->clear();

	if (0 == m_connCom->text().compare(ComClose)) {
		pTransport->CloseCom();
		m_statusLabel->setText(tr("串口已经关闭"));
		m_connCom->setText(ComOpen);
		return;
	}	

	QString comName = m_comName->currentText();
	int baud = m_comBaud->currentText().toInt();
		
	m_connCom->setEnabled(false);
	pTransport->SetComPort(comName, baud, 8, 1, 0);
	if (!pTransport->OpenCom()) {
		m_statusLabel->setText(tr("连接串口失败!"));
		m_connCom->setEnabled(true);
	}
	else {
		checkConnect();
		//m_statusLabel->setText(tr("连接串口成功!"));
		//m_connCom->setText(ComClose);
		//emit connectSuccess();
	}
	//m_connCom->setEnabled(true);
	strcpy_s(connInfo.comConn.comName, 64, comName.toLatin1().data());
	connInfo.comConn.baud = baud;
}

void CommWidget::slotOnConnectNet()
{
	m_statusLabel->clear();
	if (!m_bNetUsable) {
		WSADATA wsaData;
		if (0 != WSAStartup(WINSOCK_VERSION, &wsaData)) {
			m_statusLabel->setText(tr("当前网络不可用, 请先检查网络!"));
			return;
		}
		else
			m_bNetUsable = true;
	}
	
	TransportThread *pTransport = TransportThread::Get();

	if (0 == m_connNet->text().compare(NetClose)) {
		pTransport->CloseNet();
		m_statusLabel->setText(tr("网口已经关闭"));
		m_connCom->setText(NetOpen);
		return;
	}


	QString addr = m_addr->text();
	int port = m_recvPort->text().toInt();
	m_connNet->setEnabled(false);
	pTransport->SetNetPort(addr, port);
	if (!pTransport->OpenNet()) {
		m_statusLabel->setText(tr("连接网口失败!"));
		m_connNet->setEnabled(true);
	}
	else {
		//m_statusLabel->setText(tr("连接网口成功!"));
		//m_connNet->setText(NetClose);
		//emit connectSuccess();
		checkConnect();
	}
	//m_connNet->setEnabled(true);
	strcpy_s(connInfo.netConn.netIP, 64, addr.toLatin1().data());
	connInfo.netConn.netPort = port;
}

void CommWidget::slotOnSendResult(const QString &ret)
{
	if (m_bHasResult) return;
	m_bHasResult = true;

	QString strRet = ret;
	if (!m_bConnectFailed && 
		strRet.contains("- look input or output message"/*"look ------- look input or output message"*/, Qt::CaseInsensitive)) {
		if (m_bUseCom) {
			m_statusLabel->setText(tr("连接串口成功!"));
			m_connCom->setText(ComClose);
			m_connCom->setEnabled(true);
		}
		else {
			m_statusLabel->setText(tr("连接网口成功!"));
			m_connNet->setText(NetClose); 
			m_connNet->setEnabled(true);
		}
		emit connectSuccess();
	}
	else {
		if (m_bUseCom) {
			m_statusLabel->setText(tr("连接串口失败: 波特率不匹配!"));
			m_connCom->setEnabled(true);
		}
		else {
			m_statusLabel->setText(tr("连接网口失败!"));
			m_connNet->setEnabled(true);
		}
	}
}

void CommWidget::slotOnSendTimeOut(int excp)
{
	switch (excp) {
	case SendFailedException:
		m_bConnectFailed = true;
		break;
	default:
		break;
	}
}

void CommWidget::slotOnCheckTimeOut()
{
	if (m_bHasResult) return;
	m_bHasResult = true;
		
	TransportThread *pTransport = TransportThread::Get();
	if (m_bUseCom) {
		pTransport->CloseCom();
		m_statusLabel->setText(tr("连接串口失败: 未收到数据!"));
		m_connCom->setEnabled(true);
	}
	else {
		pTransport->CloseNet();
		m_statusLabel->setText(tr("连接网口失败: 未收到数据!"));
		m_connNet->setEnabled(true);
	}
}
