#include "stdafx.h"
#include "tabwidget.h"
#include "stateparamswidget.h"



/****************************************************************************************
主窗口
*****************************************************************************************/
TabWidget::TabWidget(QWidget *parent)
	: QDialog(parent)
{
	//ui.setupUi(this);
	resize(600, 400);

	m_tabWidget = new QTabWidget;
	m_tabWidget->addTab(new MainTab(), tr("主界面"));
	m_tabWidget->addTab(new TimeSrcTab(), tr("时间源"));
	m_tabWidget->addTab(new ComSettingsTab(), tr("串口设置"));
	m_tabWidget->addTab(new NetSettingsTab(), tr("网口设置"));
	m_tabWidget->addTab(new StateParamsTab(), tr("状态参数"));	

	auto baseLayout = new QVBoxLayout;
	baseLayout->addWidget(m_tabWidget);
	baseLayout->setMargin(0);
	baseLayout->setSpacing(0);
	setLayout(baseLayout);
	
	if (windowState() | Qt::WindowMaximized)
		setWindowState(windowState() & ~Qt::WindowMaximized);

	setStyleSheet(QSS_TabWidget);
	setWindowTitle(tr("B2068控制软件"));
	
	// 测试
	m_tabWidget->setCurrentIndex(4);
}

TabWidget::~TabWidget()
{
}


/****************************************************************************************
主界面
*****************************************************************************************/
MainTab::MainTab(QWidget *parent)
	: QWidget(parent)
{
	int w = width();
	int h = height();	

	int frameStyle = QFrame::NoFrame;

	QString smallLabelQss("smalllabel");
	QString mediumLabelQss("mediumlabel");
	QString largeLabelQss("largelabel");	

	m_deviceModel = new QLabel(tr("B2068-2D"), this);	
	m_deviceModel->setObjectName(smallLabelQss);	// 特定文字样式的标签名称
	m_deviceModel->setFrameStyle(frameStyle);		// 标签样式
	m_deviceModel->setScaledContents(true);			// 随内容调整大小
	m_deviceModel->setGeometry(LeftMargin, TopMargin, LblWidth, LblHeight);	// 相对位置
	//m_deviceModel->setSizePolicy()

	m_refSrc = new QLabel(tr("GLONASS"), this);
	m_refSrc->setObjectName(smallLabelQss);
	m_refSrc->setFrameStyle(frameStyle);
	m_refSrc->setScaledContents(true);
	m_refSrc->setGeometry(w - LeftMargin * 10.5, TopMargin, LblWidth, LblHeight);

	m_avlSatellites = new QLabel(tr("12"), this);
	m_avlSatellites->setObjectName(smallLabelQss);
	m_avlSatellites->setFrameStyle(frameStyle);
	m_avlSatellites->setScaledContents(true);
	m_avlSatellites->setGeometry(w - LeftMargin * 6, TopMargin, LblWidth, LblHeight);

	m_signalState = new QLabel(this);
	m_signalState->setPixmap(QPixmap(":/BackendControlling/images/antenna3.png", "PNG"));
	m_signalState->setFrameStyle(frameStyle);
	m_signalState->setScaledContents(true);
	m_signalState->setGeometry(w - LeftMargin * 4, TopMargin, LblHeight, LblHeight);

	m_lockedState = new QLabel(this);	
	m_lockedState->setPixmap(QPixmap(":/BackendControlling/images/lock_white_close.png", "PNG"));
	m_lockedState->setFrameStyle(frameStyle);
	m_lockedState->setScaledContents(true);
	m_lockedState->setGeometry(w - LeftMargin * 2, TopMargin, LblHeight, LblHeight);
	
	m_timeName = new QLabel(tr("北京时间"), this);
	m_timeName->setObjectName(mediumLabelQss);
	m_timeName->setFrameStyle(frameStyle);
	m_timeName->setScaledContents(true);
	m_timeName->setGeometry(LblWidth, LblWidth, LblWidth * 2, LblHeight);

	m_refSrcTime = new QLabel(tr("12:30:50"), this);
	m_refSrcTime->setObjectName(largeLabelQss);
	m_refSrcTime->setFrameStyle(frameStyle);
	m_refSrcTime->setScaledContents(true);
	m_refSrcTime->setGeometry((w - LblWidth * 5) / 2, (h - LblHeight * 6) / 2, LblWidth * 6, LblHeight * 4);

	m_refSrcDate = new QLabel(tr("2018 年 08 月 08 日"), this);
	m_refSrcDate->setObjectName(mediumLabelQss);
	m_refSrcDate->setFrameStyle(frameStyle);
	m_refSrcDate->setScaledContents(true);
	m_refSrcDate->setGeometry((w - LblWidth * 2) / 2, (h * 2) / 3, LblWidth * 3, LblHeight);

	m_position = new QLabel(tr("东经 113°56′51″  北纬 22°33′57″  高程 63.625 米"), this);
	m_position->setObjectName(mediumLabelQss);
	m_position->setFrameStyle(frameStyle);
	m_position->setScaledContents(true);
	m_position->setGeometry(LeftMargin, h - BottonMargin * 1.5, w - LeftMargin, LblHeight);
	
	QString objName("maintab");
	setObjectName(objName);	
	setStyleSheet(QSS_MainTabBackground.arg(objName) 
		+ QSS_YaHeiMainTabLabel.arg(smallLabelQss).arg(mediumLabelQss).arg(largeLabelQss));
	
	// 测试	
	// 设置精度，最精确的一个
	QString curDate = QDate::currentDate().toString("yyyy 年 MM 月 dd 日");
	m_refSrcDate->setText(curDate);
	m_refSrcTimer.setTimerType(Qt::PreciseTimer);  
	connectSlots();
	m_refSrcTimer.start(100);
}

void MainTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}		

	m_deviceModel->setGeometry(LeftMargin, TopMargin, LblWidth, LblHeight);
	m_refSrc->setGeometry(w - LeftMargin * 10.5, TopMargin, LblWidth, LblHeight);
	m_avlSatellites->setGeometry(w - LeftMargin * 6, TopMargin, LblWidth, LblHeight);
	m_signalState->setGeometry(w - LeftMargin * 4, TopMargin, LblHeight, LblHeight);
	m_lockedState->setGeometry(w - LeftMargin * 2, TopMargin, LblHeight, LblHeight);
	m_timeName->setGeometry(LblWidth, LblWidth, LblWidth * 2, LblHeight);
	m_refSrcTime->setGeometry((w - LblWidth * 5) / 2, (h - LblHeight * 6) / 2, LblWidth * 6, LblHeight * 4);
	m_refSrcDate->setGeometry((w - LblWidth * 2) / 2, (h * 2) / 3, LblWidth * 3, LblHeight);
	m_position->setGeometry(LeftMargin, h - BottonMargin * 1.5, w - LeftMargin, LblHeight);
}

// 解决从QWidget派生的简单类 通过setStyleSheet设置背景无效 的问题
void MainTab::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void MainTab::connectSlots()
{
	//connect(&m_refSrcTimer, &QTimer::timeout, this, &MainTab::slotOnRefSrcTimeOut);	
	QMetaObject::Connection result = connect(&m_refSrcTimer, SIGNAL(timeout()), this, SLOT(slotOnRefSrcTimeOut()));
	if (Q_NULLPTR == result) {
		QMessageBox::warning(this, "failed", "connect timer's timeout failed.");
	}
}

void MainTab::slotOnRefSrcTimeOut()
{
	static QString lastTime("");
	QString curTime = QTime::currentTime().toString("hh:mm:ss");
	if (0 != curTime.compare(lastTime)) {
		m_refSrcTime->setText(curTime);
		lastTime.swap(curTime);
	}
}

