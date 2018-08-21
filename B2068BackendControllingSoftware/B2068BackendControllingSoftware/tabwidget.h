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

private Q_SLOTS:
	// 想要在connect中使用SLOT, 必须将槽函数声明为Q_SLOTS
	void slotOnRefSrcTimeOut();

private:
	QLabel *m_deviceModel;
	QLabel *m_refSrc;
	QLabel *m_avlSatellites;
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

private:
	void connectSlots();
	void switchAutoManual(bool isAuto, bool isInitial = false);

private Q_SLOTS:
	void slotOnSwitchAutoManual(bool checked);
	void slotOnCurrentIndexChanged(const QString& text);

private:
	QRadioButton *m_manual;
	QRadioButton *m_auto;

	QLabel *m_bdsLock;
	QRadioButton *m_bds;
	QLabel *m_bdsDateTime;
	QLabel *m_bdsAvlSatellites;
	QComboBox *m_bdsPriority;

	QLabel *m_gpsLock;
	QRadioButton *m_gps;
	QLabel *m_gpsDateTime;
	QLabel *m_gpsAvlSatellites;
	QComboBox *m_gpsPriority;

	QLabel *m_gloLock;
	QRadioButton *m_glo;
	QLabel *m_gloDateTime;
	QLabel *m_gloAvlSatellites;
	QComboBox *m_gloPriority;

	QLabel *m_dcbLock;
	QRadioButton *m_dcb;
	QLabel *m_dcbDateTime;	
	QComboBox *m_dcbPriority;

	QLabel *m_acbLock;
	QRadioButton *m_acb;
	QLabel *m_acbDateTime;	
	QComboBox *m_acbPriority;
	
	QRadioButton *m_input;
	QLabel *m_inputDateTime;		

	QLineEdit *m_inputSetting;
	QPushButton *m_confirm;

	QButtonGroup *m_refSrcGroup;
	QComboBox *m_priorityGroup[5];
	int m_priorityValue[5];
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
