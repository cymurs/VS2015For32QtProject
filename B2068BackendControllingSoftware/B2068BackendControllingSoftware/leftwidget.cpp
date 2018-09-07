#include "stdafx.h"
#include "leftwidget.h"

LeftWidget::LeftWidget(QWidget *parent)
	: QWidget(parent)
{
	QIcon icon(QPixmap(tr(":/BackendControlling/images/left_btn.png"), "PNG"));

	m_timeSrcBtn = new QPushButton(tr("时间源"), this);	
	m_comBtn = new QPushButton(tr("串口设置"), this);	
	m_netBtn = new QPushButton(tr("网络设置"), this);
	m_stateParamsBtn = new QPushButton(tr("状态参数"), this);

	m_timeSrcBtn->setFlat(true);
	m_comBtn->setFlat(true);
	m_netBtn->setFlat(true);
	m_stateParamsBtn->setFlat(true);

	m_btnGroup = new QButtonGroup;
	m_btnGroup->addButton(m_timeSrcBtn, 0);
	m_btnGroup->addButton(m_comBtn, 1);
	m_btnGroup->addButton(m_netBtn, 2);
	m_btnGroup->addButton(m_stateParamsBtn, 3);

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);	

	QString objName("leftwidget");
	setObjectName(objName);
	setStyleSheet(QSS_LeftWidget.arg(objName) + QSS_LeftWidgetDefaultButton);
	// 默认按下的button
	m_curBtn = m_timeSrcBtn;
	m_curBtn->setStyleSheet(QSS_LeftWidgetToggledButton);

	connectSlots();
}

LeftWidget::~LeftWidget()
{
}

void LeftWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void LeftWidget::resizeEvent(QResizeEvent *event)
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

void LeftWidget::setChildrenGeometry(int w, int h)
{
	if (0 == w && 0 == h) return;

	int left = LeftMargin * 0.25;
	int height = LblHeight * 2.5;
	int top = h / 4 + BottomMargin / 2 - height;
	m_timeSrcBtn->setGeometry(left, top, w - left, height); 
	m_comBtn->setGeometry(left, h / 2 - height, w - left, height);
	m_netBtn->setGeometry(left, h * 3 / 4 - BottomMargin / 2 - height, w - left, height);
	m_stateParamsBtn->setGeometry(left, h - BottomMargin - height, w - left, height);
}

void LeftWidget::connectSlots()
{
	//connect(m_timeSrcBtn, SIGNAL(clicked(bool)), this, SIGNAL(timeSrcBtn()));
	//connect(m_comBtn, SIGNAL(clicked(bool)), this, SIGNAL(comBtn()));
	//connect(m_netBtn, SIGNAL(clicked(bool)), this, SIGNAL(netBtn()));
	//connect(m_stateParamsBtn, SIGNAL(clicked(bool)), this, SIGNAL(stateBtn()));
	connect(m_btnGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(leftButtonClicked(int)));
	connect(m_btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotOnLeftButtonClicked(int)));
}

void LeftWidget::slotOnLeftButtonClicked(int id)
{
	m_curBtn->setStyleSheet(QSS_LeftWidgetDefaultButton); // 复原
	m_btnGroup->button(id)->setStyleSheet(QSS_LeftWidgetToggledButton); // 按下
	m_curBtn = dynamic_cast<QPushButton *>(m_btnGroup->button(id));
}
