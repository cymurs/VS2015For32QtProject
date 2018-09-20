#include "stdafx.h"
#include "chiefwidget.h"
#include "tabwidget.h"
#include "centralwidget.h"
#include "commwidget.h"

ChiefWidget::ChiefWidget(QWidget *parent)
	: QWidget(parent)
{
	//setFixedSize(720, 440);
	setFixedSize(600, 400);

	m_comm = new CommWidget;
	m_signIn = new SignInWidget;
	m_main = new MainTab;
	m_center = new CentralWidget;
	m_baseLayout = new QStackedLayout;
	m_baseLayout->addWidget(m_comm);
	m_baseLayout->addWidget(m_signIn);
	m_baseLayout->addWidget(m_main);
	m_baseLayout->addWidget(m_center);
	m_baseLayout->setMargin(0);
	m_baseLayout->setSpacing(0);
	m_baseLayout->setCurrentIndex(0);
	setLayout(m_baseLayout);

	if (windowState() | Qt::WindowMaximized)
		setWindowState(windowState() & ~Qt::WindowMaximized);

	setWindowTitle(tr("B2068 控制软件 V%1.%2").arg(LargeVer).arg(SmallVer));
	setWindowIcon(QIcon(":/BackendControlling/images/90.ico"));

		
	m_animation = new QPropertyAnimation(m_main, QByteArray());
	m_animation->setDuration(5000);
	
	connectSlots();
	loadConfig();
	m_firstTime = true;
}

ChiefWidget::~ChiefWidget()
{
}

void ChiefWidget::closeEvent(QCloseEvent *event)
{
	if (isQuit())
	{
		storeConfig();
		event->accept();
	}
	else {
		event->ignore();
	}
}

bool ChiefWidget::isQuit()
{
	// 询问
	QMessageBox qesBox(QMessageBox::Question, tr("退出"), tr("是否确定要退出?"), QMessageBox::NoButton, this);
	qesBox.addButton(tr("是"), QMessageBox::YesRole);
	qesBox.addButton(tr("否"), QMessageBox::NoRole);
	qesBox.setStyleSheet(QSS_MsgBox + QSS_PushButton);

	if (0 == qesBox.exec())
		return true;
	else
		return false;
}

void ChiefWidget::connectSlots()
{
	connect(m_comm, SIGNAL(connectSuccess()), this, SLOT(slotOnConnect()));
	connect(m_signIn, SIGNAL(login(bool)), this, SLOT(slotOnSignIn(bool)));
	connect(m_main, SIGNAL(fadeOut()), this, SLOT(slotOnAnimationFinished()));
	connect(m_animation, SIGNAL(finished()), this, SLOT(slotOnAnimationFinished()));
	connect(m_center, SIGNAL(home()), this, SLOT(slotOnGoHome()));
}

void ChiefWidget::loadConfig()
{
	if (!QFile::exists(ConfigFile)) {
		userPwd.user2pwd["user1"] = "888888";
		userPwd.user2pwd["user2"] = "888888";
		userPwd.user2pwd["user3"] = "888888";
		return;
	}

	QSettings file(ConfigFile, QSettings::IniFormat);
	userPwd.user2pwd["user1"] = file.value("/USER/user1").toString();
	userPwd.user2pwd["user2"] = file.value("/USER/user2").toString();
	userPwd.user2pwd["user3"] = file.value("/USER/user3").toString();
}

void ChiefWidget::storeConfig()
{
	QSettings file(ConfigFile, QSettings::IniFormat);
	QMap<QString, QString> &m = userPwd.user2pwd;
	for (auto user = m.begin(); user != m.end(); ++user) {
		file.setValue(tr("/USER/%1").arg(user.key()), user.value());
	}
}

void ChiefWidget::slotOnConnect()
{
	m_baseLayout->setCurrentIndex(1);
}

void ChiefWidget::slotOnSignIn(bool signin)
{
	if (signin) {
		m_baseLayout->setCurrentIndex(2);
		if (m_firstTime) {
			//m_tabWidget->setCurrentIndex(0);
			m_firstTime = false;
		}
		QTimer::singleShot(LoginInterval, this, &ChiefWidget::slotOnLoginTimeout);
	}
	else {
		if (isQuit())
		{
			storeConfig();
			exit(0);
		}		
	}
}

void ChiefWidget::slotOnLoginTimeout()
{
	m_baseLayout->setCurrentIndex(1);
}

void ChiefWidget::slotOnMainFadeOut()
{
	m_animation->setTargetObject(m_main);
	m_animation->setStartValue(1);
	m_animation->setEndValue(0);
	m_animation->start();

	//m_baseLayout->setCurrentIndex(2);
}

void ChiefWidget::slotOnAnimationFinished()
{
	m_baseLayout->setCurrentIndex(3);
}

void ChiefWidget::slotOnGoHome()
{
	m_baseLayout->setCurrentIndex(2);
}
