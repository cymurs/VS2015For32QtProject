#pragma once

#include <QWidget>

class CRadar;
extern const int MaxAvlSatellites;

// 仰角和方位角结构体
struct ElevationAzimuthParams {
	double elevations[MaxAvlSatellites] = {0};
	double azimuths[MaxAvlSatellites] = {0};
	int count{0};
};

// 设备总览
class DeviceOverviewTab : public QWidget
{
	Q_OBJECT

public:
	DeviceOverviewTab(QWidget *parent = 0);	

private:
	QLabel *m_runTime;
	QLabel *m_vibrationType;
	QLabel *m_tamingState;
	QLabel *m_leapTime;
	QLabel *m_curTimezone;
	QLabel *m_curRefSrc;
	QTableWidget *m_refSrcTable;
};

// 北斗状态、GPS状态、GLONASS状态
class SatTypeStateTab : public QWidget
{
	Q_OBJECT

public:
	SatTypeStateTab(QWidget *parent = 0);

	void setGnssstaInfo(const st_Gnsssta &gnsssta);
	void setGnssgsvInfo(const st_Gnssgsv &gnssgsv);

private:
	QPointF angleToCoordinate(double elev, double azim);

private:
	QLabel *m_workState;
	QLabel *m_signState;
	QLabel *m_pdop;
	QLabel *m_visSatellites;
	QLabel *m_avlSatellites;
	QTableView *m_satTypeTable;
	CRadar *m_radar;
	ElevationAzimuthParams *m_elevAzimParams;
};

// 直流B码状态
class DCBStateTab : public QWidget
{
	Q_OBJECT

public:
	DCBStateTab(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;

	void setTextAlignCenter(QComboBox *comboBox);

private:
	void setChildrenGeometry(int w = 0, int h = 0);

private:
	QLabel *m_runStateLabel;
	QLabel *m_runState;
	QLabel *m_synStateLabel;
	QLabel *m_synState;
	QLabel *m_inFormatLabel;
	QComboBox *m_inFormat;
	QLabel *m_outFormatLabel;
	QComboBox *m_outFormat;
	QPushButton *m_confirm;	

	const int m_lblWidth;
	const int m_lblHeight;
};

// 交流B码状态
class ACBStateTab : public QWidget
{
	Q_OBJECT

public:
	ACBStateTab(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;

	void setTextAlignCenter(QComboBox *comboBox);

private:
	void setChildrenGeometry(int w = 0, int h = 0);

private:
	QLabel *m_runStateLabel;
	QLabel *m_runState;
	QLabel *m_synStateLabel;
	QLabel *m_synState;
	QLabel *m_inFormatLabel;
	QComboBox *m_inFormat;
	QLabel *m_outFormatLabel;
	QComboBox *m_outFormat;
	QLabel *m_outAmpLabel;
	QLineEdit *m_outAmplitude;
	QLabel *m_outAmpUnit;
	QLabel *m_outMrLabel;
	QComboBox *m_outMr;
	QPushButton *m_confirm;

	const int m_lblWidth;
	const int m_lblHeight;
};

// 延时补偿
class DelayCompensationTab : public QWidget
{
	Q_OBJECT

public:
	DelayCompensationTab(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	void setChildrenGeometry(int w = 0, int h = 0);

private:
	QLabel *m_bdsLabel;
	QLineEdit *m_bds;
	QLabel *m_bdsUnit;
	QLabel *m_gpsLabel;
	QLineEdit *m_gps;
	QLabel *m_gpsUnit;
	QLabel *m_gloLabel;
	QLineEdit *m_glo;
	QLabel *m_gloUnit;
	QLabel *m_acbLabel;
	QLineEdit *m_acb;
	QLabel *m_acbUnit;
	QLabel *m_dcbLabel;
	QLineEdit *m_dcb;
	QLabel *m_dcbUnit;
	QPushButton *m_confirm;

	const int m_lblWidth;
	const int m_lblHeight;
};

// 脉冲配置
class PulseSettingsTab : public QWidget
{
	Q_OBJECT

public:
	PulseSettingsTab(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	void setChildrenGeometry(int w = 0, int h = 0);
	void connectSlots();

public Q_SLOTS:
	void slotOnWidthUnitChanged(int index);
	void slotOnDelayUnitChanged(int index);

private:
	QLabel *m_ppsWidthLabel;
	QLineEdit *m_ppsWidth;
	QComboBox *m_ppsWidthUnit;
	QLabel *m_ppsDelayLabel;
	QLineEdit *m_ppsDelay;
	QComboBox *m_ppsDelayUnit;
	QPushButton *m_confirm;

	QValidator *m_msWidthValidator;
	QValidator *m_nsWidthValidator;
	QValidator *m_msDelayValidator;
	QValidator *m_nsDelayValidator;

	const int m_lblWidth;
	const int m_lblHeight;
};

// 时区设置
class TimezoneTab : public QWidget
{
	Q_OBJECT

public:
	TimezoneTab(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	void setChildrenGeometry(int w = 0, int h = 0);

private:
	QLabel *m_timezoneLabel;
	QLineEdit *m_timezone;
	QPushButton *m_confirm;

	const int m_lblWidth;
	const int m_lblHeight;
};

// 日志信息
class LogInfoTab : public QWidget
{
	Q_OBJECT

public:
	LogInfoTab(QWidget *parent = 0);

