#include "stdafx.h"
#include "centralwidget.h"
#include "topwidget.h"
#include "leftwidget.h"
#include "tabwidget.h"
#include "stateparamswidget.h"
#include "timepositiondatabase.h"

CentralWidget::CentralWidget(QWidget *parent)
	: QWidget(parent)
{
	//setFixedSize(720, 440);
	//setFixedSize(600, 400);
	m_top = new TopWidget(this);
	
	m_left = new LeftWidget(this);

	m_timeSrc = new TimeSrcTab;
	m_comSettings = new ComSettingsTab;
	m_netSettings = new NetSettingsTab;
	m_stateMain = new StateMainTab;
	m_dev = new DeviceOverviewTab;
	m_bds = new SatTypeStateTab;
	m_gps = new SatTypeStateTab;
	m_glo = new SatTypeStateTab;
	m_acb = new ACBStateTab;
	m_dcb = new DCBStateTab;
	m_delay = new DelayCompensationTab;
	m_pulse = new PulseSettingsTab;
	m_timezone = new TimezoneTab;
	m_log = new LogInfoTab;
	m_alarm = new LogInfoTab;
	m_buzz = new ScreenSettingTab;
	m_restore = new RestoreTab;
	m_factory = new FactorySettingTab;
	m_pwd = new PasswordChangeTab;
	m_ver = new VersionInfoTab;

	m_center = new QFrame(this);
	m_centerLayout = new QStackedLayout;
	m_centerLayout->addWidget(m_timeSrc);
	m_centerLayout->addWidget(m_comSettings);
	m_centerLayout->addWidget(m_netSettings);
	m_centerLayout->addWidget(m_stateMain);
	m_centerLayout->addWidget(m_dev);
	m_centerLayout->addWidget(m_bds);
	m_centerLayout->addWidget(m_gps);
	m_centerLayout->addWidget(m_glo);
	m_centerLayout->addWidget(m_acb);
	m_centerLayout->addWidget(m_dcb);
	m_centerLayout->addWidget(new QWidget); // 占位 位置配置
	m_centerLayout->addWidget(new QWidget); // 占位 运动配置
	m_centerLayout->addWidget(new QWidget); // 占位 最小仰角
	m_centerLayout->addWidget(new QWidget); // 占位 最小信噪比
	m_centerLayout->addWidget(m_delay);
	m_centerLayout->addWidget(m_pulse);
	m_centerLayout->addWidget(m_timezone);
	m_centerLayout->addWidget(m_log);
	m_centerLayout->addWidget(m_alarm);
	m_centerLayout->addWidget(m_buzz);
	m_centerLayout->addWidget(m_factory);
	m_centerLayout->addWidget(m_pwd);
	m_centerLayout->addWidget(m_restore);
	m_centerLayout->addWidget(m_ver);
	m_centerLayout->setCurrentWidget(m_timeSrc);
	m_center->setLayout(m_centerLayout);

	initTest();   // 仅做测试
	connectSlots();

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);

	setStyleSheet(QSS_StateParams.arg(valueLabelQss).arg(valueTableViewQss).arg(valueLightgrayQss));
}

CentralWidget::~CentralWidget()
{
}

void CentralWidget::changeStartup()
{
	dynamic_cast<BaseWidget *>(m_centerLayout->currentWidget())->changeStartup();
}

void CentralWidget::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	setChildrenGeometry(w, h);
}

void CentralWidget::setChildrenGeometry(int w, int h)
{
	if (0 == w && 0 == h) return;

	int cLeft = LblWidth * 1.5;
	int cTop = LblHeight * 2;
	m_top->setGeometry(cLeft, 0, w - cLeft, cTop);
	m_left->setGeometry(0, 0, cLeft, h);
	m_center->setGeometry(QRect(cLeft, cTop, w - cLeft, h - cTop));
}

void CentralWidget::initTest()
{
	st_Gnsssta gs;
	strcpy_s(gs.satelliteType, 4, "bds");
	m_bds->setGnssstaInfo(gs);

	strcpy_s(gs.satelliteType, 4, "gps");
	m_gps->setGnssstaInfo(gs);

	strcpy_s(gs.satelliteType, 4, "glo");
	m_glo->setGnssstaInfo(gs);

	st_Gnssgsv gg;
	strcpy_s(gg.satelliteType, 4, "bds");
	m_bds->setGnssgsvInfo(gg);

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
	m_gps->setGnssgsvInfo(gg);

	strcpy_s(gg.satelliteType, 4, "glo");
	m_glo->setGnssgsvInfo(gg);
}

void CentralWidget::connectSlots()
{	
	connect(m_top, SIGNAL(homeBtn()), this, SIGNAL(home()));
	connect(m_top, SIGNAL(backBtn()), this, SIGNAL(back()));
	connect(m_top, SIGNAL(backBtn()), this, SLOT(slotOnBackBtnClicked()));
	connect(m_left, SIGNAL(leftButtonClicked(int)), this, SLOT(slotOnLeftBtnClicked(int)));
	connect(m_stateMain, SIGNAL(stateParamsButtonClicked(int)), this, SLOT(slotOnStateParamsBtnClicked(int)));

	connect(m_comSettings, SIGNAL(changeParams()), this, SIGNAL(changeParams()));
	connect(m_netSettings, SIGNAL(changeParams()), this, SIGNAL(changeParams()));

	connect(m_restore, SIGNAL(changeParams()), this, SIGNAL(changeParams()));
}

void CentralWidget::queryBoardInfo()
{
	TimePositionDatabase timePositionDB;
	unsigned char chCmd = COMMAND_IS_AT;

	QString data("ppswidth");
	timePositionDB.selectFromMasterBoard(chCmd, data);

	data = "ppsdelay";
	timePositionDB.selectFromMasterBoard(chCmd, data);
}

void CentralWidget::slotOnLeftBtnClicked(int id)
{
	m_centerLayout->setCurrentIndex(id); return;

	switch (id) {
	case 0:
		m_centerLayout->setCurrentWidget(m_timeSrc);
		break;
	case 1:
		m_centerLayout->setCurrentWidget(m_comSettings);
		break;
	case 2:
		m_centerLayout->setCurrentWidget(m_netSettings);
		break;
	case 3:
		m_centerLayout->setCurrentWidget(m_stateMain);
		break;
	default:
		break;
	}
}

void CentralWidget::slotOnStateParamsBtnClicked(int id)
{
	if (11 == id) {
		queryBoardInfo();
	}
	
	m_centerLayout->setCurrentIndex(id + 4);
}

void CentralWidget::slotOnBackBtnClicked()
{
	if (m_centerLayout->currentIndex() < 4)
		emit home();
	else
		m_centerLayout->setCurrentIndex(3);
}
