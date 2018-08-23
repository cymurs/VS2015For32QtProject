#pragma once

#include <QWidget>

class CRadar;

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

// 北斗状态
class BDSStateTab : public QWidget
{
	Q_OBJECT

public:
	BDSStateTab(QWidget *parent = 0);

private:
	QLabel *m_workState;
	QLabel *m_signState;
	QLabel *m_pdop;
	QLabel *m_visSatellites;
	QLabel *m_avlSatellites;
	QTableView *m_bdsTable;
	CRadar *m_radar;
};