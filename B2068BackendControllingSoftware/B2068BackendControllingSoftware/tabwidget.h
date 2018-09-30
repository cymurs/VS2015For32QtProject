#pragma once

#include <QDialog>
#include "ui_tabwidget.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QSerialPort;
QT_END_NAMESPACE

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

// 需要继承的父界面
class BaseWidget : public QWidget
{
	Q_OBJECT

public:
	BaseWidget(QWidget *parent = 0);

	virtual void changeStartup();

signals:
	void changeParams();
};

// 登录界面
class SignInWidget : public QWidget
{
	Q_OBJECT

public:
	SignInWidget(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	
	void setTextAlignCenter(QComboBox *comboBox);

private:
	void setChildrenGeometry(int w = 0, int h = 0);
	void connectSlots();

private slots:
	void slotOnLoginClicked(bool checked);
	void slotOnQuitClicked();

signals:
	void login(bool checked = false);

private:
	QLabel *m_userLabel;
	QComboBox *m_user;
	QLabel *m_pwdLabel;
	QLineEdit *m_pwd;
	QPushButton *m_login;
	QPushButton *m_quit;
	int m_pwdArray[3];

	const int m_lblWidth;
	const int m_lblHeight;
};

// 主界面
class MainTab : public QWidget
{
	Q_OBJECT

public:
	explicit MainTab(QWidget *parent = 0);
	virtual ~MainTab();

protected:
	void resizeEvent(QResizeEvent *event) override;
	void	paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	QString LonLatConvert(float value);

private:
	void connectSlots();
	void queryBoardInfo();

private Q_SLOTS:
	// 想要在connect中使用SLOT, 必须将槽函数声明为Q_SLOTS
	void slotOnRefSrcTimeOut();

	void slotOnStatusReached(const st_Status &status);
	void slotOnGnsstimeReached(const st_Gnsstime &gnsstime);
	void slotOnGnssstaReached(const st_Gnsssta &gnsssta);
	void slotOnB2068Reached(int mark);

signals:
	void fadeOut();

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
	QPropertyAnimation *m_animation;
	QPixmap m_signNormal;
	QPixmap m_signAbnormal;
	QPixmap m_lockClose;
	QPixmap m_lockOpen;

private:
	// 测试
	QTimer m_refSrcTimer;

	int m_iRefSrc;
	int m_iAvlSatellites;
	bool m_bSignNormal;
	bool m_bLockState;
};

// 时间源
class TimeSrcTab : public BaseWidget
{
	Q_OBJECT

public:
	explicit TimeSrcTab(QWidget *parent = 0);

	void changeStartup() override;

protected:
	void setTextAlignCenter(QComboBox *comboBox);

private:
	void connectSlots();
	void switchAutoManual(bool isAuto, bool isInitial = false);

private Q_SLOTS:
	void slotOnSwitchAutoManual(bool checked);
	void slotOnCurrentIndexChanged(const QString& text);
	void slotOnConfirmButtonClicked();
	void slotOnGnsstimeReached(const st_Gnsstime &gnsstime);
	void slotOnPriorityReached(const QString &priority);
	void slotOnRefUseokReached(const st_RefAvailInfo &refAvail);
	void slotOnGnssstaReached(const st_Gnsssta &gnsssta);
	void slotOnStatusReached(const st_Status &status);

private:
	QRadioButton *m_manual;
	QRadioButton *m_auto;
	QButtonGroup *m_operatingGroup;

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
	st_TimeSrcPriority m_priority;
	QComboBox *m_priorityGroup[5];
	int m_priorityValue[5];

	QString m_strInputSetting;
	QPixmap m_lockOpen;
	QPixmap m_lockClose;
	RSFlag m_curRefSrc;	
	bool m_lockCurRefSrc;

};

// 串口设置
class ComSettingsTab : public BaseWidget
{
	Q_OBJECT

public:
	explicit ComSettingsTab(QWidget *parent = 0);

	void changeStartup() override;

protected:
	void resizeEvent(QResizeEvent *event) override;