/****************************************************************************************
时间源
*****************************************************************************************/
TimeSrcTab::TimeSrcTab(QWidget *parent)
	: QWidget(parent)
{		
	const QSize LockSize(20, 20);
	const int RowSpan(1), MinRowHeight(40);	
	
	QStringList priority;
	priority << "1" << "2" << "3" << "4" << "5";
	
	QString largeRadioQss("largeradio");
	QString smallRadioQss("smallradio");
	QString normalLabelQss("normallabel");
	QString boldLabelQss("boldlabel");


	m_manual = new QRadioButton(tr("手动"), this);
	m_manual->setObjectName(largeRadioQss);

	m_auto = new QRadioButton(tr("自动"), this);
	m_auto->setObjectName(largeRadioQss);
	

	m_bdsLock = new QLabel(this);		
	m_bdsLock->setScaledContents(true);
	m_bdsLock->setFixedSize(LockSize);
	m_bdsLock->setPixmap(QPixmap(":/BackendControlling/images/lock_black_close.png", "PNG"));
		
	m_bds = new QRadioButton(tr("北斗"), this);
	m_bds->setObjectName(smallRadioQss);

	m_bdsDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_bdsDateTime->setObjectName(normalLabelQss);
	m_bdsDateTime->setScaledContents(true);

	m_bdsAvlSatellites = new QLabel(tr("12 颗"), this);
	m_bdsAvlSatellites->setObjectName(normalLabelQss);

	m_bdsPriority = new QComboBox(this);
	m_bdsPriority->addItems(priority);


	m_gpsLock = new QLabel(this);
	m_gpsLock->setScaledContents(true);
	m_gpsLock->setFixedSize(LockSize);
	m_gpsLock->setPixmap(QPixmap(":/BackendControlling/images/lock_black_open.png", "PNG"));

	m_gps = new QRadioButton(tr("GPS"), this);
	m_gps->setObjectName(smallRadioQss);

	m_gpsDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_gpsDateTime->setObjectName(normalLabelQss);
	m_gpsDateTime->setScaledContents(true);

	m_gpsAvlSatellites = new QLabel(tr("10 颗"), this);
	m_gpsAvlSatellites->setObjectName(normalLabelQss);

	m_gpsPriority = new QComboBox(this);
	m_gpsPriority->addItems(priority);


	m_gloLock = new QLabel(this);
	m_gloLock->setScaledContents(true);
	m_gloLock->setFixedSize(LockSize);

	m_glo = new QRadioButton(tr("GLO"), this);
	m_glo->setObjectName(smallRadioQss);

	m_gloDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_gloDateTime->setObjectName(normalLabelQss);
	m_gloDateTime->setScaledContents(true);

	m_gloAvlSatellites = new QLabel(tr("11 颗"), this);
	m_gloAvlSatellites->setObjectName(normalLabelQss);

	m_gloPriority = new QComboBox(this);
	m_gloPriority->addItems(priority);


	m_dcbLock = new QLabel(this);
	m_dcbLock->setScaledContents(true);
	m_dcbLock->setFixedSize(LockSize);

	m_dcb = new QRadioButton(tr("直流B码"), this);
	m_dcb->setObjectName(smallRadioQss);

	m_dcbDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_dcbDateTime->setObjectName(normalLabelQss);
	m_dcbDateTime->setScaledContents(true);

	m_dcbPriority = new QComboBox(this);
	m_dcbPriority->addItems(priority);


	m_acbLock = new QLabel(this);
	m_acbLock->setScaledContents(true);
	m_acbLock->setFixedSize(LockSize);

	m_acb = new QRadioButton(tr("交流B码"), this);
	m_acb->setObjectName(smallRadioQss);

	m_acbDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_acbDateTime->setObjectName(normalLabelQss);
	m_acbDateTime->setScaledContents(true);

	m_acbPriority = new QComboBox(this);
	m_acbPriority->addItems(priority);


	m_input = new QRadioButton(tr("输入时间"), this);
	m_input->setObjectName(smallRadioQss);

	m_inputDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_inputDateTime->setObjectName(normalLabelQss);
	m_inputDateTime->setScaledContents(true);


	QLabel *inputLabel = new QLabel(tr("输入时间设置"), this);
	inputLabel->setObjectName(boldLabelQss);

	m_inputSetting = new QLineEdit(tr("20180817113510"), this);
	m_inputSetting->setAlignment(Qt::AlignCenter);
	m_inputSetting->setPlaceholderText("YYYYMMDDhhmmss");
	QRegExp rx("2\\d{3}(0[1-9]|1[0-2])(0[1-9]|[12]\\d|3[01])([01]\\d|2[0-3])([0-5]\\d){2}");
	QValidator *validator = new QRegExpValidator(rx, this);
	m_inputSetting->setValidator(validator);	

	m_confirm = new QPushButton(tr("确认\n设置"), this);

	QLabel *priorityLabel = new QLabel(tr("优先级"), this);
	priorityLabel->setObjectName(boldLabelQss);

	
	// 分组
	QButtonGroup *operatingGroup = new QButtonGroup(this);
	operatingGroup->addButton(m_manual);
	operatingGroup->addButton(m_auto);
	m_manual->setChecked(true);

	m_refSrcGroup = new QButtonGroup(this);
	m_refSrcGroup->addButton(m_bds);
	m_refSrcGroup->addButton(m_gps);
	m_refSrcGroup->addButton(m_glo);
	m_refSrcGroup->addButton(m_dcb);
	m_refSrcGroup->addButton(m_acb);
	m_refSrcGroup->addButton(m_input);
	m_bds->setChecked(true);

	m_priorityGroup[0] = m_bdsPriority;
	m_priorityGroup[1] = m_gpsPriority;
	m_priorityGroup[2] = m_gloPriority;
	m_priorityGroup[3] = m_dcbPriority;
	m_priorityGroup[4] = m_acbPriority;


	auto topLayout = new QHBoxLayout;
	topLayout->addStretch(1);
	topLayout->addWidget(m_manual, 1);
	//topLayout->addStretch(1);
	topLayout->addWidget(m_auto, 1);
	topLayout->addStretch(1);

	auto mainLayout = new QGridLayout;
	mainLayout->addWidget(priorityLabel, 0, 4, RowSpan, 1);
	mainLayout->addWidget(m_bdsLock, 1, 0, RowSpan, 1);
	mainLayout->addWidget(m_bds, 1, 1, RowSpan, 1);
	mainLayout->addWidget(m_bdsDateTime, 1, 2, RowSpan, 1);
	mainLayout->addWidget(m_bdsAvlSatellites, 1, 3, RowSpan, 1);
	mainLayout->addWidget(m_bdsPriority, 1, 4, RowSpan, 1);
	mainLayout->addWidget(m_gpsLock, 2, 0, RowSpan, 1);
	mainLayout->addWidget(m_gps, 2, 1, RowSpan, 1);
	mainLayout->addWidget(m_gpsDateTime, 2, 2, RowSpan, 1);
	mainLayout->addWidget(m_gpsAvlSatellites, 2, 3, RowSpan, 1);
	mainLayout->addWidget(m_gpsPriority, 2, 4, RowSpan, 1);
	mainLayout->addWidget(m_gloLock, 3, 0, RowSpan, 1);
	mainLayout->addWidget(m_glo, 3, 1, RowSpan, 1);
	mainLayout->addWidget(m_gloDateTime, 3, 2, RowSpan, 1);
	mainLayout->addWidget(m_gloAvlSatellites, 3, 3, RowSpan, 1);
	mainLayout->addWidget(m_gloPriority, 3, 4, RowSpan, 1);
	mainLayout->addWidget(m_dcbLock, 4, 0, RowSpan, 1);
	mainLayout->addWidget(m_dcb, 4, 1, RowSpan, 1);
	mainLayout->addWidget(m_dcbDateTime, 4, 2, RowSpan, 1);
	mainLayout->addWidget(m_dcbPriority, 4, 4, RowSpan, 1);
	mainLayout->addWidget(m_acbLock, 5, 0, RowSpan, 1);
	mainLayout->addWidget(m_acb, 5, 1, RowSpan, 1);
	mainLayout->addWidget(m_acbDateTime, 5, 2, RowSpan, 1);
	mainLayout->addWidget(m_acbPriority, 5, 4, RowSpan, 1);
	mainLayout->addWidget(m_input, 6, 1, RowSpan, 1);
	mainLayout->addWidget(m_inputDateTime, 6, 2, RowSpan, 1);
	mainLayout->addWidget(m_confirm, 6, 4, RowSpan+2, 1);
	mainLayout->addWidget(inputLabel, 7, 1, RowSpan, 1);
	mainLayout->addWidget(m_inputSetting, 7, 2, RowSpan, 1);	
	// 设置行距
	mainLayout->setRowMinimumHeight(1, MinRowHeight);
	mainLayout->setRowMinimumHeight(2, MinRowHeight);
	mainLayout->setRowMinimumHeight(3, MinRowHeight);
	mainLayout->setRowMinimumHeight(4, MinRowHeight);
	mainLayout->setRowMinimumHeight(5, MinRowHeight);
	mainLayout->setRowMinimumHeight(6, MinRowHeight);
	mainLayout->setRowMinimumHeight(7, MinRowHeight);


	auto baseLayout = new QVBoxLayout;
	//baseLayout->addStretch(1);
	baseLayout->addLayout(topLayout, 1);
	baseLayout->addLayout(mainLayout, 8);
	//baseLayout->addStretch(1);
	baseLayout->setMargin(15);
	setLayout(baseLayout);

	setStyleSheet(QSS_YaHeiTimeSrcRadio.arg(largeRadioQss).arg(smallRadioQss)
		+ QSS_TimeSrcLabel.arg(normalLabelQss).arg(boldLabelQss));

	connectSlots();
	switchAutoManual(false, true);
}

