#include "stdafx.h"
#include "chiefwidget.h"
#include "tabwidget.h"
#include "centralwidget.h"

ChiefWidget::ChiefWidget(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(720, 440);

	m_signIn = new SignInWidget;
	m_main = new MainTab;
	m_center = new CentralWidget;
	m_baseLayout = new QStackedLayout;
	m_baseLayout->addWidget(m_signIn);
	m_baseLayout->addWidget(m_main);
	m_baseLayout->addWidget(m_center);
	m_baseLayout->setMargin(0);
	m_baseLayout->setSpacing(0);
	m_baseLayout->setCurrentIndex(0);
	setLayout(m_baseLayout);

	if (windowState() | Qt::WindowMaximized)
		setWindowState(windowState() & ~Qt::WindowMaximized);

	setWindowTitle(tr("B2068 ¿ØÖÆÈí¼þ V%1.%2").arg(LargeVer).arg(SmallVer));
	setWindowIcon(QIcon(":/BackendControlling/images/90.ico"));

	m_firstTime = true;
	connectSlots();
	loadConfig();
}

ChiefWidget::~ChiefWidget()
{
}

void ChiefWidget::closeEvent(QCloseEvent *event)
{
	storeConfig();
}

void ChiefWidget::connectSlots()
{
	connect(m_signIn, SIGNAL(login(bool)), this, SLOT(slotOnSignIn(bool)));
}

void ChiefWidget::loadConfig()
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

void ChiefWidget::storeConfig()
{
	QSettings file(ConfigFile, QSettings::IniFormat);
	QMap<QString, QString> &m = userPwd.user2pwd;
	for (auto user = m.begin(); user != m.end(); ++user) {
		file.setValue(tr("/USER/%1").arg(user.key()), user.value());
	}
}

void ChiefWidget::slotOnSignIn(bool signin)
{
	if (signin) {
		m_baseLayout->setCurrentIndex(1);
		if (m_firstTime) {
			//m_tabWidget->setCurrentIndex(0);
			m_firstTime = false;
		}
		QTimer::singleShot(LoginInterval, this, &ChiefWidget::slotOnLoginTimeout);
	}
	else {
		exit(0);
	}
}

void ChiefWidget::slotOnLoginTimeout()
{
	m_baseLayout->setCurrentIndex(0);
}
