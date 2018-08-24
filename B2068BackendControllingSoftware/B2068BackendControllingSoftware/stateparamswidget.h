#pragma once

#include <QWidget>

class CRadar;
extern const int MaxAvlSatellites;

// ���Ǻͷ�λ�ǽṹ��
struct ElevationAzimuthParams {
	double elevations[MaxAvlSatellites] = {0};
	double azimuths[MaxAvlSatellites] = {0};
	int count{0};
};

// �豸����
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

// ����״̬��GPS״̬��GLONASS״̬
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