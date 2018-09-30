#include "stdafx.h"
#include "tabwidget.h"
#include "stateparamswidget.h"
#include "transportthread.h"
#include "timepositiondatabase.h"

/****************************************************************************************/
//const int LoginInterval = 5 * 60 * 1000;
const char *ConfigFile = "config.ini";
st_UserPwd userPwd;

/****************************************************************************************
需继承的父类
*****************************************************************************************/
BaseWidget::BaseWidget(QWidget *parent /* = 0 */)
	: QWidget(parent)
{}

void BaseWidget::changeStartup()
{}

/****************************************************************************************
主窗口
*****************************************************************************************/
TabWidget::TabWidget(QWidget *parent)
	: QDialog(parent)
{
	//ui.setupUi(this);
	resize(600, 400);
	setMinimumSize(600, 400);
	setFixedSize(600, 400);

	m_signIn = new SignInWidget;

	m_stateParams = new StateParamsTab;
	m_tabWidget = new QTabWidget;
	m_tabWidget->addTab(new MainTab(), tr("主界面"));
	m_tabWidget->addTab(new TimeSrcTab(), tr("时间源"));
	m_tabWidget->addTab(new ComSettingsTab(), tr("串口设置"));
	m_tabWidget->addTab(new NetSettingsTab(), tr("网络设置"));
	m_tabWidget->addTab(m_stateParams, tr("状态参数"));	

	//auto baseLayout = new QVBoxLayout;
	m_centralLayout = new QStackedLayout;
	m_centralLayout->addWidget(m_signIn);
	m_centralLayout->addWidget(m_tabWidget);
	m_centralLayout->setMargin(0);
	m_centralLayout->setSpacing(0);
	m_centralLayout->setCurrentIndex(0);
	setLayout(m_centralLayout);
	
	if (windowState() | Qt::WindowMaximized)
		setWindowState(windowState() & ~Qt::WindowMaximized);

	setStyleSheet(QSS_TabWidget);
	setWindowTitle(tr("B2068控制软件"));
	setWindowIcon(QIcon(":/BackendControlling/images/90.ico"));
	
	m_firstTime = true;
	connectSlots();	
	loadConfig();
}

TabWidget::~TabWidget()
{
}

void TabWidget::closeEvent(QCloseEvent *event)
{
	storeConfig();
	
}

void TabWidget::connectSlots()
{
	connect(m_signIn, SIGNAL(login(bool)), this, SLOT(slotOnSignIn(bool)));
	connect(m_stateParams, SIGNAL(relogin()), this, SLOT(slotOnLoginTimeout()));
}

void TabWidget::loadConfig()
{
	if (!QFile::exists(ConfigFile)) {
		userPwd.user2pwd["USER1"] = "888888";
		userPwd.user2pwd["USER2"] = "888888";
		userPwd.user2pwd["USER3"] = "888888";
		return;
	}

	QSettings file(ConfigFile, QSettings::IniFormat);
	userPwd.user2pwd["USER1"] = file.value("/USER/USER1").toString();
	userPwd.user2pwd["USER2"] = file.value("/USER/USER2").toString();
	userPwd.user2pwd["USER3"] = file.value("/USER/USER3").toString();
}

void TabWidget::storeConfig()
{
	QSettings file(ConfigFile, QSettings::IniFormat);
	QMap<QString, QString> &m = userPwd.user2pwd;
	for (auto user = m.begin(); user != m.end(); ++user) {
		file.setValue(tr("/USER/%1").arg(user.key()), user.value());
	}
}

void TabWidget::slotOnSignIn(bool signin)
{
	if (signin) {		
		m_centralLayout->setCurrentIndex(1);
		if (m_firstTime) {
			m_tabWidget->setCurrentIndex(0);
			m_firstTime = false;
		}
		QTimer::singleShot(LoginInterval, this, &TabWidget::slotOnLoginTimeout);
	}
	else {
		exit(0);
	}
}

void TabWidget::slotOnLoginTimeout()
{
	m_centralLayout->setCurrentIndex(0);
}

/****************************************************************************************
登录界面
*****************************************************************************************/
SignInWidget::SignInWidget(QWidget *parent /* = 0 */)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	QStringList users = { "user1", "user2", "user3" };
	QRegExp rx("^\\d{6}$");
	QValidator *validator = new QRegExpValidator(rx, this);
	
	m_userLabel = new QLabel(tr("用  户: "), this);
	m_userLabel->setFrameStyle(QFrame::NoFrame);
	m_user = new QComboBox(this);
	m_user->addItems(users);
	m_pwdLabel = new QLabel(tr("密  码: "), this); 
	m_pwd = new QLineEdit(this);	
	m_pwd->setValidator(validator);
	m_pwd->setAlignment(Qt::AlignCenter);
	m_pwd->setEchoMode(QLineEdit::Password); // 结合QSS实现任意掩码 星号: 42  实心圆圈: 9679 实心中小方形: 9726
	m_login = new QPushButton(tr("登  录"), this);
	m_quit = new QPushButton(tr("退  出"), this);
	//m_pwdArray[0] = 888888;
	//m_pwdArray[1] = 888888;
	//m_pwdArray[2] = 888888;		

	// 下拉列表框中字体居中 
	//static_cast<QStandardItemModel *>(m_user->view()->model())->item(0)->setTextAlignment(Qt::AlignCenter);

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);

	setTextAlignCenter(m_user);
	QString objName("signin");
	setObjectName(objName);
	setStyleSheet(QSS_SignInWidget.arg(objName));
	connectSlots();

	setTabOrder(m_user, m_pwd);
	setTabOrder(m_pwd, m_login);
}

void SignInWidget::resizeEvent(QResizeEvent *event)
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

void SignInWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void SignInWidget::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		if (dynamic_cast<QWidget *>(m_pwd) == QApplication::focusWidget() &&
			m_pwd->text().length() < 6) {
			QMessageBox msgBox(QMessageBox::Critical, tr("提示"), tr("密码必须为6位数字!"), QMessageBox::NoButton);
			msgBox.addButton(tr("确认"), QMessageBox::AcceptRole);
			msgBox.setStyleSheet(QSS_MsgBox + QSS_PushButton);
			msgBox.exec();			
		}
	}	
}

void SignInWidget::setTextAlignCenter(QComboBox *comboBox)
{
	int items = comboBox->count();
	if (0 == items) return;

	for (int i = 0; i < items; ++i) {
		static_cast<QStandardItemModel *>(comboBox->model())->item(i)->setTextAlignment(Qt::AlignCenter);
	}
}

void SignInWidget::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h) return;

	int left = w / 4;
	int top = (h - m_lblHeight * 6) / 2;
	m_userLabel->setGeometry(left, top, m_lblWidth, m_lblHeight);
	m_user->setGeometry(w / 2, top, m_lblWidth, m_lblHeight);
	m_pwdLabel->setGeometry(left, top + m_lblHeight * 2, m_lblWidth, m_lblHeight);
	m_pwd->setGeometry(w / 2, top + m_lblHeight * 2, m_lblWidth, m_lblHeight);
	m_login->setGeometry(left, top + m_lblHeight * 5, m_lblWidth, m_lblHeight);
	m_quit->setGeometry(w / 2, top + m_lblHeight * 5, m_lblWidth, m_lblHeight);
}

void SignInWidget::connectSlots()
{
	connect(m_user, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), 
		[=](int) {	m_pwd->setFocus();	}
	);
	connect(m_pwd, &QLineEdit::returnPressed, [=]() {
		m_login->click();
	});
	connect(m_login, SIGNAL(clicked(bool)), this, SLOT(slotOnLoginClicked(bool)));
	connect(m_quit, SIGNAL(clicked(bool)), this, SLOT(slotOnQuitClicked()));
}

void SignInWidget::slotOnLoginClicked(bool checked)
{
	QString user = m_user->currentText();
	QString pwd = m_pwd->text();

	if (0 != pwd.compare(userPwd.user2pwd[user])) {
		QMessageBox msgBox(QMessageBox::Critical, tr("提示"), tr("密码不正确!"), QMessageBox::NoButton);
		msgBox.addButton(tr("确认"), QMessageBox::AcceptRole);
		msgBox.setStyleSheet(QSS_MsgBox + QSS_PushButton);
		msgBox.exec();

		m_pwd->setFocus(Qt::MouseFocusReason);
		return;
	}
	userPwd.curUser = user;
	m_pwd->clear();

	emit login(true);
}

void SignInWidget::slotOnQuitClicked()
{
	emit login(false);
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

	m_signNormal = QPixmap(":/BackendControlling/images/antenna3.png", "PNG");
	m_signAbnormal = QPixmap(":/BackendControlling/images/antenna0.png", "PNG");
	m_lockClose = QPixmap(":/BackendControlling/images/lock_white_close.png", "PNG");
	m_lockOpen = QPixmap(":/BackendControlling/images/lock_white_open.png", "PNG");

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
	//m_signalState->setPixmap(QPixmap(":/BackendControlling/images/antenna3.png", "PNG"));
	m_signalState->setPixmap(m_signNormal);
	m_signalState->setFrameStyle(frameStyle);
	m_signalState->setScaledContents(true);
	m_signalState->setGeometry(w - LeftMargin * 4, TopMargin, LblHeight, LblHeight);

	m_lockedState = new QLabel(this);	
	//m_lockedState->setPixmap(QPixmap(":/BackendControlling/images/lock_white_close.png", "PNG"));
	m_lockedState->setPixmap(m_lockClose);
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
	m_position->setGeometry(LeftMargin, h - BottomMargin * 1.5, w - LeftMargin, LblHeight);
	
	QString objName("maintab");
	setObjectName(objName);	
	setStyleSheet(QSS_MainTabBackground.arg(objName) 
		+ QSS_TimesMainTabLabel.arg(smallLabelQss).arg(mediumLabelQss).arg(largeLabelQss));
	
	// 动画1
	//m_animation = new QPropertyAnimation(this, "geometry");
	//m_animation->setDuration(600);
	//m_animation->setStartValue(geometry());
	//m_animation->setEndValue(QRect(-width(), 0, width(), height()));
	//m_animation->setEasingCurve(QEasingCurve::InOutSine);  // 有效

	// 动画2
	m_animation = new QPropertyAnimation(this, "pos");
	m_animation->setDuration(600);
	m_animation->setStartValue(QPoint(rect().topLeft()) );
	m_animation->setEndValue(QPoint(-width(), 0));
	m_animation->setEasingCurve(QEasingCurve::InOutBack);

	// 测试	
	// 设置精度，最精确的一个
	//QString curDate = QDate::currentDate().toString("yyyy 年 MM 月 dd 日");
	//m_refSrcDate->setText(curDate);
	m_refSrcTimer.setTimerType(Qt::PreciseTimer);  
	connectSlots();
	//m_refSrcTimer.start(100);

	m_iRefSrc = NOTPOS;
}

