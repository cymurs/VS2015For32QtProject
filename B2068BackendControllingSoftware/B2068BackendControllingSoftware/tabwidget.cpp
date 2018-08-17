#include "stdafx.h"
#include "tabwidget.h"

/****************************************************************************************
子控件位置属性
*****************************************************************************************/
const int LeftMargin = 20;			// 左边距
const int TopMargin = 10;		// 顶边距
const int BottonMargin = 20;	// 底边距
const int LblWidth = 80;			//	标签宽度
const int LblHeight = 20;			// 标签高度

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
	m_tabWidget->setCurrentIndex(1);
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

	int frameStyle = QFrame::NoFrame;
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
	//m_bdsLock->setFrameStyle(frameStyle);
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

	m_inputSetting = new QLineEdit(tr("20180817113510"), this);
	

	m_confirm = new QPushButton(tr("确认\n设置"), this);

	QLabel *priorityLabel = new QLabel(tr("优先级"), this);
	priorityLabel->setObjectName(boldLabelQss);

	QLabel *inputLabel = new QLabel(tr("输入时间设置"), this);
	inputLabel->setObjectName(boldLabelQss);

	QButtonGroup *operatingGroup = new QButtonGroup(this);
	operatingGroup->addButton(m_manual);
	operatingGroup->addButton(m_auto);
	m_manual->setChecked(true);

	QButtonGroup *refSrcGroup = new QButtonGroup(this);
	refSrcGroup->addButton(m_bds);
	refSrcGroup->addButton(m_gps);
	refSrcGroup->addButton(m_glo);
	refSrcGroup->addButton(m_dcb);
	refSrcGroup->addButton(m_acb);
	refSrcGroup->addButton(m_input);
	m_bds->setChecked(true);

	auto topLayout = new QHBoxLayout;
	topLayout->addStretch(1);
	topLayout->addWidget(m_manual, 1);
	topLayout->addStretch(1);
	topLayout->addWidget(m_auto, 1);
	topLayout->addStretch(2);

	auto mainLayout = new QGridLayout;
	mainLayout->addWidget(priorityLabel, 0, 4);
	mainLayout->addWidget(m_bdsLock, 1, 0);
	mainLayout->addWidget(m_bds, 1, 1);
	mainLayout->addWidget(m_bdsDateTime, 1, 2);
	mainLayout->addWidget(m_bdsAvlSatellites, 1, 3);
	mainLayout->addWidget(m_bdsPriority, 1, 4);
	mainLayout->addWidget(m_gpsLock, 2, 0);
	mainLayout->addWidget(m_gps, 2, 1);
	mainLayout->addWidget(m_gpsDateTime, 2, 2);
	mainLayout->addWidget(m_gpsAvlSatellites, 2, 3);
	mainLayout->addWidget(m_gpsPriority, 2, 4);
	mainLayout->addWidget(m_gloLock, 3, 0);
	mainLayout->addWidget(m_glo, 3, 1);
	mainLayout->addWidget(m_gloDateTime, 3, 2);
	mainLayout->addWidget(m_gloAvlSatellites, 3, 3);
	mainLayout->addWidget(m_gloPriority, 3, 4);
	mainLayout->addWidget(m_dcbLock, 4, 0);
	mainLayout->addWidget(m_dcb, 4, 1);
	mainLayout->addWidget(m_dcbDateTime, 4, 2);
	mainLayout->addWidget(m_dcbPriority, 4, 4);
	mainLayout->addWidget(m_acbLock, 5, 0);
	mainLayout->addWidget(m_acb, 5, 1);
	mainLayout->addWidget(m_acbDateTime, 5, 2);
	mainLayout->addWidget(m_acbPriority, 5, 4);
	mainLayout->addWidget(m_input, 6, 1);
	mainLayout->addWidget(m_inputDateTime, 6, 2);
	mainLayout->addWidget(m_confirm, 6, 4, 2, 1);
	mainLayout->addWidget(inputLabel, 7, 1);
	mainLayout->addWidget(m_inputSetting, 7, 2);
	

	auto baseLayout = new QVBoxLayout;
	baseLayout->addLayout(topLayout);
	baseLayout->addLayout(mainLayout);
	setLayout(baseLayout);

	setStyleSheet(QSS_YaHeiTimeSrcRadio.arg(largeRadioQss).arg(smallRadioQss)
		+ QSS_TimeSrcLabel.arg(normalLabelQss).arg(boldLabelQss));
}

/****************************************************************************************
网口设置
*****************************************************************************************/
NetSettingsTab::NetSettingsTab(QWidget *parent)
	: QWidget(parent)
{}

/****************************************************************************************
状态参数
*****************************************************************************************/
StateParamsTab::StateParamsTab(QWidget *parent)
	: QWidget(parent)
{}