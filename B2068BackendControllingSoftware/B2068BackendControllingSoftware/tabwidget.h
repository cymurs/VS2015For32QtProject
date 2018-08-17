#pragma once

#include <QDialog>
#include "ui_tabwidget.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

// 主界面
class MainTab : public QWidget
{
	Q_OBJECT

public:
	explicit MainTab(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;
	void	paintEvent(QPaintEvent *event) override;

private:
	void connectSlots();
	void slotOnRefSrcTimeOut();

private:
	QLabel *m_deviceModel;
	QLabel *m_referenceSrc;
	QLabel *m_availableSatallites;
	QLabel *m_signalState;
	QLabel *m_lockedState;
	QLabel *m_timeName;
	QLabel *m_refSrcTime;
	QLabel *m_refSrcDate;
	QLabel *m_position;	

private:
	// 测试
	QTimer m_refSrcTimer;
};

// 时间源
class TimeSrcTab : public QWidget
{
	Q_OBJECT

public:
	explicit TimeSrcTab(QWidget *parent = 0);
};

// 网口设置
class NetSettingsTab : public QWidget
{
	Q_OBJECT

public:
	explicit NetSettingsTab(QWidget *parent = 0);
};

class StateParamsTab : public QWidget
{
	Q_OBJECT

public:
	explicit StateParamsTab(QWidget *parent = 0);
};

class TabWidget : public QDialog
{
	Q_OBJECT

public:
	TabWidget(QWidget *parent = Q_NULLPTR);
	~TabWidget();

private:
	Ui::TabWidget ui;

private:
	QTabWidget *m_tabWidget;
};
