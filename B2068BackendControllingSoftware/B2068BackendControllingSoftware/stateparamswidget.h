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

// 
class GPSStateTab : public QWidget
{
	Q_OBJECT

public:
	GPSStateTab(QWidget *parent = 0);

private:
	QLabel *m_workState;
	QLabel *m_signState;
	QLabel *m_pdop;
	QLabel *m_visSatellites;
	QLabel *m_avlSatellites;
	QTableView *m_bdsTable;
	CRadar *m_radar;
	ElevationAzimuthParams *m_elevAzimParams;
};

// 
class GLOStateTab : public QWidget
{
	Q_OBJECT

public:
	GLOStateTab(QWidget *parent = 0);

private:
	QLabel *m_workState;
	QLabel *m_signState;
	QLabel *m_pdop;
	QLabel *m_visSatellites;
	QLabel *m_avlSatellites;
	QTableView *m_bdsTable;
	CRadar *m_radar;
	ElevationAzimuthParams *m_elevAzimParams;
};