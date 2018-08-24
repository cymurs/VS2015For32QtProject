#pragma once
#include <QString>
// ���������ʾ��������
#pragma execution_character_set("utf-8")  

/****************************************************************************************
qss�����ַ���
*****************************************************************************************/
extern const QString valueLabelQss;
extern const QString valueTableViewQss;

/****************************************************************************************/
const int MaxAvlSatellites = 10;

// ֡����
struct st_Gnsssta
{
	char satelliteType[4] = { '\0' };	// ��������
	int visSatellites{ 0 };					// �ɼ�������
	int avlSatellites{ 0 };					// ����������
	int d3PosFlag{ 0 };					// 3D��λ��־λ
	float pdop{ 0.0 };						// PDOPֵ
	int rmcStateFlag{0};					// RMC״̬��־λ
	int snrFlag{0};							// �����(Signal to noise ratio)��־λ
};

struct st_Gnssgsv
{
	char satelliteType[4] = { '\0' };	// ��������
	int frameTotal{0};				// ������֡��
	int frameNo{0};				// ��ǰ֡��
	int satellitesTotal{ 0 };		// �����ܿ���
	int satelliteNo[MaxAvlSatellites] = { 0 };			// ���Ǻ�
	int elevation[MaxAvlSatellites] = { 0 };			// ����
	int azimuth[MaxAvlSatellites] = { 0 };				// ��λ��
	int snr[MaxAvlSatellites] = { 0 };						// �����
	int count{0};														// �������Ǹ���
};