#include "stdafx.h"
#include "topwidget.h"

TopWidget::TopWidget(QWidget *parent)
	: QWidget(parent)
{
	m_homeBtn = new QPushButton(this);
	m_homeBtn->setFlat(true);	// 去掉button边框
	m_homeBtn->setIcon(QIcon(QPixmap(":/BackendControlling/images/home.png", "PNG")));  // 设置背景图片

	m_backBtn = new QPushButton(this);
	m_backBtn->setFlat(true);   
	m_backBtn->setIcon(QIcon(QPixmap(":/BackendControlling/images/back.png", "PNG")));

	// 设置背景图片
	// [ 方法1 ]
	QPalette palette;
	//palette.setBrush(QPalette::Window,
	//	QBrush(QPixmap(":/BackendControlling/images/top_blue.png").scaled(
	//		size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	palette.setBrush(QPalette::Background,
		QBrush(QPixmap(":/BackendControlling/images/top_blue.png")));
	setPalette(palette);
	setAutoFillBackground(true); // 没有这一句，显示不出背景图
	// [ 方法2 ]  qss + paintEvent
	//setObjectName("topwidget");
	//setStyleSheet("QWidget#topwidget {background-image: url(:/BackendControlling/images/top_blue.png);}");

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);

	connectSlots();
}

TopWidget::~TopWidget()
{
}

//void TopWidget::paintEvent(QPaintEvent *event)
//{
//	QStyleOption opt;
//	opt.init(this);
//	QPainter p(this);
//	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//	QWidget::paintEvent(event);
//}

void TopWidget::resizeEvent(QResizeEvent *event)
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

void TopWidget::setChildrenGeometry(int w, int h)
{
	if (0 == w && 0 == h) return;

	QSize s(h - 5, h - 5);
	m_homeBtn->setGeometry(0, 0, h, h);
	m_backBtn->setGeometry(w - h, 0, h, h);
	// 使得图片铺满QPushButton
	m_homeBtn->setIconSize(s);
	m_backBtn->setIconSize(s);
}

void TopWidget::connectSlots()
{
	connect(m_homeBtn, SIGNAL(clicked(bool)), this, SIGNAL(homeBtn()));
	connect(m_backBtn, SIGNAL(clicked(bool)), this, SIGNAL(backBtn()));
}