	void prepend(const QString &info);
	void append(const QString &info);
protected:
	//void resizeEvent(QResizeEvent *event) override;

private:
	//void setChildrenGeometry(int w = 0, int h = 0);

private:
	QTextEdit *m_log;

	const int m_maxCount;
};

// 告警信息
//class AlarmInfoTab : public QWidget
//{
//	Q_OBJECT
//
//public:
//	AlarmInfoTab(QWidget *parent = 0);
//
//	void prepend(const QString &info);
//	void append(const QString &info);
//protected:
//	//void resizeEvent(QResizeEvent *event) override;
//
//private:
//	//void setChildrenGeometry(int w = 0, int h = 0);
//
//private:
//	QTextEdit *m_alarm;
//
//	const int m_maxCount;
//};

// 屏幕设置
class ScreenSettingTab : public QWidget
{
	Q_OBJECT

public:
	ScreenSettingTab(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	void setChildrenGeometry(int w = 0, int h = 0);

private:
	QCheckBox *m_buzzer;
	QPushButton *m_confirm;

	const int m_lblWidth;
	const int m_lblHeight;
};

// 出厂设置
class FactorySettingTab : public QWidget
{
	Q_OBJECT

public:
	FactorySettingTab(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	void setChildrenGeometry(int w = 0, int h = 0);

private:
	QLabel *m_devTypeLabel;
	QCheckBox *m_b2d;
	QCheckBox *m_b3;
	QPushButton *m_typeConfirm;
	QLabel *m_devIDLabel;
	QLineEdit *m_devID;
	QPushButton *m_idConfirm;
	QLabel *m_hwVersionLabel;
	QLineEdit *m_hwVersion;
	QPushButton *m_verConfirm;
	QLabel *m_card1Label;
	QLineEdit *m_networkCard1;
	QPushButton *m_card1Confirm;
	QLabel *m_card2Label;
	QLineEdit *m_networkCard2;
	QPushButton *m_card2Confirm;

	const int m_lblWidth;
	const int m_lblHeight;
};

// 密码修改
class PasswordChangeTab : public QWidget
{
	Q_OBJECT

public:
	PasswordChangeTab(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	void setChildrenGeometry(int w = 0, int h = 0);
	void connectSlots();

signals:
	void relogin();

private slots:
	void slotOnConfirmClicked();

private:
	QLabel *m_oldPwdLabel;
	QLineEdit *m_oldPwd;
	QLabel *m_newPwdLabel;
	QLineEdit *m_newPwd;
	QLabel *m_againPwdLabel;
	QLineEdit *m_againPwd;
	QPushButton *m_confirm;

	const int m_lblWidth;
	const int m_lblHeight;
};

// 还原设置
class RestoreTab : public QWidget
{
	Q_OBJECT

public:
	RestoreTab(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	void setChildrenGeometry(int w = 0, int h = 0);
	void connectSlots();
	void generateRandomValue();

private slots:
	void slotOnConfirmClicked();

private:
	QLabel *m_pwdLabel;	
	QLineEdit *m_pwd;
	QPushButton *m_confirm;
	QLabel *m_random;
	QString m_value;

	const int m_lblWidth;
	const int m_lblHeight;
};

// 版本信息
class VersionInfoTab : public QWidget
{
	Q_OBJECT

public:
	VersionInfoTab(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	void setChildrenGeometry(int w = 0, int h = 0);

private:
	QLabel *m_softVerLabel;
	QLabel *m_softVer;
	QLabel *m_firmVerLabel;
	QLabel *m_firmVer;
	QLabel *m_hardVerLabel;
	QLabel *m_hardVer;
	QLabel *m_devSNLabel;
	QLabel *m_devSN;

	const int m_lblWidth;
	const int m_lblHeight;
};

// 状态参数
class StateMainTab : public QWidget
{
	Q_OBJECT

public:
	StateMainTab(QWidget *parent = 0);

signals:
	void stateParamsButtonClicked(int id);

private:
	QPushButton *m_devOverview;
	QPushButton *m_bdsStatus;
	QPushButton *m_gpsStatus;
	QPushButton *m_gloStatus;
	QPushButton *m_acbStatus;
	QPushButton *m_dcbStatus;	

	QPushButton *m_posConfiguration;
	QPushButton *m_sportConfiguration;
	QPushButton *m_minElevation;
	QPushButton *m_minSnr;
	QPushButton *m_delayCompensation;
	QPushButton *m_pulseConfiguration;
	QPushButton *m_timezone;	

	QPushButton *m_logInfo;
	QPushButton *m_alarmInfo;
	QPushButton *m_screenSettings;
	QPushButton *m_factorySettings;
	QPushButton *m_pwdChanging;
	QPushButton *m_restoreSettings;
	QPushButton *m_verInfo;

	QButtonGroup *m_btnGroup;
};