MainTab::~MainTab()
{
	if (m_animation != Q_NULLPTR) {
		delete m_animation;
		m_animation = Q_NULLPTR;
	}		
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
	m_position->setGeometry(LeftMargin, h - BottomMargin * 1.5, w - LeftMargin, LblHeight);
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

void MainTab::mousePressEvent(QMouseEvent *event)
{
	//QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
	//animation->setDuration(600);
	//animation->setStartValue(geometry());
	//animation->setEndValue(QRect(width(), 0, width(), height()));
	//animation->setEasingCurve(QEasingCurve::Linear);
	//animation->start(QAbstractAnimation::DeleteWhenStopped);
	//connect(animation, SIGNAL(finished()), this, SIGNAL(fadeOut()));

	queryBoardInfo();
	m_animation->start();			
}

QString MainTab::LonLatConvert(float value)
{
	int t1 = value / 100.0;
	float t2 = (value - t1 * 100.0) / 60.0;
	float t = t1 + t2;
	int degree = t;
	int minute = (t - degree) * 60.0;
	int second = (t - degree - minute / 60.0) * 3600;
	return QString("%1°%2′%3″  ").arg(degree).arg(minute).arg(second);
}

void MainTab::connectSlots()
{
	TransportThread *pTransport = TransportThread::Get();
	connect(m_animation, SIGNAL(finished()), this, SIGNAL(fadeOut()));
	connect(pTransport, SIGNAL(statusSignal(st_Status)), this, SLOT(slotOnStatusReached(st_Status)));
	connect(pTransport, SIGNAL(gnsstimeSignal(st_Gnsstime)), this, SLOT(slotOnGnsstimeReached(st_Gnsstime)));
	connect(pTransport, SIGNAL(gnssstaSignal(st_Gnsssta)), this, SLOT(slotOnGnssstaReached(st_Gnsssta)));
	connect(pTransport, SIGNAL(b2068Signal(int)), this, SLOT(slotOnB2068Reached(int)));

	//connect(&m_refSrcTimer, &QTimer::timeout, this, &MainTab::slotOnRefSrcTimeOut);	
	//QMetaObject::Connection result = connect(&m_refSrcTimer, SIGNAL(timeout()), this, SLOT(slotOnRefSrcTimeOut()));
	//if (Q_NULLPTR == result) {
	//	QMessageBox::warning(this, "failed", "connect timer's timeout failed.");
	//}
}

void MainTab::queryBoardInfo()
{
	TimePositionDatabase timePositionDB;
	unsigned char chCmd = COMMAND_IS_AT;

	QString data("priority");
	timePositionDB.selectFromMasterBoard(chCmd, data);

	data = "timezone";
	timePositionDB.selectFromMasterBoard(chCmd, data);

	data = "IRIG_BACiF"; // 交流B码输入
	timePositionDB.selectFromMasterBoard(chCmd, data);

	data = "IRIG_BACoF"; // 交流B码输出
	timePositionDB.selectFromMasterBoard(chCmd, data);

	data = "IRIG_BACV"; // 交流B码输出幅值
	timePositionDB.selectFromMasterBoard(chCmd, data);

	data = "IRIG_BACM"; // 交流B码输出调制比
	timePositionDB.selectFromMasterBoard(chCmd, data);

	data = "IRIG_BDCiF"; // 直流B码输入
	timePositionDB.selectFromMasterBoard(chCmd, data);

	data = "IRIG_BDCoF"; // 直流B码输出
	timePositionDB.selectFromMasterBoard(chCmd, data);
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

void MainTab::slotOnStatusReached(const st_Status &status)
{
	m_iRefSrc = status.curRef;
	switch (m_iRefSrc) {
	case GPS:
		m_refSrc->setText("GPS");
		break;
	case GLONASS:
		m_refSrc->setText("GLONASS");
		break;
	case BDS:
		m_refSrc->setText(tr("北斗"));
		break;
	case BAC:
		m_refSrc->setText(tr("BAC码"));
		break;
	case BDC:
		m_refSrc->setText(tr("BDC码"));
		break;
	case SETTIME:
		m_refSrc->setText(tr("置时"));
		break;
	default:
		m_refSrc->setText("未定位");
		break;
	}
	if (m_iRefSrc > 0 && m_iRefSrc < 5) {
		if (0x02 == status.lockState) {
			m_lockedState->setPixmap(m_lockClose);	
		}
		else {
			m_lockedState->setPixmap(m_lockOpen);			
		}
	}
	else {
		m_lockedState->clear();
	}
}

void MainTab::slotOnGnsstimeReached(const st_Gnsstime &gnsstime)
{
	uint curTime(0);
	bool hasTimezone(false);
	if (m_iRefSrc == GPS
		|| m_iRefSrc == GLONASS
		|| m_iRefSrc == BDS) {		
		m_timeName->setText(tr("北京时间"));
		curTime = gnsstime.UTCTime.toUInt();	
		hasTimezone = true;
	} else if (m_iRefSrc == BDC) {
		m_timeName->setText(tr("直流B码时间"));
		curTime = gnsstime.dcbTime.toUInt();
		hasTimezone = false;
	} else if (m_iRefSrc == BAC) {
		m_timeName->setText(tr("交流B码时间"));
		curTime = gnsstime.acbTime.toUInt();
		hasTimezone = false;
	} else if (m_iRefSrc == SETTIME) {
		m_timeName->setText(tr("输入时间"));
		curTime = gnsstime.inputTime.toUInt();
		hasTimezone = false;
	} else {
		m_timeName->clear();
		m_refSrcTime->clear();
		m_refSrcDate->clear();
		return;
	}

	char szText[128] = { '\0' };
	structTime t = TimeStampToTime(curTime, hasTimezone);
	sprintf_s(szText, 128, "%02d:%02d:%02d", t.Hour, t.Minute, t.Second);
	m_refSrcTime->setText(szText);
	memset(szText, '\0', 128);
	sprintf_s(szText, 128, ("%04d 年 %02d 月 %02d 日"), t.Year, t.Month, t.Date);
	m_refSrcDate->setText(tr("%1").arg(szText));
}

void MainTab::slotOnGnssstaReached(const st_Gnsssta &gnsssta)
{
	if ( (m_iRefSrc == GPS && 0 == strcmp(gnsssta.satelliteType, "gps"))
		|| (m_iRefSrc == GLONASS && 0 == strcmp(gnsssta.satelliteType, "glo"))
		|| (m_iRefSrc == BDS && 0 == strcmp(gnsssta.satelliteType, "bds")) ) {
		m_iAvlSatellites = gnsssta.avlSatellites;
		m_avlSatellites->setText(QString::number(m_iAvlSatellites));
		if (gnsssta.visSatellites > 3 && 1 == gnsssta.snrFlag) {
			m_bSignNormal = true;
			m_signalState->setPixmap(m_signNormal);
		}
		else {
			m_bSignNormal = false;
			m_signalState->setPixmap(m_signAbnormal);
		}
		QString position;
		if (0 == gnsssta.longitude.compare("e")) {
			position = tr("东经 ");
		}
		else {
			position = tr("西经 ");
		}
		position.append(LonLatConvert(gnsssta.lngValue));
		if (0 == gnsssta.latitude.compare("s")) {
			position.append(tr("南纬 "));
		}
		else {
			position.append(tr("北纬"));
		}
		position.append(LonLatConvert(gnsssta.latValue));
		position.append(tr("高程 %1 米").arg(gnsssta.elevation));
		m_position->setText(position);
		return;
	}
	if (NOTPOS == m_iRefSrc
		|| BAC == m_iRefSrc
		|| BDC == m_iRefSrc
		|| SETTIME == m_iRefSrc) {
		m_avlSatellites->clear();
		m_signalState->clear();
		m_position->clear();	
	}

}

void MainTab::slotOnB2068Reached(int mark)
{
	if (2 == mark) {
		m_deviceModel->setText("B20682D");
	}
	else {
		m_deviceModel->setText("B2068-3");
	}
}

/****************************************************************************************
时间源
*****************************************************************************************/
TimeSrcTab::TimeSrcTab(QWidget *parent)
	: BaseWidget(parent)
{		
	const QSize LockSize(20, 20);
	const int RowSpan(1), MinRowHeight(40);	
	
	QStringList priority;
	priority << "1" << "2" << "3" << "4" << "5";
	
	QString largeRadioQss("largeradio");
	QString smallRadioQss("smallradio");
	QString normalLabelQss("normallabel");
	QString boldLabelQss("boldlabel");
	QString autoColorQss("autoField");

	m_lockOpen = QPixmap(":/BackendControlling/images/lock_black_open.png", "PNG");
	m_lockClose = QPixmap(":/BackendControlling/images/lock_black_close.png", "PNG");

	m_manual = new QRadioButton(tr("手动"), this);
	m_manual->setObjectName(largeRadioQss);

	m_auto = new QRadioButton(tr("自动"), this);
	m_auto->setObjectName(largeRadioQss);
	m_auto->setProperty(qPrintable(autoColorQss), true);

	m_bdsLock = new QLabel(this);		
	m_bdsLock->setScaledContents(true);
	m_bdsLock->setFixedSize(LockSize);
	m_bdsLock->setPixmap(m_lockClose);
		
	m_bds = new QRadioButton(tr("    北斗"), this);
	m_bds->setObjectName(smallRadioQss);

	//m_bdsDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_bdsDateTime = new QLabel(this);
	m_bdsDateTime->setObjectName(normalLabelQss);
	m_bdsDateTime->setScaledContents(true);

	//m_bdsAvlSatellites = new QLabel(tr("12 颗"), this);
	m_bdsAvlSatellites = new QLabel(this);
	m_bdsAvlSatellites->setAlignment(Qt::AlignCenter);
	m_bdsAvlSatellites->setObjectName(normalLabelQss);

	m_bdsPriority = new QComboBox(this);
	m_bdsPriority->addItems(priority);
	m_bdsPriority->setProperty(qPrintable(autoColorQss), true);

	m_gpsLock = new QLabel(this);
	m_gpsLock->setScaledContents(true);
	m_gpsLock->setFixedSize(LockSize);
	//m_gpsLock->setPixmap(QPixmap(":/BackendControlling/images/lock_black_open.png", "PNG"));

	m_gps = new QRadioButton(tr("    GPS"), this);
	m_gps->setObjectName(smallRadioQss);

	//m_gpsDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_gpsDateTime = new QLabel(this);
	m_gpsDateTime->setObjectName(normalLabelQss);
	m_gpsDateTime->setScaledContents(true);

	//m_gpsAvlSatellites = new QLabel(tr("10 颗"), this); 
	m_gpsAvlSatellites = new QLabel(this);
	m_gpsAvlSatellites->setAlignment(Qt::AlignCenter);
	m_gpsAvlSatellites->setObjectName(normalLabelQss);

	m_gpsPriority = new QComboBox(this);
	m_gpsPriority->addItems(priority);
	m_gpsPriority->setProperty(qPrintable(autoColorQss), true);

	m_gloLock = new QLabel(this);
	m_gloLock->setScaledContents(true);
	m_gloLock->setFixedSize(LockSize);

	m_glo = new QRadioButton(tr("    GLO"), this);
	m_glo->setObjectName(smallRadioQss);

	//m_gloDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_gloDateTime = new QLabel(this);
	m_gloDateTime->setObjectName(normalLabelQss);
	m_gloDateTime->setScaledContents(true);

	//m_gloAvlSatellites = new QLabel(tr("11 颗"), this); 
	m_gloAvlSatellites = new QLabel(this);
	m_gloAvlSatellites->setAlignment(Qt::AlignCenter);
	m_gloAvlSatellites->setObjectName(normalLabelQss);

	m_gloPriority = new QComboBox(this);
	m_gloPriority->addItems(priority);
	m_gloPriority->setProperty(qPrintable(autoColorQss), true);

	m_dcbLock = new QLabel(this);
	m_dcbLock->setScaledContents(true);
	m_dcbLock->setFixedSize(LockSize);

	m_dcb = new QRadioButton(tr("直流B码"), this);
	m_dcb->setObjectName(smallRadioQss);

	//m_dcbDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_dcbDateTime = new QLabel(this);
	m_dcbDateTime->setObjectName(normalLabelQss);
	m_dcbDateTime->setScaledContents(true);

	m_dcbPriority = new QComboBox(this);
	m_dcbPriority->addItems(priority);
	m_dcbPriority->setProperty(qPrintable(autoColorQss), true);

	m_acbLock = new QLabel(this);
	m_acbLock->setScaledContents(true);
	m_acbLock->setFixedSize(LockSize);

	m_acb = new QRadioButton(tr("交流B码"), this);
	m_acb->setObjectName(smallRadioQss);

	//m_acbDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_acbDateTime = new QLabel(this);
	m_acbDateTime->setObjectName(normalLabelQss);
	m_acbDateTime->setScaledContents(true);

	m_acbPriority = new QComboBox(this);
	m_acbPriority->addItems(priority);
	m_acbPriority->setProperty(qPrintable(autoColorQss), true);

	m_input = new QRadioButton(tr("输入时间"), this);
	m_input->setObjectName(smallRadioQss);

	//m_inputDateTime = new QLabel(tr("2018/08/17 11:35:10"), this);
	m_inputDateTime = new QLabel(this);
	m_inputDateTime->setObjectName(normalLabelQss);
	m_inputDateTime->setScaledContents(true);


	QLabel *inputLabel = new QLabel(tr("输入时间设置"), this);
	inputLabel->setObjectName(boldLabelQss);

	//m_inputSetting = new QLineEdit(tr("20180817113510"), this);
	m_inputSetting = new QLineEdit(this);
	m_inputSetting->setAlignment(Qt::AlignCenter);
	m_inputSetting->setPlaceholderText("YYYYMMDDhhmmss");
	QRegExp rx("2\\d{3}(0[1-9]|1[0-2])(0[1-9]|[12]\\d|3[01])([01]\\d|2[0-3])([0-5]\\d){2}");
	QValidator *validator = new QRegExpValidator(rx, this);
	m_inputSetting->setValidator(validator);	
	m_inputSetting->setMaximumWidth(LblWidth * 2);

	m_confirm = new QPushButton(tr("确认\n设置"), this);

	QLabel *priorityLabel = new QLabel(tr("优先级"), this);
	priorityLabel->setObjectName(boldLabelQss);
	priorityLabel->setProperty(qPrintable(autoColorQss), true);
	
	// 分组
	m_operatingGroup = new QButtonGroup(this);
	m_operatingGroup->addButton(m_manual, 0);
	m_operatingGroup->addButton(m_auto, 1);
	m_manual->setChecked(true);

	m_refSrcGroup = new QButtonGroup(this);
	m_refSrcGroup->addButton(m_gps, 1);
	m_refSrcGroup->addButton(m_glo, 2);
	m_refSrcGroup->addButton(m_bds, 3);
	m_refSrcGroup->addButton(m_acb, 4);
	m_refSrcGroup->addButton(m_dcb, 5);
	m_refSrcGroup->addButton(m_input, 8);
	m_bds->setChecked(true);
	m_curRefSrc = BDS;
	m_lockCurRefSrc = false;

	m_priorityGroup[0] = m_gpsPriority;
	m_priorityGroup[1] = m_gloPriority;
	m_priorityGroup[2] = m_bdsPriority;
	m_priorityGroup[3] = m_acbPriority;
	m_priorityGroup[4] = m_dcbPriority;	

	setTextAlignCenter(m_gpsPriority);
	setTextAlignCenter(m_gloPriority);
	setTextAlignCenter(m_bdsPriority);
	setTextAlignCenter(m_acbPriority);
	setTextAlignCenter(m_dcbPriority);
	
	auto topLayout = new QHBoxLayout;
	topLayout->addStretch(1);
	topLayout->addWidget(m_manual, 1);
	//topLayout->addStretch(1);
	topLayout->addWidget(m_auto, 1);
	topLayout->addStretch(1);

	auto mainLayout = new QGridLayout;
	mainLayout->addWidget(priorityLabel, 0, 5, RowSpan, 1);
	mainLayout->addWidget(m_bdsLock, 1, 1, RowSpan, 1);
	mainLayout->addWidget(m_bds, 1, 2, RowSpan, 1);
	mainLayout->addWidget(m_bdsDateTime, 1, 3, RowSpan, 1);
	mainLayout->addWidget(m_bdsAvlSatellites, 1, 4, RowSpan, 1);
	mainLayout->addWidget(m_bdsPriority, 1, 5, RowSpan, 1);
	mainLayout->addWidget(m_gpsLock, 2, 1, RowSpan, 1);
	mainLayout->addWidget(m_gps, 2, 2, RowSpan, 1);
	mainLayout->addWidget(m_gpsDateTime, 2, 3, RowSpan, 1);
	mainLayout->addWidget(m_gpsAvlSatellites, 2, 4, RowSpan, 1);
	mainLayout->addWidget(m_gpsPriority, 2, 5, RowSpan, 1);
	mainLayout->addWidget(m_gloLock, 3, 1, RowSpan, 1);
	mainLayout->addWidget(m_glo, 3, 2, RowSpan, 1);
	mainLayout->addWidget(m_gloDateTime, 3, 3, RowSpan, 1);
	mainLayout->addWidget(m_gloAvlSatellites, 3, 4, RowSpan, 1);
	mainLayout->addWidget(m_gloPriority, 3, 5, RowSpan, 1);
	mainLayout->addWidget(m_dcbLock, 4, 1, RowSpan, 1);
	mainLayout->addWidget(m_dcb, 4, 2, RowSpan, 1);
	mainLayout->addWidget(m_dcbDateTime, 4, 3, RowSpan, 1);
	mainLayout->addWidget(m_dcbPriority, 4, 5, RowSpan, 1);
	mainLayout->addWidget(m_acbLock, 5, 1, RowSpan, 1);
	mainLayout->addWidget(m_acb, 5, 2, RowSpan, 1);
	mainLayout->addWidget(m_acbDateTime, 5, 3, RowSpan, 1);
	mainLayout->addWidget(m_acbPriority, 5, 5, RowSpan, 1);
	mainLayout->addWidget(m_input, 6, 2, RowSpan, 1);
	mainLayout->addWidget(m_inputDateTime, 6, 3, RowSpan, 1);
	mainLayout->addWidget(m_confirm, 6, 5, RowSpan+1, 1);
	mainLayout->addWidget(inputLabel, 7, 2, RowSpan, 1);
	mainLayout->addWidget(m_inputSetting, 7, 3, RowSpan, 1);	
	mainLayout->setHorizontalSpacing(15);
	// 设置行距
	for (int i = 1; i < 8; ++i) {
		mainLayout->setRowMinimumHeight(i, MinRowHeight);
	}
	// 设置列距
	for (int i = 0; i < 7; ++i) {
		if (0 == i || i > 4 ) {
			mainLayout->setColumnStretch(i, 1);
			continue;
		}

		if (1 == i || 4 == i ) {
			mainLayout->setColumnStretch(i, 2);
			continue;
		}

		mainLayout->setColumnStretch(i, 3);
	}

	auto baseLayout = new QVBoxLayout;
	//baseLayout->addStretch(1);
	baseLayout->addLayout(topLayout, 1);
	baseLayout->addLayout(mainLayout, 8);
	//baseLayout->addStretch(1);
	//baseLayout->setMargin(15);	
	baseLayout->setContentsMargins(0, 20, 0, 10); // 替代旧版setMargin, 对应: 左 上 右 下
	setLayout(baseLayout);

	setStyleSheet(QSS_TimesTimeSrcRadio.arg(largeRadioQss).arg(smallRadioQss)
		+ QSS_TimeSrcOthers.arg(normalLabelQss).arg(boldLabelQss));

	connectSlots();
	switchAutoManual(false, true);
}

void TimeSrcTab::changeStartup()
{
	TimePositionDatabase db;
	unsigned char chCmd = COMMAND_IS_AT;
	
	QString inputSetting = m_inputSetting->text();
	if (0 != inputSetting.compare(m_strInputSetting)) {
		QString data = QString("intime,%1").arg(inputSetting);
		db.selectFromMasterBoard(chCmd, data);
	}

	int curRefSrc = m_refSrcGroup->checkedId();
	if (0 == m_operatingGroup->checkedId()) {		
		if ((m_priority.curRefSrc - '0') != curRefSrc) {
			QString data = QString("priority,b,%1,%2,%3,%4,%5,%6")
				.arg(m_priority.priorFirst).arg(m_priority.priorSecond)
				.arg(m_priority.priorThird).arg(m_priority.priorFourth)
				.arg(m_priority.priorFifth).arg(curRefSrc);
			db.selectFromMasterBoard(chCmd, data);
		}
	}
	else {
		if (m_priorityValue[m_priority.priorFirst - 1] != 1
			|| m_priorityValue[m_priority.priorSecond - 1] != 2
			|| m_priorityValue[m_priority.priorThird - 1] != 3
			|| m_priorityValue[m_priority.priorFourth - 1] != 4
			|| m_priorityValue[m_priority.priorFifth - 1] != 5) {
			int priorValue[5] = { 0 };
			//for (int i = 0; i < 5; ++i) {
			//	m_priorityValue[i]
			//}
			QString data = QString("priority,a,%1,%2,%3,%4,%5,%6")
				.arg(m_priority.priorFirst).arg(m_priority.priorSecond)
				.arg(m_priority.priorThird).arg(m_priority.priorFourth)
				.arg(m_priority.priorFifth).arg(m_priority.curRefSrc);
		}
	}
}

void TimeSrcTab::setTextAlignCenter(QComboBox *comboBox)
{
	int items = comboBox->count();
	if (0 == items) return;

	for (int i = 0; i < items; ++i) {
		static_cast<QStandardItemModel *>(comboBox->model())->item(i)->setTextAlignment(Qt::AlignCenter);
	}
}

void TimeSrcTab::connectSlots()
{
	connect(m_auto, SIGNAL(toggled(bool)), this, SLOT(slotOnSwitchAutoManual(bool)));
	connect(m_bdsPriority, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotOnCurrentIndexChanged(QString)));
	connect(m_gpsPriority, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotOnCurrentIndexChanged(QString)));
	connect(m_gloPriority, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotOnCurrentIndexChanged(QString)));
	connect(m_dcbPriority, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotOnCurrentIndexChanged(QString)));
	connect(m_acbPriority, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotOnCurrentIndexChanged(QString)));
	connect(m_confirm, SIGNAL(clicked()), this, SLOT(slotOnConfirmButtonClicked()));

	TransportThread *pTransport = TransportThread::Get();
	connect(pTransport, SIGNAL(gnsstimeSignal(st_Gnsstime)), this, SLOT(slotOnGnsstimeReached(st_Gnsstime)));
	connect(pTransport, SIGNAL(prioritySignal(QString)), this, SLOT(slotOnPriorityReached(QString)));
	connect(pTransport, SIGNAL(refavailinfoSignal(st_RefAvailInfo)), this, SLOT(slotOnRefUseokReached(st_RefAvailInfo)));
	connect(pTransport, SIGNAL(gnssstaSignal(st_Gnsssta)), this, SLOT(slotOnGnssstaReached(st_Gnsssta)));
	connect(pTransport, SIGNAL(statusSignal(st_Status)), this, SLOT(slotOnStatusReached(st_Status)));
}

