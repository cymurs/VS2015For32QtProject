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

	m_referenceSrc = new QLabel(tr("GLONASS"), this);
	m_referenceSrc->setObjectName(smallLabelQss);
	m_referenceSrc->setFrameStyle(frameStyle);
	m_referenceSrc->setScaledContents(true);
	m_referenceSrc->setGeometry(w - LeftMargin * 10.5, TopMargin, LblWidth, LblHeight);

	m_availableSatallites = new QLabel(tr("12"), this);
	m_availableSatallites->setObjectName(smallLabelQss);
	m_availableSatallites->setFrameStyle(frameStyle);
	m_availableSatallites->setScaledContents(true);
	m_availableSatallites->setGeometry(w - LeftMargin * 6, TopMargin, LblWidth, LblHeight);

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
	
	connectSlots();
	m_refSrcTimer.start(1000);
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
	m_referenceSrc->setGeometry(w - LeftMargin * 10.5, TopMargin, LblWidth, LblHeight);
	m_availableSatallites->setGeometry(w - LeftMargin * 6, TopMargin, LblWidth, LblHeight);
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
	m_refSrcTimer.setParent(this);
	connect(&m_refSrcTimer, SIGNAL(timeout()), this, SLOT(slotOnRefSrcTimeOut()));
}

void MainTab::slotOnRefSrcTimeOut()
{
	QString curTime = QTime::currentTime().toString("hh:mm:ss");
	m_refSrcTime->setText(curTime);
}

/****************************************************************************************
时间源
*****************************************************************************************/
TimeSrcTab::TimeSrcTab(QWidget *parent)
	: QWidget(parent)
{}

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