void TimeSrcTab::connectSlots()
{
	connect(m_auto, SIGNAL(toggled(bool)), this, SLOT(slotOnSwitchAutoManual(bool)));
	connect(m_bdsPriority, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotOnCurrentIndexChanged(QString)));
	connect(m_gpsPriority, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotOnCurrentIndexChanged(QString)));
	connect(m_gloPriority, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotOnCurrentIndexChanged(QString)));
	connect(m_dcbPriority, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotOnCurrentIndexChanged(QString)));
	connect(m_acbPriority, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotOnCurrentIndexChanged(QString)));
}

void TimeSrcTab::switchAutoManual(bool isAuto, bool isInitial /*= false*/)
{
	bool isManual = !isAuto;
	QList<QAbstractButton *> refSrcButtons = m_refSrcGroup->buttons();
	for (auto &button : refSrcButtons) {
		button->setEnabled(isManual);
	}

	for (int i = 0; i < 5; ++i) {
		m_priorityGroup[i]->setEnabled(isAuto);
		if (isInitial) {
			m_priorityGroup[i]->setCurrentIndex(i);
			m_priorityValue[i] = i + 1;
		}
	}
}

void TimeSrcTab::slotOnSwitchAutoManual(bool checked)
{
	if (checked) {
		switchAutoManual(true);

	} else {
		switchAutoManual(false);
	}
}

void TimeSrcTab::slotOnCurrentIndexChanged(const QString& text)
{	
	int idx(0), value(0);
	for (int i = 0; i < 5; ++i) {
		if ( 0 == text.compare(m_priorityGroup[i]->currentText()) ) {
			if (m_priorityValue[i] != text.toInt()) {
				idx = i;
				value = m_priorityValue[i];
				break;
			}
		}
	}
	for (int i = 0; i < 5; ++i) {
		if (0 == text.compare(m_priorityGroup[i]->currentText())) {
			if (m_priorityValue[i] == text.toInt()) {
				m_priorityGroup[i]->setCurrentIndex(value - 1);
				m_priorityValue[i] = value;
				break;
			}
		}
	}
	m_priorityValue[idx] = text.toInt();
}

/****************************************************************************************
串口设置
*****************************************************************************************/
ComSettingsTab::ComSettingsTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	int w = width();
	int h = height();
	
	QStringList baud;
	baud << QString::number(QSerialPort::Baud9600) 
		<< QString::number(QSerialPort::Baud19200) 
		<< QString::number(QSerialPort::Baud57600);

	m_debugLabel = new QLabel(tr("调试串口"), this);
	m_debugLabel->setGeometry(w / 4, (h - m_lblHeight * 8) / 2, m_lblWidth, m_lblHeight);

	m_debugCom = new QComboBox(this);
	m_debugCom->addItems(baud);
	m_debugCom->setCurrentIndex(2);
	m_debugCom->setGeometry(w / 2, (h - m_lblHeight * 8) / 2, m_lblWidth, m_lblHeight);

	m_firstLabel = new QLabel(tr("定时/串口1"), this);
	m_firstLabel->setGeometry(w / 4, (h - m_lblHeight * 4) / 2, m_lblWidth, m_lblHeight);

	m_timingFirst = new QComboBox(this);
	m_timingFirst->addItems(baud);
	m_timingFirst->setGeometry(w / 2, (h - m_lblHeight * 4) / 2, m_lblWidth, m_lblHeight);

	m_secondLabel = new QLabel(tr("定时/串口2"), this);
	m_secondLabel->setGeometry(w / 4, h / 2, m_lblWidth, m_lblHeight);

	m_timingSecond = new QComboBox(this);
	m_timingSecond->addItems(baud);
	m_timingSecond->setGeometry(w / 2, h / 2, m_lblWidth, m_lblHeight);

	m_confirm = new QPushButton(tr("确认设置"), this);
	m_confirm->setGeometry(w / 2 + m_lblWidth / 2, h * 3 / 4 - m_lblHeight, m_lblWidth, m_lblHeight);

	setStyleSheet(QSS_ComSettingsLabel);
}

void ComSettingsTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	m_debugLabel->setGeometry(w / 4, (h - m_lblHeight * 8) / 2, m_lblWidth, m_lblHeight);
	m_debugCom->setGeometry(w / 2, (h - m_lblHeight * 8) / 2, m_lblWidth, m_lblHeight);
	m_firstLabel->setGeometry(w / 4, (h - m_lblHeight * 4) / 2, m_lblWidth, m_lblHeight);
	m_timingFirst->setGeometry(w / 2, (h - m_lblHeight * 4) / 2, m_lblWidth, m_lblHeight);
	m_secondLabel->setGeometry(w / 4, h / 2, m_lblWidth, m_lblHeight);
	m_timingSecond->setGeometry(w / 2, h / 2, m_lblWidth, m_lblHeight);
	m_confirm->setGeometry(w / 2 + m_lblWidth / 2, h * 3 / 4 - m_lblHeight, m_lblWidth, m_lblHeight);
}