void TimeSrcTab::switchAutoManual(bool isAuto, bool isInitial /*= false*/)
{
	bool isManual = !isAuto;
	QList<QAbstractButton *> refSrcButtons = m_refSrcGroup->buttons();
	for (auto &button : refSrcButtons) {
		button->setEnabled(isManual);
	}
	if (m_lockCurRefSrc) {
		switch (m_curRefSrc) {
		case GPS:
			m_gpsLock->setPixmap(m_lockClose);
			break;
		case GLONASS:
			m_gloLock->setPixmap(m_lockClose);
			break;
		case BDS:
			m_bdsLock->setPixmap(m_lockClose);
			break;
		case BAC:
			m_acbLock->setPixmap(m_lockClose);
			break;
		case BDC:
			m_dcbLock->setPixmap(m_lockClose);
			break;
		default:
			break;
		}
	}
	else {
		switch (m_curRefSrc) {
		case GPS:
			m_gpsLock->setPixmap(m_lockOpen);
			break;
		case GLONASS:
			m_gloLock->setPixmap(m_lockOpen);
			break;
		case BDS:
			m_bdsLock->setPixmap(m_lockOpen);
			break;
		case BAC:
			m_acbLock->setPixmap(m_lockOpen);
			break;
		case BDC:
			m_dcbLock->setPixmap(m_lockOpen);
			break;
		default:
			break;
		}
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

void TimeSrcTab::slotOnConfirmButtonClicked()
{
	emit changeParams();
}

void TimeSrcTab::slotOnGnsstimeReached(const st_Gnsstime &gnsstime)
{
	char szText[128] = { '\0' };

	structTime t = TimeStampToTime(gnsstime.UTCTime.toUInt());
	sprintf_s(szText, 128, "%04d/%02d/%02d %02d:%02d:%02d", t.Year, t.Month, t.Date, t.Hour, t.Minute, t.Second);
	m_bdsDateTime->setText(szText);
	m_gpsDateTime->setText(szText);
	m_gloDateTime->setText(szText);

	t = TimeStampToTime(gnsstime.dcbTime.toUInt(), false);
	sprintf_s(szText, 128, "%04d/%02d/%02d %02d:%02d:%02d", t.Year, t.Month, t.Date, t.Hour, t.Minute, t.Second);
	m_dcbDateTime->setText(szText);

	t = TimeStampToTime(gnsstime.acbTime.toUInt(), false);
	sprintf_s(szText, 128, "%04d/%02d/%02d %02d:%02d:%02d", t.Year, t.Month, t.Date, t.Hour, t.Minute, t.Second);
	m_acbDateTime->setText(szText);

	t = TimeStampToTime(gnsstime.inputTime.toUInt(), false);
	sprintf_s(szText, 128, "%04d/%02d/%02d %02d:%02d:%02d", t.Year, t.Month, t.Date, t.Hour, t.Minute, t.Second);
	m_inputDateTime->setText(szText);

	if (m_inputSetting->text().isEmpty()) {
		sprintf_s(szText, 128, "%04d%02d%02d%02d%02d%02d", t.Year, t.Month, t.Date, t.Hour, t.Minute, t.Second);
		m_inputSetting->setText(szText);
		m_strInputSetting = szText;
	}
}

void TimeSrcTab::slotOnPriorityReached(const QString &priority)
{
	switch (priority.at(0).toLatin1()) {
	case 'a':
		m_auto->setChecked(true);
		break;
	case 'b':
	case 'c':
		m_manual->setChecked(true);
		break;
	default:
		break;
	}

	for (int i = 1; i < 6; ++i) {
		int d = priority.at(i).digitValue();
		m_priorityGroup[d - 1]->setCurrentIndex(i - 1);
		m_priorityValue[d - 1] = i;
	}

	switch (priority.at(6).digitValue()) {
	case 0:
		m_input->setChecked(true);
		m_curRefSrc = SETTIME;
		break;
	case 1:
		m_gps->setChecked(true);
		m_curRefSrc = GPS;
		break;
	case 2:
		m_glo->setChecked(true);
		m_curRefSrc = GLONASS;
		break;
	case 3:
		m_bds->setChecked(true);
		m_curRefSrc = BDS;
		break;
	case 4:
		m_acb->setChecked(true);
		m_curRefSrc = BAC;
		break;
	case 5:
		m_dcb->setChecked(true);
		m_curRefSrc = BDC;
		break;
	default:
		break;
	}	

	m_priority.model = priority.at(0).toLatin1();
	m_priority.priorFirst = priority.at(1).toLatin1();
	m_priority.priorSecond = priority.at(2).toLatin1();
	m_priority.priorThird = priority.at(3).toLatin1();
	m_priority.priorFourth = priority.at(4).toLatin1();
	m_priority.priorFifth = priority.at(5).toLatin1();
	m_priority.curRefSrc = priority.at(6).toLatin1();
}

void TimeSrcTab::slotOnRefUseokReached(const st_RefAvailInfo &refAvail)
{
	QString QssNormalLabel("font: bold normal 16px \"arial\";");
	QString QssBlueLabel("font: bold normal 16px \"arial\"; color: #0306A8;");
	QString QssAbnormalLabel("font: bold normal 16px \"arial\"; color: lightgray;");
	
	if (refAvail.gpsuse) {
		m_gps->setStyleSheet(QssNormalLabel);
		m_gpsDateTime->setStyleSheet(QssBlueLabel);
		m_gpsAvlSatellites->setStyleSheet(QssBlueLabel);
	}
	else {
		m_gps->setStyleSheet(QssAbnormalLabel);
		m_gpsDateTime->setStyleSheet(QssAbnormalLabel);
		m_gpsAvlSatellites->setStyleSheet(QssAbnormalLabel);
	}

	if (refAvail.glouse) {
		m_glo->setStyleSheet(QssNormalLabel);
		m_gloDateTime->setStyleSheet(QssBlueLabel);
		m_gloAvlSatellites->setStyleSheet(QssBlueLabel);
	}
	else {
		m_glo->setStyleSheet(QssAbnormalLabel);
		m_gloDateTime->setStyleSheet(QssAbnormalLabel);
		m_gloAvlSatellites->setStyleSheet(QssAbnormalLabel);
	}

	if (refAvail.bdsuse) {
		m_bds->setStyleSheet(QssNormalLabel);
		m_bdsDateTime->setStyleSheet(QssBlueLabel);
		m_bdsAvlSatellites->setStyleSheet(QssBlueLabel);
	}
	else {
		m_bds->setStyleSheet(QssAbnormalLabel);
		m_bdsDateTime->setStyleSheet(QssAbnormalLabel);
		m_bdsAvlSatellites->setStyleSheet(QssAbnormalLabel);
	}

	if (refAvail.acbuse) {
		m_acb->setStyleSheet(QssNormalLabel);
		m_acbDateTime->setStyleSheet(QssBlueLabel);
	}
	else {
		m_acb->setStyleSheet(QssAbnormalLabel);
		m_acbDateTime->setStyleSheet(QssAbnormalLabel);
	}

	if (refAvail.dcbuse) {
		m_dcb->setStyleSheet(QssNormalLabel);
		m_dcbDateTime->setStyleSheet(QssBlueLabel);
	}
	else {				
		m_dcb->setStyleSheet(QssAbnormalLabel);
		m_dcbDateTime->setStyleSheet(QssAbnormalLabel);
	}
}

void TimeSrcTab::slotOnGnssstaReached(const st_Gnsssta &gnsssta)
{
	QString contents = tr("%1 颗").arg(gnsssta.avlSatellites);
	if (0 == strcmp(gnsssta.satelliteType, "gps")) {		
		m_gpsAvlSatellites->setText(contents);
	}
	else if (0 == strcmp(gnsssta.satelliteType, "glo")) {		
		m_gloAvlSatellites->setText(contents);
	}
	else if (0 == strcmp(gnsssta.satelliteType, "bds")) {		
		m_bdsAvlSatellites->setText(contents);
	}
}

void TimeSrcTab::slotOnStatusReached(const st_Status &status)
{
	switch (m_curRefSrc) {
	case GPS:
		m_gpsLock->clear();
		break;
	case GLONASS:
		m_gloLock->clear();
		break;
	case BDS:
		m_bdsLock->clear();
		break;
	case BAC:
		m_acbLock->clear();
		break;
	case BDC:
		m_dcbLock->clear();
		break;
	default:
		break;
	}

	m_curRefSrc = (RSFlag)status.curRef;
	QLabel *m_curLockLabel(nullptr);
	switch (m_curRefSrc) {
	case GPS:
		m_curLockLabel = m_gpsLock;
		break;
	case GLONASS:
		m_curLockLabel = m_gloLock;
		break;
	case BDS:
		m_curLockLabel = m_bdsLock;
		break;
	case BAC:
		m_curLockLabel = m_acbLock;
		break;
	case BDC:
		m_curLockLabel = m_dcbLock;
		break;
	default:
		break;
	}
	if (nullptr != m_curLockLabel) {
		if (0x02 == status.lockState) {
			m_curLockLabel->setPixmap(m_lockClose);
			m_lockCurRefSrc = true;
		}
		else {
			m_curLockLabel->setPixmap(m_lockOpen);
			m_lockCurRefSrc = false;
		}
	}
}

/****************************************************************************************
串口设置
*****************************************************************************************/
ComSettingsTab::ComSettingsTab(QWidget *parent /*= 0*/)
	: BaseWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
	, m_cmdList()
{
	int w = width();
	int h = height();
	
	QStringList baud;
	baud << QString::number(QSerialPort::Baud9600) 
		<< QString::number(QSerialPort::Baud19200) 
		<< QString::number(QSerialPort::Baud57600);

	m_debugLabel = new QLabel(tr("调试串口"), this);	

	m_debugCom = new QComboBox(this);
	m_debugCom->addItems(baud);
	m_debugCom->setCurrentIndex(0);

	m_firstLabel = new QLabel(tr("定时/串口1"), this);

	m_timingFirst = new QComboBox(this);
	m_timingFirst->addItems(baud);
	m_timingFirst->setCurrentIndex(0);

	m_secondLabel = new QLabel(tr("定时/串口2"), this);

	m_timingSecond = new QComboBox(this);
	m_timingSecond->addItems(baud);
	m_timingSecond->setCurrentIndex(0);

	m_confirm = new QPushButton(tr("确认设置"), this);
	setChildrenGeometry(w, h);

	setStyleSheet(QSS_ComSettingsLabel);

	setTextAlignCenter(m_debugCom);
	setTextAlignCenter(m_timingFirst);
	setTextAlignCenter(m_timingSecond);

	m_debug = QSerialPort::Baud9600;
	m_first = QSerialPort::Baud9600;
	m_second = QSerialPort::Baud9600;
	m_resultSuccess = 0;

	connectSlots();
}

void ComSettingsTab::changeStartup()
{
	if (0 == m_cmdList.size())  return;
	
	TimePositionDatabase db;
	unsigned char chCmd = COMMAND_IS_AT;
	for (int i = 0; i < m_cmdList.size(); ++i) {
		db.selectFromMasterBoard(chCmd, m_cmdList.at(i));
	}
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

	setChildrenGeometry(w, h);
}

void ComSettingsTab::setTextAlignCenter(QComboBox *comboBox)
{
	int items = comboBox->count();
	if (0 == items) return;

	for (int i = 0; i < items; ++i) {
		static_cast<QStandardItemModel *>(comboBox->model())->item(i)->setTextAlignment(Qt::AlignCenter);
	}
}

int ComSettingsTab::indexByText(const QString &text)
{
	int baud = text.toInt();
	switch (baud) {
	case QSerialPort::Baud9600:
		return 0;
	case QSerialPort::Baud19200:
		return 1;
	case QSerialPort::Baud57600:
		return 2;
	default:
		return -1;
	}
}

void ComSettingsTab::setChildrenGeometry(int w, int h)
{
	if (0 == w && 0 == h) return;

	int wdth = LblWidth;
	m_debugLabel->setGeometry(w / 4, (h - m_lblHeight * 8) / 2, m_lblWidth, m_lblHeight);
	m_debugCom->setGeometry(w / 2, (h - m_lblHeight * 8) / 2, wdth, m_lblHeight);
	m_firstLabel->setGeometry(w / 4, (h - m_lblHeight * 4) / 2, m_lblWidth, m_lblHeight);
	m_timingFirst->setGeometry(w / 2, (h - m_lblHeight * 4) / 2, wdth, m_lblHeight);
	m_secondLabel->setGeometry(w / 4, h / 2, m_lblWidth, m_lblHeight);
	m_timingSecond->setGeometry(w / 2, h / 2, wdth, m_lblHeight);
	m_confirm->setGeometry(w / 2 + wdth / 2, h * 3 / 4 - m_lblHeight, m_lblWidth, m_lblHeight);
}

void ComSettingsTab::connectSlots()
{
	TransportThread *pTrans = TransportThread::Get();
	connect(pTrans, SIGNAL(baudSignal(QString)), this, SLOT(slotOnBaudReceived(QString)));
	connect(m_confirm, SIGNAL(clicked()), this, SLOT(slotOnConfirmClicked()));
}

void ComSettingsTab::slotOnBaudReceived(const QString &ret)
{
	QStringList retLst = ret.split(",");
	if (4 == retLst.size()) {
		//m_debugCom->setCurrentText(retLst.at(1));  // 默认uneditable, 所以无效
		int idx = indexByText(retLst.at(1));
		m_debugCom->setCurrentIndex(idx);		
		idx = indexByText(retLst.at(2));
		m_timingFirst->setCurrentIndex(idx);		
		idx = indexByText(retLst.at(3));
		m_timingSecond->setCurrentIndex(idx);

		m_debug = retLst.at(1).toUInt();
		m_first = retLst.at(2).toUInt();
		m_second = retLst.at(3).toUInt();
		return;
	}
	if (3 == retLst.size()) {		
		if (0 == m_resultSuccess) {
			++m_resultSuccess;

			QMessageBox msgBox(QMessageBox::Information, tr("提示"), tr("参数修改成功!"), QMessageBox::NoButton);
			msgBox.addButton(tr("确认"), QMessageBox::AcceptRole);
			msgBox.setStyleSheet(QSS_MsgBox + QSS_PushButton);
			msgBox.exec();			
		}		

		if (0 == retLst.at(1).toUInt()) {
			m_debugCom->setCurrentIndex(indexByText(retLst.at(2)));
		}
		else if(1 == retLst.at(1).toUInt()) {		
			m_timingFirst->setCurrentIndex(indexByText(retLst.at(2)));
		}
		else if (2 == retLst.at(1).toUInt()) {
			m_timingSecond->setCurrentIndex(indexByText(retLst.at(2)));
		}
		return;
	}	
}

void ComSettingsTab::slotOnConfirmClicked()
{
	uint debug = m_debugCom->currentText().toUInt();
	uint first = m_timingFirst->currentText().toUInt();
	uint second = m_timingSecond->currentText().toUInt();

	int changes = 0;	
	if (m_debug != debug) {
		QString strCmd = QString("baud,0,%1").arg(debug);
		m_cmdList << strCmd;		
		++changes;
	}
	if (m_first != first) {
		QString strCmd = QString("baud,1,%1").arg(first);
		m_cmdList << strCmd;		
		++changes;
	}
	if (m_second != second) {
		QString strCmd = QString("baud,2,%1").arg(second);
		m_cmdList << strCmd;		
		++changes;
	}
	m_resultSuccess = 0;

	if (0 != changes)
		emit changeParams();
}

/****************************************************************************************
网口设置
*****************************************************************************************/
UnicastWidget::UnicastWidget(QWidget *parent /*= 0*/)
	: BaseWidget(parent)
{
	auto localIPLabel = new QLabel(tr("本机IP"), this);
	//m_localIP = new QLineEdit(tr("192.168.59.180"), this);
	m_localIP = new QLineEdit(this);
	m_localIP->setAlignment(Qt::AlignCenter);
	m_localIP->setMaximumWidth(LeWidth);
	m_ipConfirm = new QPushButton(tr("确认设置"), this);	

	auto localPortLabel = new QLabel(tr("本机端口"), this);
	//m_localPort = new QLineEdit(tr("6666"), this);
	m_localPort = new QLineEdit(this);
	m_localPort->setAlignment(Qt::AlignCenter);
	m_localPort->setMaximumWidth(LeWidth);
	m_portConfirm = new QPushButton(tr("确认设置"), this);

	auto submaskLabel = new QLabel(tr("本机掩码"), this);
	//m_submask = new QLineEdit(tr("255.255.255.0"), this);
	m_submask = new QLineEdit(this);
	m_submask->setAlignment(Qt::AlignCenter);
	m_submask->setMaximumWidth(LeWidth);
	m_maskConfirm = new QPushButton(tr("确认设置"), this);

	auto gatewayLabel = new QLabel(tr("本机网关"), this);
	//m_gateway = new QLineEdit(tr("192.168.59.1"), this);
	m_gateway = new QLineEdit(this);
	m_gateway->setAlignment(Qt::AlignCenter);
	m_gateway->setMaximumWidth(LeWidth);
	m_gatewayConfirm = new QPushButton(tr("确认设置"), this);

	auto remoteIPLabel = new QLabel(tr("目标IP"), this);
	//m_remoteIP = new QLineEdit(tr("192.168.59.117"), this);
	m_remoteIP = new QLineEdit(this);
	m_remoteIP->setAlignment(Qt::AlignCenter);
	m_remoteIP->setMaximumWidth(LeWidth);
	m_remoteIPConfirm = new QPushButton(tr("确认设置"), this);

	auto remotePortLabel = new QLabel(tr("目标端口"), this);
	//m_remotePort = new QLineEdit(tr("8888"), this);
	m_remotePort = new QLineEdit(this);
	m_remotePort->setAlignment(Qt::AlignCenter);
	m_remotePort->setMaximumWidth(LeWidth);
	m_remotePortConfirm = new QPushButton(tr("确认设置"), this);

	auto delayLabel = new QLabel(tr("网络延时"), this);
	//m_delay = new QLineEdit(tr("33"), this);
	m_delay = new QLineEdit(this);
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

	connectSlots();

	m_strLocalIP = QString();
	m_strRemoteIP = QString();
	m_uLocalPort = 0;
	m_uRemotePort = 0;
	m_strSubmask = QString();
	m_strGateway = QString();
	m_iDelay = 0;
	m_curNetNum = 0;
	m_curCmdStr = QString();
}

void UnicastWidget::setLocalIP(const QString &ip)
{
	m_localIP->setText(ip);
	m_strLocalIP = ip;
}

void UnicastWidget::setRemoteIP(const QString &ip)
{
	m_remoteIP->setText(ip);
	m_strRemoteIP = ip;
}

void UnicastWidget::setPorts(ushort local, ushort target)
{
	if (0 != local) {
		m_localPort->setText(QString::number(local));
		m_uLocalPort = local;
	}
	if (0 != target) {
		m_remotePort->setText(QString::number(target));
		m_uRemotePort = target;
	}
}

void UnicastWidget::setSubmask(const QString &mask)
{
	m_submask->setText(mask);
	m_strSubmask = mask;
}

void UnicastWidget::setGateway(const QString &gateway)
{
	m_gateway->setText(gateway);
	m_strGateway = gateway;
}

void UnicastWidget::setDelay(int delay)
{
	if (0 != delay) {
		m_delay->setText(QString::number(delay));
		m_iDelay = delay;
	}
}

void UnicastWidget::setCurNetNum(uint num)
{
	m_curNetNum = num;
}

void UnicastWidget::changeStartup()
{
	TimePositionDatabase db;
	unsigned char chCmd = COMMAND_IS_AT;
	BoardAddrFlag boardAddr = (m_curNetNum == 1) ? Net1Addr : Net2Addr;
	if (!m_curCmdStr.isEmpty()) {
		db.selectFromNetBoard(chCmd, m_curCmdStr, boardAddr);
	}
}

QString UnicastWidget::currentCmdStr() const
{
	return m_curCmdStr;
}

void UnicastWidget::paintEvent(QPaintEvent *event)
{
	// 使主窗口中设置的qss生效
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void UnicastWidget::connectSlots()
{
	connect(m_ipConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_portConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_maskConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_gatewayConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_remoteIPConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_remotePortConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_delayConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
}

void UnicastWidget::slotOnButtonsClicked()
{
	bool changed(false);
	do 
	{
		QString localIP = m_localIP->text();
		if (0 != m_strLocalIP.compare(localIP)) {
			changed = true;
			m_curCmdStr = QString("addrip,%1,%2").arg(m_curNetNum).arg(localIP);
			break;
		}

		ushort localPort = m_localPort->text().toUInt();
		ushort remotePort = m_remotePort->text().toUInt();
		if (m_uLocalPort != localPort || m_uRemotePort != remotePort) {
			changed = true;
			m_curCmdStr = QString("workport,%1,%2,%3").arg(m_curNetNum).arg(localPort).arg(remotePort);
			break;
		}

		QString submask = m_submask->text();
		if (0 != m_strSubmask.compare(submask)) {
			changed = true;
			m_curCmdStr = QString("submask,%1,%2").arg(m_curNetNum).arg(submask);
			break;
		}

		QString gateway = m_gateway->text();
		if (0 != m_strGateway.compare(gateway)) {
			changed = true;
			m_curCmdStr = QString("gateway,%1,%2").arg(m_curNetNum).arg(gateway);
			break;
		}

		QString remoteIP = m_remoteIP->text();
		if (0 != m_strRemoteIP.compare(remoteIP)) {
			changed = true;
			m_curCmdStr = QString("remoteip,%1,%2").arg(m_curNetNum).arg(remoteIP);
			break;
		}

		int delay = m_delay->text().toInt();
		if (m_iDelay != delay) {
			changed = true;
			m_curCmdStr = QString("delay,%1,%2").arg(m_curNetNum).arg(delay);
			break;
		}
	} while (false);

	if (changed)
		emit changeParams();
}

MulticastWidget::MulticastWidget(QWidget *parent /*= 0*/)
	: QWidget(parent)
{
	auto localIPLabel = new QLabel(tr("组播本机IP"), this);
	//m_localIP = new QLineEdit(tr("192.168.59.180"), this);
	m_localIP = new QLineEdit(this);
	m_localIP->setAlignment(Qt::AlignCenter);
	m_localIP->setMaximumWidth(LeWidth);
	m_ipConfirm = new QPushButton(tr("确认设置"), this);

	auto localPortLabel = new QLabel(tr("组播本机端口"), this);
	//m_localPort = new QLineEdit(tr("6666"), this);
	m_localPort = new QLineEdit(this);
	m_localPort->setAlignment(Qt::AlignCenter);
	m_localPort->setMaximumWidth(LeWidth);
	m_portConfirm = new QPushButton(tr("确认设置"), this);

	auto remoteIPLabel = new QLabel(tr("组播目标IP"), this);
	//m_remoteIP = new QLineEdit(tr("192.168.59.117"), this);
	m_remoteIP = new QLineEdit(this);
	m_remoteIP->setAlignment(Qt::AlignCenter);
	m_remoteIP->setMaximumWidth(LeWidth);
	m_remoteIPConfirm = new QPushButton(tr("确认设置"), this);

	auto remotePortLabel = new QLabel(tr("组播目标端口"), this);
	//m_remotePort = new QLineEdit(tr("8888"), this);
	m_remotePort = new QLineEdit(this);
	m_remotePort->setAlignment(Qt::AlignCenter);
	m_remotePort->setMaximumWidth(LeWidth);
	m_remotePortConfirm = new QPushButton(tr("确认设置"), this);

	auto delayLabel = new QLabel(tr("网络延时"), this);
	//m_delay = new QLineEdit(tr("33"), this);
	m_delay = new QLineEdit(this);
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

	connectSlots();

	m_strLocalIP = QString();
	m_strRemoteIP = QString();
	m_uLocalPort = 0;
	m_uRemotePort = 0;
	m_iDelay = 0;
	m_curNetNum = 0;
	m_curCmdStr = QString();
}

void MulticastWidget::setLocalIP(const QString &ip)
{
	m_localIP->setText(ip);
	m_strLocalIP = ip;
}

void MulticastWidget::setRemoteIP(const QString &ip)
{
	m_remoteIP->setText(ip);
	m_strRemoteIP = ip;
}

void MulticastWidget::setPorts(ushort local, ushort remote)
{
	if (0 != local) {
		m_localPort->setText(QString::number(local));
		m_uLocalPort = local;
	}
	if (0 != remote) {
		m_remotePort->setText(QString::number(remote));
		m_uRemotePort = remote;
	}
}

void MulticastWidget::setDelay(int delay)
{
	if (0 != delay) {
		m_delay->setText(QString::number(delay));
		m_iDelay = delay;
	}
}

void MulticastWidget::setCurNetNum(uint num)
{
	m_curNetNum = num;
}

QString MulticastWidget::currentCmdStr() const
{
	return m_curCmdStr;
}

void MulticastWidget::paintEvent(QPaintEvent *event)
{
	// 使主窗口中设置的qss生效
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MulticastWidget::connectSlots()
{
	connect(m_ipConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_portConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_remoteIPConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_remotePortConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_delayConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
}

void MulticastWidget::slotOnButtonsClicked()
{
	bool changed(false);
	do
	{
		QString localIP = m_localIP->text();
		if (0 != m_strLocalIP.compare(localIP)) {
			changed = true;
			m_curCmdStr = QString("groupipaddr,%1,%2").arg(m_curNetNum).arg(localIP);
			break;
		}

		ushort localPort = m_localPort->text().toUInt();
		ushort remotePort = m_remotePort->text().toUInt();
		if (m_uLocalPort != localPort || m_uRemotePort != remotePort) {
			changed = true;
			m_curCmdStr = QString("groupport,%1,%2,%3").arg(m_curNetNum).arg(localPort).arg(remotePort);
			break;
		}

		QString remoteIP = m_remoteIP->text();
		if (0 != m_strRemoteIP.compare(remoteIP)) {
			changed = true;
			m_curCmdStr = QString("remotegroupipaddr,%1,%2").arg(m_curNetNum).arg(remoteIP);
			break;
		}

		int delay = m_delay->text().toInt();
		if (m_iDelay != delay) {
			changed = true;
			m_curCmdStr = QString("delay,%1,%2").arg(m_curNetNum).arg(delay);
			break;
		}
	} while (false);

	if (changed)
		emit changeParams();
}

BroadcastWidget::BroadcastWidget(QWidget *parent /*= 0*/)
	: QWidget(parent)
{
	auto broadcastIPLabel = new QLabel(tr("广播IP"), this);
	//m_broadcastIP = new QLineEdit(tr("192.168.59.180"), this);
	m_broadcastIP = new QLineEdit(this);
	m_broadcastIP->setAlignment(Qt::AlignCenter);
	m_broadcastIP->setMaximumWidth(LeWidth);
	m_ipConfirm = new QPushButton(tr("确认设置"), this);

	auto broadcastPortLabel = new QLabel(tr("广播端口"), this);
	//m_broadcastPort = new QLineEdit(tr("6666"), this);
	m_broadcastPort = new QLineEdit(this);
	m_broadcastPort->setAlignment(Qt::AlignCenter);
	m_broadcastPort->setMaximumWidth(LeWidth);
	m_portConfirm = new QPushButton(tr("确认设置"), this);

	auto delayLabel = new QLabel(tr("网络延时"), this);
	//m_delay = new QLineEdit(tr("33"), this);
	m_delay = new QLineEdit(this);
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

	connectSlots();

	m_strBroadcastIP = QString();
	m_uBroadcastPort = 0;
	m_iDelay = 0;
	m_curNetNum = 0;
	m_curCmdStr = QString();
}

void BroadcastWidget::setIP(const QString &ip)
{
	m_broadcastIP->setText(ip);
	m_strBroadcastIP = ip;
}

void BroadcastWidget::setPort(ushort port)
{
	m_broadcastPort->setText(QString::number(port));
	m_uBroadcastPort = port;
}

void BroadcastWidget::setDelay(int delay)
{
	m_delay->setText(QString::number(delay));
	m_iDelay = delay;
}

void BroadcastWidget::setCurNetNum(uint num)
{
	m_curNetNum = num;
}

QString BroadcastWidget::currentCmdStr() const
{
	return m_curCmdStr;
}

void BroadcastWidget::paintEvent(QPaintEvent *event)
{
	// 使主窗口中设置的qss生效
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void BroadcastWidget::connectSlots()
{
	connect(m_ipConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_portConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
	connect(m_delayConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
}

void BroadcastWidget::slotOnButtonsClicked()
{
	bool changed(false);
	do
	{
		QString broadcastIP = m_broadcastIP->text();
		if (0 != m_strBroadcastIP.compare(broadcastIP)) {
			changed = true;
			m_curCmdStr = QString("broadcastipaddr,%1,%2").arg(m_curNetNum).arg(broadcastIP);
			break;
		}

		ushort broadcastPort = m_broadcastPort->text().toUInt();		
		if (m_uBroadcastPort != broadcastPort) {
			changed = true;
			m_curCmdStr = QString("broadcastport,%1,%2,%3").arg(m_curNetNum).arg(broadcastPort);
			break;
		}

		int delay = m_delay->text().toInt();
		if (m_iDelay != delay) {
			changed = true;
			m_curCmdStr = QString("delay,%1,%2").arg(m_curNetNum).arg(delay);
			break;
		}
	} while (false);

	if (changed)
		emit changeParams();
}

MonitorWidget::MonitorWidget(QWidget *parent /*= 0*/)
	: QWidget(parent)
{
	auto localIPLabel = new QLabel(tr("本机IP"), this);
	//m_localIP = new QLineEdit(tr("192.168.59.180"), this);
	m_localIP = new QLineEdit(this);
	m_localIP->setAlignment(Qt::AlignCenter);
	m_localIP->setMaximumWidth(LeWidth);

	auto submaskLabel = new QLabel(tr("本机掩码"), this);
	//m_submask = new QLineEdit(tr("255.255.255.0"), this);
	m_submask = new QLineEdit(this);
	m_submask->setAlignment(Qt::AlignCenter);
	m_submask->setMaximumWidth(LeWidth);

	auto gatewayLabel = new QLabel(tr("本机网关"), this);
	//m_gateway = new QLineEdit(tr("192.168.59.1"), this);
	m_gateway = new QLineEdit(this);
	m_gateway->setAlignment(Qt::AlignCenter);
	m_gateway->setMaximumWidth(LeWidth);

	auto recvPortLabel = new QLabel(tr("监控接收端口"), this);
	//m_recvPort = new QLineEdit(tr("20212"), this);
	m_recvPort = new QLineEdit(this);
	m_recvPort->setAlignment(Qt::AlignCenter);
	m_recvPort->setMaximumWidth(LeWidth);

	auto sendPortLabel = new QLabel(tr("监控发送端口"), this);
	//m_sendPort = new QLineEdit(tr("10212"), this);
	m_sendPort = new QLineEdit(this);
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

	connectSlots();

	m_uRecvPort = 0;
	m_uSendPort = 0;
}

void MonitorWidget::setIP(const QString &ip)
{
	m_localIP->setText(ip);
}

void MonitorWidget::setSubmask(const QString &mask)
{
	m_submask->setText(mask);
}

void MonitorWidget::setGateway(const QString &gateway)
{
	m_gateway->setText(gateway);
}

void MonitorWidget::setPorts(ushort recv, ushort send)
{
	if (0 != recv) {
		m_recvPort->setText(QString::number(recv));
		m_uRecvPort = recv;
	}
	if (0 != send) {
		m_sendPort->setText(QString::number(send));
		m_uSendPort = send;
	}
}

void MonitorWidget::setCurNetNum(uint num)
{
	m_curNetNum = num;
}

QString MonitorWidget::currentCmdStr() const
{
	return m_curCmdStr;
}

void MonitorWidget::paintEvent(QPaintEvent *event)
{
	// 使主窗口中设置的qss生效
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MonitorWidget::connectSlots()
{
	connect(m_sendPortConfirm, SIGNAL(clicked()), this, SLOT(slotOnButtonsClicked()));
}

void MonitorWidget::slotOnButtonsClicked()
{
	bool changed(false);
	do
	{
		ushort recvPort = m_recvPort->text().toUShort();
		ushort sendPort = m_sendPort->text().toUShort();
		if (m_uRecvPort != recvPort || m_uSendPort != sendPort) {
			changed = true;
			m_curCmdStr = QString("debugport,%1,%2,%3")
				.arg(m_curNetNum).arg(recvPort).arg(sendPort);
			break;
		}
	} while (false);

	if (changed)
		emit changeParams();
}

NetSettingsTab::NetSettingsTab(QWidget *parent)
	: BaseWidget(parent)
	, m_curCmdStr()
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
	
	// 使得GroupBox占满
	baseLayout->setMargin(0);
	baseLayout->setSpacing(0);	

	m_netBox->setProperty("netField", true);
	m_commBox->setProperty("commField", true);
	setStyleSheet(QSS_NetSettings.arg(LblWidth).arg(LblHeight));

	connectSlots();
}

void NetSettingsTab::changeStartup()
{
	QString strCmd;
	switch (m_commGroup->checkedId()) {
	case 0:
		strCmd = m_unicastWgt->currentCmdStr();
		break;
	case 1:
		strCmd = m_multicastWgt->currentCmdStr();
		break;
	case 2:
		strCmd = m_broadcastWgt->currentCmdStr();
		break;
	case 3:
		strCmd = m_monitorWgt->currentCmdStr();
		break;
	}
	m_curCmdStr = strCmd;

	TimePositionDatabase db;
	BoardAddrFlag boardAddr = (m_curNetNum == 1) ? Net1Addr : Net2Addr;
	db.selectFromNetBoard(COMMAND_IS_AT, strCmd, boardAddr);
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
	m_curNetNum = 1;

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
	TransportThread *pTransport = TransportThread::Get();
	connect(m_commGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotOnCommButtonClicked(int)));
	connect(m_netGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotOnNetButtonClicked(int)));
	connect(pTransport, SIGNAL(net1InfoSignal(st_NetInfo)), this, SLOT(slotOnNet1InfoReceived(st_NetInfo)));
	connect(pTransport, SIGNAL(net2InfoSignal(st_NetInfo)), this, SLOT(slotOnNet2InfoReceived(st_NetInfo)));
	connect(pTransport, SIGNAL(netResultSignal(QString)), this, SLOT(slotOnNetResultReceived(QString)));
	connect(m_unicastWgt, SIGNAL(changeParams()), this, SIGNAL(changeParams()));
	connect(m_multicastWgt, SIGNAL(changeParams()), this, SIGNAL(changeParams()));
	connect(m_broadcastWgt, SIGNAL(changeParams()), this, SIGNAL(changeParams()));
	connect(m_monitorWgt, SIGNAL(changeParams()), this, SIGNAL(changeParams()));
}

void NetSettingsTab::setNetInfo(const st_NetInfo &info)
{
	m_unicastWgt->setLocalIP(info.ip);
	m_unicastWgt->setPorts(info.worklocal, info.worktarget);
	m_unicastWgt->setSubmask(info.submask);
	m_unicastWgt->setGateway(info.gateway);
	m_unicastWgt->setRemoteIP(info.remoteip);
	m_unicastWgt->setDelay(info.delay);
	m_unicastWgt->setCurNetNum(m_curNetNum);

	m_multicastWgt->setLocalIP(info.groupip);
	m_multicastWgt->setPorts(info.grouplocal, info.grouptarget);
	m_multicastWgt->setRemoteIP(info.remotegroupip);
	m_multicastWgt->setDelay(info.delay);
	m_multicastWgt->setCurNetNum(m_curNetNum);

	m_broadcastWgt->setIP(info.broadcastip);
	m_broadcastWgt->setPort(info.broadcastport);
	m_broadcastWgt->setDelay(info.delay);
	m_broadcastWgt->setCurNetNum(m_curNetNum);

	m_monitorWgt->setIP(info.ip);
	m_monitorWgt->setSubmask(info.submask);
	m_monitorWgt->setGateway(info.gateway);
	m_monitorWgt->setPorts(info.debugrecv, info.debugsend);
	m_monitorWgt->setCurNetNum(m_curNetNum);
}

void NetSettingsTab::slotOnNetButtonClicked(int id)
{
	m_curNetNum = id + 1;
	if (1 == m_curNetNum) {
		if (m_net1Info.ip.isEmpty()) return;		
		setNetInfo(m_net1Info);
	}
	else {
		if (m_net2Info.ip.isEmpty()) return;
		setNetInfo(m_net2Info);		
	}
}

void NetSettingsTab::slotOnCommButtonClicked(int id)
{
	m_settingsLayout->setCurrentIndex(id);
}

void NetSettingsTab::slotOnNet1InfoReceived(const st_NetInfo &info)
{
	m_net1Info = info;

	TimePositionDatabase db;
	unsigned char chCmd = COMMAND_IS_AT;
	if (0 == m_net1Info.grouptarget) {		
		db.selectFromNetBoard(chCmd, "groupport,1", Net1Addr);
	}
	if (m_net1Info.remotegroupip.isEmpty()) {
		db.selectFromNetBoard(chCmd, "remotegroupipaddr,1", Net1Addr);
	}

	if (1 == m_curNetNum) {
		setNetInfo(m_net1Info);
	}
}

void NetSettingsTab::slotOnNet2InfoReceived(const st_NetInfo &info)
{
	m_net2Info = info;

	TimePositionDatabase db;
	unsigned char chCmd = COMMAND_IS_AT;
	if (0 == m_net2Info.grouptarget) {		
		db.selectFromNetBoard(chCmd, "groupport,2", Net2Addr);
	}
	if (m_net2Info.remotegroupip.isEmpty()) {
		db.selectFromNetBoard(chCmd, "remotegroupipaddr,2", Net2Addr);
	}

	if (2 == m_curNetNum) {
		setNetInfo(m_net2Info);
	}
}

void NetSettingsTab::slotOnNetResultReceived(const QString &res)
{
	if (0 == m_curCmdStr.compare(res)) {
		m_curCmdStr.clear();

		QMessageBox msgBox(QMessageBox::Information, tr("提示"), tr("参数修改成功!"), QMessageBox::NoButton);
		msgBox.addButton(tr("确认"), QMessageBox::AcceptRole);
		msgBox.setStyleSheet(QSS_MsgBox + QSS_PushButton);
		msgBox.exec();
	}

	if (res.startsWith("groupport")) {
		QStringList ports = res.split(",");
		ushort grouplocal = ports.at(2).toUShort();
		ushort grouptarget = ports.at(3).toUShort();
		if (1 == ports.at(1).toInt()) {
			m_net1Info.grouplocal = grouplocal;
			m_net1Info.grouptarget = grouptarget;
			if (1 == m_curNetNum) {
				m_multicastWgt->setPorts(grouplocal, grouptarget);
			}
		}
		else {
			m_net2Info.grouplocal = grouplocal;
			m_net2Info.grouptarget = grouptarget;
			if (2 == m_curNetNum) {
				m_multicastWgt->setPorts(grouplocal, grouptarget);
			}
		}
		return;
	}

	if (res.startsWith("remotegroupipaddr")) {
		QStringList resLst = res.split(",");
		if (1 == resLst.at(1).toInt()) {
			m_net1Info.remotegroupip = resLst.at(2);
			if (1 == m_curNetNum) {
				m_multicastWgt->setRemoteIP(m_net1Info.remotegroupip);
			}
		}
		else {
			m_net2Info.remotegroupip = resLst.at(2);
			if (2 == m_curNetNum) {
				m_multicastWgt->setRemoteIP(m_net2Info.remotegroupip);
			}
		}
		return;
	}

	if (res.startsWith("addrip")) {
		QStringList resLst = res.split(",");
		if (1 == resLst.at(1).toInt()) {
			m_net1Info.ip = resLst.at(2);
			if (1 == m_curNetNum) {
				m_unicastWgt->setLocalIP(m_net1Info.ip);
				m_monitorWgt->setIP(m_net1Info.ip);
			}
		}
		else {
			m_net2Info.ip = resLst.at(2);
			if (2 == m_curNetNum) {
				m_unicastWgt->setLocalIP(m_net2Info.ip);
				m_monitorWgt->setIP(m_net2Info.ip);
			}
		}
		return;
	}

	if (res.startsWith("workport")) {
		QStringList resLst = res.split(",");
		if (1 == resLst.at(1).toInt()) {
			m_net1Info.worklocal = resLst.at(2).toUShort();
			m_net1Info.worktarget = resLst.at(3).toUShort();
			if (1 == m_curNetNum) {
				m_unicastWgt->setPorts(m_net1Info.worklocal, m_net1Info.worktarget);
			}
		}
		else {
			m_net2Info.worklocal = resLst.at(2).toUShort();
			m_net2Info.worktarget = resLst.at(3).toUShort();
			if (2 == m_curNetNum) {
				m_unicastWgt->setPorts(m_net2Info.worklocal, m_net2Info.worktarget);
			}
		}
		return;
	}

	if (res.startsWith("submask")) {
		QStringList resLst = res.split(",");
		if (1 == resLst.at(1).toInt()) {
			m_net1Info.submask = resLst.at(2);			
			if (1 == m_curNetNum) {
				m_unicastWgt->setSubmask(m_net1Info.submask);
				m_monitorWgt->setSubmask(m_net1Info.submask);
			}
		}
		else {
			m_net2Info.submask = resLst.at(2);
			if (2 == m_curNetNum) {
				m_unicastWgt->setSubmask(m_net2Info.submask);
				m_monitorWgt->setSubmask(m_net2Info.submask);
			}
		}
		return;
	}

	if (res.startsWith("gateway")) {
		QStringList resLst = res.split(",");
		if (1 == resLst.at(1).toInt()) {
			m_net1Info.gateway = resLst.at(2);
			if (1 == m_curNetNum) {
				m_unicastWgt->setGateway(m_net1Info.gateway);
				m_monitorWgt->setGateway(m_net1Info.gateway);
			}
		}
		else {
			m_net2Info.gateway = resLst.at(2);
			if (2 == m_curNetNum) {
				m_unicastWgt->setGateway(m_net2Info.gateway);
				m_monitorWgt->setGateway(m_net2Info.gateway);
			}
		}
		return;
	}

	if (res.startsWith("remoteip")) {
		QStringList resLst = res.split(",");
		if (1 == resLst.at(1).toInt()) {
			m_net1Info.remoteip = resLst.at(2);
			if (1 == m_curNetNum) {
				m_unicastWgt->setRemoteIP(m_net1Info.remoteip);
			}
		}
		else {
			m_net2Info.remoteip = resLst.at(2);
			if (2 == m_curNetNum) {
				m_unicastWgt->setRemoteIP(m_net2Info.remoteip);
			}
		}
		return;
	}

	if (res.startsWith("delay")) {
		QStringList resLst = res.split(",");
		if (1 == resLst.at(1).toInt()) {
			m_net1Info.delay = resLst.at(2).toInt();
			if (1 == m_curNetNum) {
				m_unicastWgt->setDelay(m_net1Info.delay);
				m_multicastWgt->setDelay(m_net1Info.delay);
				m_broadcastWgt->setDelay(m_net1Info.delay);
			}
		}
		else {
			m_net2Info.delay = resLst.at(2).toInt();
			if (2 == m_curNetNum) {
				m_unicastWgt->setDelay(m_net2Info.delay);
				m_multicastWgt->setDelay(m_net2Info.delay);
				m_broadcastWgt->setDelay(m_net2Info.delay);
			}
		}
		return;
	}

	if (res.startsWith("groupipaddr")) {
		QStringList resLst = res.split(",");
		if (1 == resLst.at(1).toInt()) {
			m_net1Info.groupip = resLst.at(2);
			if (1 == m_curNetNum) {
				m_multicastWgt->setLocalIP(m_net1Info.groupip);
			}
		}
		else {
			m_net2Info.groupip = resLst.at(2);
			if (2 == m_curNetNum) {
				m_multicastWgt->setLocalIP(m_net2Info.groupip);
			}
		}
		return;
	}

	if (res.startsWith("broadcastipaddr")) {
		QStringList resLst = res.split(",");
		if (1 == resLst.at(1).toInt()) {
			m_net1Info.broadcastip = resLst.at(2);
			if (1 == m_curNetNum) {
				m_broadcastWgt->setIP(m_net1Info.broadcastip);
			}
		}
		else {
			m_net2Info.broadcastip = resLst.at(2);
			if (2 == m_curNetNum) {
				m_broadcastWgt->setIP(m_net2Info.broadcastip);
			}
		}
		return;
	}

	if (res.startsWith("broadcastport")) {
		QStringList resLst = res.split(",");
		if (1 == resLst.at(1).toInt()) {
			m_net1Info.broadcastport = resLst.at(2).toUShort();
			if (1 == m_curNetNum) {
				m_broadcastWgt->setPort(m_net1Info.broadcastport);
			}
		}
		else {
			m_net2Info.broadcastport = resLst.at(2).toUShort();
			if (2 == m_curNetNum) {
				m_broadcastWgt->setPort(m_net2Info.broadcastport);
			}
		}
		return;
	}

	if (res.startsWith("debugport")) {
		QStringList resLst = res.split(",");
		if (1 == resLst.at(1).toInt()) {
			m_net1Info.debugrecv = resLst.at(2).toUShort();
			m_net1Info.debugsend = resLst.at(3).toUShort();
			if (1 == m_curNetNum) {
				m_monitorWgt->setPorts(m_net1Info.debugrecv, m_net1Info.debugsend);
			}
		}
		else {
			m_net2Info.debugrecv = resLst.at(2).toUShort();
			m_net2Info.debugsend = resLst.at(3).toUShort();
			if (2 == m_curNetNum) {
				m_monitorWgt->setPorts(m_net2Info.debugrecv, m_net2Info.debugsend);
			}
		}
		return;
	}
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
	m_pwdTab = new PasswordChangeTab;
	m_verTab = new VersionInfoTab;
	
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
	m_centralWidget->addTab(m_pwdTab, tr("密码修改"));
	m_centralWidget->addTab(m_restoreTab, tr("还原设置"));
	m_centralWidget->addTab(m_verTab, tr("版本信息"));
	m_centralWidget->setCurrentIndex(0);
	//m_centralWidget->setUsesScrollButtons(false); // 将tab全部铺开, 不设左移右移按钮

	auto baseLayout = new QVBoxLayout(this);
	baseLayout->addWidget(m_centralWidget);
	baseLayout->setMargin(0);	

	setStyleSheet(QSS_StateParams.arg(valueLabelQss).arg(valueTableViewQss).arg(valueLightgrayQss));
	initTest();
	connectSlots();
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

void StateParamsTab::connectSlots()
{
	connect(m_pwdTab, SIGNAL(relogin()), this, SIGNAL(relogin()));
}