	void setTextAlignCenter(QComboBox *comboBox);
	int indexByText(const QString &text);

private:
	void setChildrenGeometry(int w, int h);
	void connectSlots();

signals:
	void changeParams();

private slots:
	void slotOnBaudReceived(const QString &ret);
	void slotOnConfirmClicked();

private:
	QLabel *m_debugLabel;
	QComboBox *m_debugCom;
	QLabel *m_firstLabel;
	QComboBox *m_timingFirst;
	QLabel *m_secondLabel;
	QComboBox *m_timingSecond;
	QPushButton *m_confirm;

	const int m_lblWidth;
	const int m_lblHeight;

	QStringList m_cmdList;
	uint m_debug;
	uint m_first;
	uint m_second;
	int m_resultSuccess;
};

// 单播
class UnicastWidget : public BaseWidget
{
	Q_OBJECT

public:
	explicit UnicastWidget(QWidget *parent = 0);

	void setLocalIP(const QString &ip);
	void setRemoteIP(const QString &ip);
	void setPorts(ushort local, ushort target);
	void setSubmask(const QString &mask);
	void setGateway(const QString &gateway);
	void setDelay(int delay);
	void setCurNetNum(uint num);
	void changeStartup() override;
	QString currentCmdStr() const;

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void connectSlots();

signals:
	void changeParams();

private slots:
	void slotOnButtonsClicked();

private:
	QLineEdit *m_localIP;
	QPushButton *m_ipConfirm;
	QLineEdit *m_localPort;
	QPushButton *m_portConfirm;
	QLineEdit *m_submask;
	QPushButton *m_maskConfirm;
	QLineEdit *m_gateway;
	QPushButton *m_gatewayConfirm;
	QLineEdit *m_remoteIP;
	QPushButton *m_remoteIPConfirm;
	QLineEdit *m_remotePort;
	QPushButton *m_remotePortConfirm;
	QLineEdit *m_delay;
	QPushButton *m_delayConfirm;

	QString m_strLocalIP;
	QString m_strRemoteIP;
	ushort m_uLocalPort;
	ushort m_uRemotePort;
	QString m_strSubmask;
	QString m_strGateway;
	int m_iDelay;
	uint m_curNetNum;
	QString m_curCmdStr;
};

// 组播
class MulticastWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MulticastWidget(QWidget *parent = 0);

	void setLocalIP(const QString &ip);
	void setRemoteIP(const QString &ip);
	void setPorts(ushort local, ushort remote);
	void setDelay(int delay);
	void setCurNetNum(uint num);
	QString currentCmdStr() const;

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void connectSlots();

signals:
	void changeParams();

private slots:
	void slotOnButtonsClicked();

private:
	QLineEdit *m_localIP;
	QPushButton *m_ipConfirm;
	QLineEdit *m_localPort;
	QPushButton *m_portConfirm;
	QLineEdit *m_remoteIP;
	QPushButton *m_remoteIPConfirm;
	QLineEdit *m_remotePort;
	QPushButton *m_remotePortConfirm;
	QLineEdit *m_delay;
	QPushButton *m_delayConfirm;

	QString m_strLocalIP;
	ushort m_uLocalPort;
	ushort m_uRemotePort;
	QString m_strRemoteIP;
	int m_iDelay;
	uint m_curNetNum;
	QString m_curCmdStr;
};

// 广播
class BroadcastWidget : public QWidget
{
	Q_OBJECT

public:
	explicit BroadcastWidget(QWidget *parent = 0);

	void setIP(const QString &ip);
	void setPort(ushort port);
	void setDelay(int delay);
	void setCurNetNum(uint num);
	QString currentCmdStr() const;

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void connectSlots();

signals:
	void changeParams();

private slots:
	void slotOnButtonsClicked();

private:
	QLineEdit *m_broadcastIP;
	QPushButton *m_ipConfirm;
	QLineEdit *m_broadcastPort;
	QPushButton *m_portConfirm;
	QLineEdit *m_delay;
	QPushButton *m_delayConfirm;

	QString m_strBroadcastIP;
	ushort m_uBroadcastPort;
	int m_iDelay;
	uint m_curNetNum;
	QString m_curCmdStr;
};

