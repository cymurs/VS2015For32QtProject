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

private:
	QLabel *m_ppsWidthLabel;
	QLineEdit *m_ppsWidth;
	QComboBox *m_ppsWidthUnit;
	QLabel *m_ppsDelayLabel;
	QLineEdit *m_ppsDelay;
	QComboBox *m_ppsDelayUnit;
	QPushButton *m_confirm;
	QValidator *m_msValidator;
	QValidator *m_nsValidator;

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