/****************************************************************************************
网口设置
*****************************************************************************************/
UnicastWidget::UnicastWidget(QWidget *parent /*= 0*/)
	: QWidget(parent)
{
	auto localIPLabel = new QLabel(tr("本机IP"), this);
	m_localIP = new QLineEdit(tr("192.168.59.180"), this);
	m_localIP->setAlignment(Qt::AlignCenter);
	m_localIP->setMaximumWidth(LeWidth);
	m_ipConfirm = new QPushButton(tr("确认设置"), this);	

	auto localPortLabel = new QLabel(tr("本机端口"), this);
	m_localPort = new QLineEdit(tr("6666"), this);
	m_localPort->setAlignment(Qt::AlignCenter);
	m_localPort->setMaximumWidth(LeWidth);
	m_portConfirm = new QPushButton(tr("确认设置"), this);

	auto submaskLabel = new QLabel(tr("本机掩码"), this);
	m_submask = new QLineEdit(tr("255.255.255.0"), this);
	m_submask->setAlignment(Qt::AlignCenter);
	m_submask->setMaximumWidth(LeWidth);
	m_maskConfirm = new QPushButton(tr("确认设置"), this);

	auto gatewayLabel = new QLabel(tr("本机网关"), this);
	m_gateway = new QLineEdit(tr("192.168.59.1"), this);
	m_gateway->setAlignment(Qt::AlignCenter);
	m_gateway->setMaximumWidth(LeWidth);
	m_gatewayConfirm = new QPushButton(tr("确认设置"), this);

	auto remoteIPLabel = new QLabel(tr("目标IP"), this);
	m_remoteIP = new QLineEdit(tr("192.168.59.117"), this);
	m_remoteIP->setAlignment(Qt::AlignCenter);
	m_remoteIP->setMaximumWidth(LeWidth);
	m_remoteIPConfirm = new QPushButton(tr("确认设置"), this);

	auto remotePortLabel = new QLabel(tr("目标端口"), this);
	m_remotePort = new QLineEdit(tr("8888"), this);
	m_remotePort->setAlignment(Qt::AlignCenter);
	m_remotePort->setMaximumWidth(LeWidth);
	m_remotePortConfirm = new QPushButton(tr("确认设置"), this);

	auto delayLabel = new QLabel(tr("网络延时"), this);
	m_delay = new QLineEdit(tr("33"), this);
	m_delay->setAlignment(Qt::AlignCenter);
	m_delay->setMaximumWidth(LeWidth * 3 / 4);
	auto delayUnit = new QLabel(tr("ms"), this);
	m_delayConfirm = new QPushButton(tr("确认设置"), this);

	auto delayLayout = new QHBoxLayout;
	delayLayout->addWidget(m_delay, 3);
	delayLayout->addWidget(delayUnit, 1);

	// 不设定this父对象, 才能在其他窗口中正常展示布局的控件
	auto baseLayout = new QGridLayout;
	baseLayout->addWidget(localIPLabel, 0, 1);
	baseLayout->addWidget(m_localIP, 0, 3);
	baseLayout->addWidget(m_ipConfirm, 0, 5);
	baseLayout->addWidget(localPortLabel, 1, 1);
	baseLayout->addWidget(m_localPort, 1, 3);
	baseLayout->addWidget(m_portConfirm, 1, 5);
	baseLayout->addWidget(submaskLabel, 2, 1);
	baseLayout->addWidget(m_submask, 2, 3);
	baseLayout->addWidget(m_maskConfirm, 2, 5);
	baseLayout->addWidget(gatewayLabel, 3, 1);
	baseLayout->addWidget(m_gateway, 3, 3);
	baseLayout->addWidget(m_gatewayConfirm, 3, 5);
	baseLayout->addWidget(remoteIPLabel, 4, 1);
	baseLayout->addWidget(m_remoteIP, 4, 3);
	baseLayout->addWidget(m_remoteIPConfirm, 4, 5);
	baseLayout->addWidget(remotePortLabel, 5, 1);
	baseLayout->addWidget(m_remotePort, 5, 3);
	baseLayout->addWidget(m_remotePortConfirm, 5, 5);
	baseLayout->addWidget(delayLabel, 6, 1);
	baseLayout->addLayout(delayLayout, 6, 3);
	baseLayout->addWidget(m_delayConfirm, 6, 5);
	// 设置控件之间的间隔
	baseLayout->setColumnStretch(0, 1);
	baseLayout->setColumnStretch(1, 2);
	baseLayout->setColumnStretch(2, 1);
	baseLayout->setColumnStretch(3, 3);
	baseLayout->setColumnStretch(4, 1);
	baseLayout->setColumnStretch(5, 2);
	setLayout(baseLayout);
}

