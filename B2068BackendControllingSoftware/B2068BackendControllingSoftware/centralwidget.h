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

protected:
	void resizeEvent(QResizeEvent *event) override;
	void setChildrenGeometry(int w, int h);

private:
	void connectSlots();

private slots:
	void slotOnLeftBtnClicked(int id);

private:
	TopWidget *m_top;
	LeftWidget *m_left;
	QFrame *m_center;
	QStackedLayout *m_centerLayout;

	TimeSrcTab *m_timeSrc;
	ComSettingsTab *m_comSettings;
	NetSettingsTab *m_netSettings;
	StateMainTab *m_stateMain;
};
