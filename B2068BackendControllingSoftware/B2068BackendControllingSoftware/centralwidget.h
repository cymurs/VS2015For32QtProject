#pragma once

#include <QWidget>

class TopWidget;
class LeftWidget;
class TimeSrcTab;
class ComSettingsTab;
class NetSettingsTab;

class DeviceOverviewTab;
class SatTypeStateTab;
class DCBStateTab;
class ACBStateTab;
class DelayCompensationTab;
class PulseSettingsTab;
class TimezoneTab;
class LogInfoTab;
class ScreenSettingTab;
class RestoreTab;
class FactorySettingTab;
class PasswordChangeTab;
class VersionInfoTab;
class StateMainTab;

class CentralWidget : public QWidget
{
	Q_OBJECT

public:
	CentralWidget(QWidget *parent = Q_NULLPTR);
	~CentralWidget();

	void changeStartup();

protected:
	void resizeEvent(QResizeEvent *event) override;
	void setChildrenGeometry(int w, int h);

private:
	void initTest();  // ½ö×ö²âÊÔ
	void connectSlots();

private slots:
	void slotOnBackBtnClicked();
	void slotOnLeftBtnClicked(int id);
	void slotOnStateParamsBtnClicked(int id);

signals:
	void home();
	void back();
	void changeParams();

private:
	TopWidget *m_top;
	LeftWidget *m_left;
	QFrame *m_center;
	QStackedLayout *m_centerLayout;

	TimeSrcTab *m_timeSrc;
	ComSettingsTab *m_comSettings;
	NetSettingsTab *m_netSettings;
	StateMainTab *m_stateMain;

	DeviceOverviewTab *m_dev;
	SatTypeStateTab *m_bds;
	SatTypeStateTab *m_gps;
	SatTypeStateTab *m_glo;
	ACBStateTab *m_acb;
	DCBStateTab *m_dcb;	
	DelayCompensationTab *m_delay;
	PulseSettingsTab *m_pulse;
	TimezoneTab *m_timezone;
	LogInfoTab *m_log;
	LogInfoTab *m_alarm;
	ScreenSettingTab *m_buzz;	 // ·äÃùÆ÷
	FactorySettingTab *m_factory;
	PasswordChangeTab *m_pwd;
	RestoreTab *m_restore;
	VersionInfoTab *m_ver;
};