// 监控
class MonitorWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MonitorWidget(QWidget *parent = 0);

	void setIP(const QString &ip);
	void setSubmask(const QString &mask);
	void setGateway(const QString &gateway);
	void setPorts(ushort recv, ushort send);
	void setCurNetNum(uint num);
	QString currentCmdStr() const;

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void connectSlots();

signals:
	void changeParams();

private slots:
	void slotOnButtonsClicked();

private:
	QLineEdit *m_localIP;
	QPushButton *m_ipConfirm;	
	QLineEdit *m_submask;
	QPushButton *m_maskConfirm;
	QLineEdit *m_gateway;
	QPushButton *m_gatewayConfirm;
	QLineEdit *m_recvPort;
	QLineEdit *m_sendPort;
	QPushButton *m_sendPortConfirm;

	ushort m_uRecvPort;
	ushort m_uSendPort;
	uint m_curNetNum;
	QString m_curCmdStr;
};

// 网口设置
class NetSettingsTab : public BaseWidget
{
	Q_OBJECT

public:
	explicit NetSettingsTab(QWidget *parent = 0);

	void changeStartup() override;

protected:
	//void resizeEvent(QResizeEvent *event) override;

private:
	QGroupBox *createNetExclusiveGroup();
	QGroupBox *createCommExclusiveGroup();
	void connectSlots();
	void setNetInfo(const st_NetInfo &info);

signals:
	void changeParams();

private Q_SLOTS:
	void slotOnNetButtonClicked(int id);
	void slotOnCommButtonClicked(int id);
	void slotOnNet1InfoReceived(const st_NetInfo &info);
	void slotOnNet2InfoReceived(const st_NetInfo &info);
	void slotOnNetResultReceived(const QString &res);

private:
	QRadioButton *m_firstNet;
	QRadioButton *m_secondNet;
	QButtonGroup *m_netGroup;
	QGroupBox *m_netBox;

	QRadioButton *m_unicast;
	QRadioButton *m_multicast;
	QRadioButton *m_broadcast;
	QRadioButton *m_monitor;
	QButtonGroup *m_commGroup;
	QGroupBox *m_commBox;

	UnicastWidget *m_unicastWgt;
	MulticastWidget *m_multicastWgt;
	BroadcastWidget *m_broadcastWgt;
	MonitorWidget *m_monitorWgt;
	QStackedLayout *m_settingsLayout;

	uint m_curNetNum;
	st_NetInfo m_net1Info;
	st_NetInfo m_net2Info;
	QString m_curCmdStr;
};

// 状态参数
class StateParamsTab : public QWidget
{
	Q_OBJECT

public:
	explicit StateParamsTab(QWidget *parent = 0);	

private:
	void initTest();  // 仅做测试
	void connectSlots();

signals:
	void relogin();

private:
	QTabWidget *m_centralWidget;
	DeviceOverviewTab *m_deviceTab;
	SatTypeStateTab *m_bdsTab;
	SatTypeStateTab *m_gpsTab;
	SatTypeStateTab *m_gloTab;
	DCBStateTab *m_dcbTab;
	ACBStateTab *m_acbTab;
	DelayCompensationTab *m_delayTab;
	PulseSettingsTab *m_pulseTab;
	TimezoneTab *m_tzTab;
	LogInfoTab *m_logTab;
	LogInfoTab *m_alarmTab;
	ScreenSettingTab *m_buzzTab;
	RestoreTab *m_restoreTab;
	FactorySettingTab *m_factoryTab;
	PasswordChangeTab *m_pwdTab;
	VersionInfoTab *m_verTab;	
};

// 总界面
class TabWidget : public QDialog
{
	Q_OBJECT

public:
	TabWidget(QWidget *parent = Q_NULLPTR);
	~TabWidget();

protected:
	void	closeEvent(QCloseEvent *event) override;


private:
	void connectSlots();
	void loadConfig();
	void storeConfig();

private slots:
	void slotOnSignIn(bool signin);
	void slotOnLoginTimeout();

private:
	Ui::TabWidget ui;

private:
	QStackedLayout *m_centralLayout;
	QTabWidget *m_tabWidget;
	SignInWidget *m_signIn;
	StateParamsTab *m_stateParams;
	
	bool m_firstTime;
};
