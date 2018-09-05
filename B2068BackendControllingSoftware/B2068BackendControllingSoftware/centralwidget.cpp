#include "stdafx.h"
#include "centralwidget.h"
#include "topwidget.h"
#include "leftwidget.h"
#include "tabwidget.h"
#include "stateparamswidget.h"

CentralWidget::CentralWidget(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(720, 440);
	m_top = new TopWidget(this);
	
	m_left = new LeftWidget(this);

	m_timeSrc = new TimeSrcTab();
	m_comSettings = new ComSettingsTab();
	m_netSettings = new NetSettingsTab();
	m_stateMain = new StateMainTab();
	m_center = new QFrame(this);
	m_centerLayout = new QStackedLayout;
	m_centerLayout->addWidget(m_timeSrc);
	m_centerLayout->addWidget(m_comSettings);
	m_centerLayout->addWidget(m_netSettings);
	m_centerLayout->addWidget(m_stateMain);
	m_centerLayout->setCurrentWidget(m_timeSrc);
	m_center->setLayout(m_centerLayout);

	connectSlots();

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);
}

CentralWidget::~CentralWidget()
{
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

void CentralWidget::connectSlots()
{
	connect(m_left, SIGNAL(leftButtonClicked(int)), this, SLOT(slotOnLeftBtnClicked(int)));
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