void UnicastWidget::paintEvent(QPaintEvent *event)
{
	// 使主窗口中设置的qss生效
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

MulticastWidget::MulticastWidget(QWidget *parent /*= 0*/)
	: QWidget(parent)
{
	auto localIPLabel = new QLabel(tr("组播本机IP"), this);
	m_localIP = new QLineEdit(tr("192.168.59.180"), this);
	m_localIP->setAlignment(Qt::AlignCenter);
	m_localIP->setMaximumWidth(LeWidth);
	m_ipConfirm = new QPushButton(tr("确认设置"), this);

	auto localPortLabel = new QLabel(tr("组播本机端口"), this);
	m_localPort = new QLineEdit(tr("6666"), this);
	m_localPort->setAlignment(Qt::AlignCenter);
	m_localPort->setMaximumWidth(LeWidth);
	m_portConfirm = new QPushButton(tr("确认设置"), this);

	auto remoteIPLabel = new QLabel(tr("组播目标IP"), this);
	m_remoteIP = new QLineEdit(tr("192.168.59.117"), this);
	m_remoteIP->setAlignment(Qt::AlignCenter);
	m_remoteIP->setMaximumWidth(LeWidth);
	m_remoteIPConfirm = new QPushButton(tr("确认设置"), this);

	auto remotePortLabel = new QLabel(tr("组播目标端口"), this);
	m_remotePort = new QLineEdit(tr("8888"), this);
	m_remotePort->setAlignment(Qt::AlignCenter);
	m_remotePort->setMaximumWidth(LeWidth);
	m_remotePortConfirm = new QPushButton(tr("确认设置"), this);

	auto delayLabel = new QLabel(tr("网络延时"), this);
	m_delay = new QLineEdit(tr("33"), this);
	m_delay->setAlignment(Qt::AlignCenter);
	m_delay->setMaximumWidth(LeWidth * 3 / 4);
	auto delayUnit = new QLabel(tr("ms"), this);
	m_delayConfirm = new QPushButton(tr("确认设置"), this);

	auto delayLayout = new QHBoxLayout;
	delayLayout->addWidget(m_delay, 3);
	delayLayout->addWidget(delayUnit, 1);

	// 不设定this父对象, 才能在其他窗口中正常展示布局的控件
	auto baseLayout = new QGridLayout;
	baseLayout->addWidget(localIPLabel, 0, 1);
	baseLayout->addWidget(m_localIP, 0, 3);
	baseLayout->addWidget(m_ipConfirm, 0, 5);
	baseLayout->addWidget(localPortLabel, 1, 1);
	baseLayout->addWidget(m_localPort, 1, 3);
	baseLayout->addWidget(m_portConfirm, 1, 5);
	baseLayout->addWidget(remoteIPLabel, 2, 1);
	baseLayout->addWidget(m_remoteIP, 2, 3);
	baseLayout->addWidget(m_remoteIPConfirm, 2, 5);
	baseLayout->addWidget(remotePortLabel, 3, 1);
	baseLayout->addWidget(m_remotePort, 3, 3);
	baseLayout->addWidget(m_remotePortConfirm, 3, 5);
	baseLayout->addWidget(delayLabel, 4, 1);
	baseLayout->addLayout(delayLayout, 4, 3);
	baseLayout->addWidget(m_delayConfirm, 4, 5);
	// 设置控件之间的间隔
	baseLayout->setColumnStretch(0, 1);
	baseLayout->setColumnStretch(1, 2);
	baseLayout->setColumnStretch(2, 1);
	baseLayout->setColumnStretch(3, 3);
	baseLayout->setColumnStretch(4, 1);
	baseLayout->setColumnStretch(5, 2);
	setLayout(baseLayout);
}

void MulticastWidget::paintEvent(QPaintEvent *event)
{
	// 使主窗口中设置的qss生效
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

BroadcastWidget::BroadcastWidget(QWidget *parent /*= 0*/)
	: QWidget(parent)
{
	auto broadcastIPLabel = new QLabel(tr("广播IP"), this);
	m_broadcastIP = new QLineEdit(tr("192.168.59.180"), this);
	m_broadcastIP->setAlignment(Qt::AlignCenter);
	m_broadcastIP->setMaximumWidth(LeWidth);
	m_ipConfirm = new QPushButton(tr("确认设置"), this);

	auto broadcastPortLabel = new QLabel(tr("广播端口"), this);
	m_broadcastPort = new QLineEdit(tr("6666"), this);
	m_broadcastPort->setAlignment(Qt::AlignCenter);
	m_broadcastPort->setMaximumWidth(LeWidth);
	m_portConfirm = new QPushButton(tr("确认设置"), this);

	auto delayLabel = new QLabel(tr("网络延时"), this);
	m_delay = new QLineEdit(tr("33"), this);
	m_delay->setAlignment(Qt::AlignCenter);
	m_delay->setMaximumWidth(LeWidth * 3 / 4);
	auto delayUnit = new QLabel(tr("ms"), this);
	m_delayConfirm = new QPushButton(tr("确认设置"), this);

	auto delayLayout = new QHBoxLayout;
	delayLayout->addWidget(m_delay, 3);
	delayLayout->addWidget(delayUnit, 1);

	// 不设定this父对象, 才能在其他窗口中正常展示布局的控件
	auto baseLayout = new QGridLayout;
	baseLayout->addWidget(broadcastIPLabel, 0, 1);
	baseLayout->addWidget(m_broadcastIP, 0, 3);
	baseLayout->addWidget(m_ipConfirm, 0, 5);
	baseLayout->addWidget(broadcastPortLabel, 1, 1);
	baseLayout->addWidget(m_broadcastPort, 1, 3);
	baseLayout->addWidget(m_portConfirm, 1, 5);
	baseLayout->addWidget(delayLabel, 4, 1);
	baseLayout->addLayout(delayLayout, 4, 3);
	baseLayout->addWidget(m_delayConfirm, 4, 5);
	// 设置控件之间的间隔
	baseLayout->setColumnStretch(0, 1);
	baseLayout->setColumnStretch(1, 2);
	baseLayout->setColumnStretch(2, 1);
	baseLayout->setColumnStretch(3, 3);
	baseLayout->setColumnStretch(4, 1);
	baseLayout->setColumnStretch(5, 2);
	setLayout(baseLayout);
}

void BroadcastWidget::paintEvent(QPaintEvent *event)
{
	// 使主窗口中设置的qss生效
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

MonitorWidget::MonitorWidget(QWidget *parent /*= 0*/)
	: QWidget(parent)
{
	auto localIPLabel = new QLabel(tr("本机IP"), this);
	m_localIP = new QLineEdit(tr("192.168.59.180"), this);
	m_localIP->setAlignment(Qt::AlignCenter);
	m_localIP->setMaximumWidth(LeWidth);

	auto submaskLabel = new QLabel(tr("本机掩码"), this);
	m_submask = new QLineEdit(tr("255.255.255.0"), this);
	m_submask->setAlignment(Qt::AlignCenter);
	m_submask->setMaximumWidth(LeWidth);

	auto gatewayLabel = new QLabel(tr("本机网关"), this);
	m_gateway = new QLineEdit(tr("192.168.59.1"), this);
	m_gateway->setAlignment(Qt::AlignCenter);
	m_gateway->setMaximumWidth(LeWidth);

	auto recvPortLabel = new QLabel(tr("监控接收端口"), this);
	m_recvPort = new QLineEdit(tr("20212"), this);
	m_recvPort->setAlignment(Qt::AlignCenter);
	m_recvPort->setMaximumWidth(LeWidth);

	auto sendPortLabel = new QLabel(tr("监控发送端口"), this);
	m_sendPort = new QLineEdit(tr("10212"), this);
	m_sendPort->setAlignment(Qt::AlignCenter);
	m_sendPort->setMaximumWidth(LeWidth);
	m_sendPortConfirm = new QPushButton(tr("确认设置"), this);

	// 不设定this父对象, 才能在其他窗口中正常展示布局的控件
	auto baseLayout = new QGridLayout;
	baseLayout->addWidget(localIPLabel, 0, 1);
	baseLayout->addWidget(m_localIP, 0, 3);
	baseLayout->addWidget(submaskLabel, 1, 1);
	baseLayout->addWidget(m_submask, 1, 3);	
	baseLayout->addWidget(gatewayLabel, 2, 1);
	baseLayout->addWidget(m_gateway, 2, 3);	
	baseLayout->addWidget(recvPortLabel, 3, 1);
	baseLayout->addWidget(m_recvPort, 3, 3);
	baseLayout->addWidget(sendPortLabel, 4, 1);
	baseLayout->addWidget(m_sendPort, 4, 3);
	baseLayout->addWidget(m_sendPortConfirm, 3, 5, 2, 1);	
	// 设置控件之间的间隔
	baseLayout->setColumnStretch(0, 1);
	baseLayout->setColumnStretch(1, 2);
	baseLayout->setColumnStretch(2, 1);
	baseLayout->setColumnStretch(3, 3);
	baseLayout->setColumnStretch(4, 1);
	baseLayout->setColumnStretch(5, 2);
	setLayout(baseLayout);
}

void MonitorWidget::paintEvent(QPaintEvent *event)
{
	// 使主窗口中设置的qss生效
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

NetSettingsTab::NetSettingsTab(QWidget *parent)
	: QWidget(parent)
{
	int w = width();
	int h = height();

	m_netBox = createNetExclusiveGroup();
	m_commBox = createCommExclusiveGroup();
		
	m_unicastWgt = new UnicastWidget;
	m_multicastWgt = new MulticastWidget;
	m_broadcastWgt = new BroadcastWidget;
	m_monitorWgt = new MonitorWidget;
	m_settingsLayout = new QStackedLayout;
	m_settingsLayout->addWidget(m_unicastWgt);
	m_settingsLayout->addWidget(m_multicastWgt);
	m_settingsLayout->addWidget(m_broadcastWgt);
	m_settingsLayout->addWidget(m_monitorWgt);
	m_settingsLayout->setCurrentIndex(0);

	auto baseLayout = new QGridLayout;
	baseLayout->addWidget(m_netBox, 0, 0, 1, 1);
	baseLayout->addWidget(m_commBox, 0, 1, 1, 1);
	baseLayout->addLayout(m_settingsLayout, 1, 0, 1, 2);
	setLayout(baseLayout);
	
	setStyleSheet(QSS_NetSettings);

	connectSlots();
}

//void NetSettingsTab::resizeEvent(QResizeEvent *event)
//{
//	QSize s = event->size();
//	int w = s.width();
//	int h = s.height();
//	if (0 == w && 0 == h) {
//		QWidget::resizeEvent(event);
//		return;
//	}
//
//	m_netBox->setGeometry(0, 0, w / 2, 80);
//	m_actionBox->setGeometry(w / 2, 0, w / 2, 80);
//	m_unicastWgt->setGeometry(0, 80, w, h - 80);
//	m_unicastWgt->show();
//}

QGroupBox * NetSettingsTab::createNetExclusiveGroup()
{
	m_firstNet = new QRadioButton(tr("网口1"), this);
	m_secondNet = new QRadioButton(tr("网口2"), this);
	m_netGroup = new QButtonGroup;
	m_netGroup->addButton(m_firstNet, 0);
	m_netGroup->addButton(m_secondNet, 1);	
	m_firstNet->setChecked(true);
	m_curNetNum = QString::number(1);

	QGroupBox *netBox = new QGroupBox(this);
	auto netLayout = new QHBoxLayout;	
	netLayout->addStretch(1);  // 使得button居中
	netLayout->addWidget(m_firstNet, 2);
	netLayout->addWidget(m_secondNet, 2);	
	netBox->setLayout(netLayout);

	return netBox;
}

QGroupBox * NetSettingsTab::createCommExclusiveGroup()
{
	m_unicast = new QRadioButton(tr("单播"), this);
	m_multicast = new QRadioButton(tr("组播"), this);
	m_broadcast = new QRadioButton(tr("广播"), this);
	m_monitor = new QRadioButton(tr("监控"), this);
	m_commGroup = new QButtonGroup;
	m_commGroup->addButton(m_unicast, 0);
	m_commGroup->addButton(m_multicast, 1);
	m_commGroup->addButton(m_broadcast, 2);
	m_commGroup->addButton(m_monitor, 3);	
	m_unicast->setChecked(true);	

	QGroupBox *commBox = new QGroupBox(this);
	auto commLayout = new QGridLayout;
	commLayout->addWidget(m_unicast, 0, 1);
	commLayout->addWidget(m_multicast, 0, 2);
	commLayout->addWidget(m_broadcast, 1, 1);
	commLayout->addWidget(m_monitor, 1, 2);	
	// 使得button居中
	commLayout->setColumnStretch(0, 1);
	commLayout->setColumnStretch(1, 2);
	commLayout->setColumnStretch(2, 2);
	commBox->setLayout(commLayout);

	return commBox;
}

void NetSettingsTab::connectSlots()
{
	connect(m_commGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotOnCommButtonClicked(int)));
	connect(m_netGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotOnNetButtonClicked(int)));
}

void NetSettingsTab::slotOnNetButtonClicked(int id)
{
	m_curNetNum = QString::number(id + 1);
}

void NetSettingsTab::slotOnCommButtonClicked(int id)
{
	m_settingsLayout->setCurrentIndex(id);
}

/****************************************************************************************
状态参数
*****************************************************************************************/
StateParamsTab::StateParamsTab(QWidget *parent)
	: QWidget(parent)
{
	m_deviceTab = new DeviceOverviewTab;
	m_bdsTab = new SatTypeStateTab;
	m_gpsTab = new SatTypeStateTab;
	m_gloTab = new SatTypeStateTab;
	m_dcbTab = new DCBStateTab;
	m_acbTab = new ACBStateTab;
	m_delayTab = new DelayCompensationTab;
	m_pulseTab = new PulseSettingsTab;
	m_tzTab = new TimezoneTab;
	m_logTab = new LogInfoTab;
	m_alarmTab = new LogInfoTab;
	m_buzzTab = new ScreenSettingTab;
	m_restoreTab = new RestoreTab;
	m_factoryTab = new FactorySettingTab;
	
	m_centralWidget = new QTabWidget;
	m_centralWidget->addTab(m_deviceTab, tr("设备总览"));
	m_centralWidget->addTab(m_bdsTab, tr("北斗状态"));
	m_centralWidget->addTab(m_gpsTab, tr("GPS状态"));
	m_centralWidget->addTab(m_gloTab, tr("GLONASS状态"));
	m_centralWidget->addTab(m_dcbTab, tr("直流状态"));
	m_centralWidget->addTab(m_acbTab, tr("交流状态"));
	m_centralWidget->addTab(m_delayTab, tr("延时补偿"));
	m_centralWidget->addTab(m_pulseTab, tr("脉冲配置"));
	m_centralWidget->addTab(m_tzTab, tr("时区设置"));
	m_centralWidget->addTab(m_logTab, tr("日志信息"));
	m_centralWidget->addTab(m_alarmTab, tr("告警信息"));
	m_centralWidget->addTab(m_buzzTab, tr("屏幕设置"));
	m_centralWidget->addTab(m_factoryTab, tr("出厂设置"));
	m_centralWidget->addTab(m_restoreTab, tr("还原设置"));
	
	m_centralWidget->setCurrentIndex(13); 

	auto baseLayout = new QVBoxLayout(this);
	baseLayout->addWidget(m_centralWidget);
	baseLayout->setMargin(0);	

	setStyleSheet(QSS_StateParams.arg(valueLabelQss).arg(valueTableViewQss).arg(valueLightgrayQss));
	initTest();
}

void StateParamsTab::initTest()
{
	st_Gnsssta gs;
	strcpy_s(gs.satelliteType, 4, "bds");
	m_bdsTab->setGnssstaInfo(gs);

	strcpy_s(gs.satelliteType, 4, "gps");
	m_gpsTab->setGnssstaInfo(gs);

	strcpy_s(gs.satelliteType, 4, "glo");
	m_gloTab->setGnssstaInfo(gs);

	st_Gnssgsv gg;
	strcpy_s(gg.satelliteType, 4, "bds");
	m_bdsTab->setGnssgsvInfo(gg);

	strcpy_s(gg.satelliteType, 4, "gps");
	gg.frameTotal = 1;	
	gg.count = 4;
	gg.satelliteNo[0] = 1;
	gg.satelliteNo[1] = 3;
	gg.satelliteNo[2] = 6;
	gg.satelliteNo[3] = 7;
	gg.elevation[0] = 57;
	gg.elevation[1] = 32;
	gg.elevation[2] = 12;
	gg.elevation[3] = 45;
	gg.azimuth[0] = 34;
	gg.azimuth[1] = 125;
	gg.azimuth[2] = 225;
	gg.azimuth[3] = 111;
	gg.snr[0] = 52;
	gg.snr[1] = 41;
	gg.snr[2] = 35;
	gg.snr[3] = 52;
	m_gpsTab->setGnssgsvInfo(gg);

	strcpy_s(gg.satelliteType, 4, "glo");
	m_gloTab->setGnssgsvInfo(gg);
}

