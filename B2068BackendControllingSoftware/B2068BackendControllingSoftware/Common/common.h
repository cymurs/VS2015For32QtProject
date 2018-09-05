#pragma once
#include <QString>
#include <QMap>
// ���������ʾ��������
#pragma execution_character_set("utf-8")  

/****************************************************************************************
�ӿؼ�λ������
*****************************************************************************************/
const int LeftMargin = 20;			// ��߾�
const int TopMargin = 10;		// ���߾�
const int BottomMargin = 20;	// �ױ߾�
const int LblWidth = 80;			//	��ǩ���
const int LblHeight = 20;			// ��ǩ�߶�
const int LeWidth = 240;        // �������

/****************************************************************************************
qss�����ַ���
*****************************************************************************************/
const QString valueLabelQss("valuelabel");
const QString valueTableViewQss("valuetableview");
const QString valueLightgrayQss("valuelightgray");

/****************************************************************************************/
const int MaxAvlSatellites = 10;
const int LargeVer = 2;
const int SmallVer = 0;
const int LoginInterval = 5 * 60 * 1000;

/****************************************************************************************/
extern const char *ConfigFile;

// �û���������
struct st_UserPwd
{
	QMap<QString, QString> user2pwd;
	QString curUser;
};
extern st_UserPwd userPwd;

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