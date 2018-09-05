#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;

QT_END_NAMESPACE

class TopWidget : public QWidget
{
	Q_OBJECT

public:
	TopWidget(QWidget *parent);
	~TopWidget();

protected:
	//void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void setChildrenGeometry(int w, int h);

private:
	void connectSlots();

signals:
	void homeBtn();
	void backBtn();

private:
	QPushButton *m_homeBtn;
	QPushButton *m_backBtn;
};
