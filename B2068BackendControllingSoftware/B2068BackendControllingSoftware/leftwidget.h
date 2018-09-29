#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;

QT_END_NAMESPACE

class LeftWidget : public QWidget
{
	Q_OBJECT

public:
	LeftWidget(QWidget *parent);
	~LeftWidget();

protected:
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void setChildrenGeometry(int w, int h);

private:
	void connectSlots();
	void queryBoardInfo();

signals:
	void leftButtonClicked(int id);

private slots:
	void slotOnLeftButtonClicked(int id);

private:
	QPushButton *m_timeSrcBtn;
	QPushButton *m_comBtn;
	QPushButton *m_netBtn;
	QPushButton *m_stateParamsBtn;
	QButtonGroup *m_btnGroup;
	QPushButton *m_curBtn;

	bool m_queryOver;
};