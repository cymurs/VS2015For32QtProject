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
extern const char SepCharSpace; // �ո�: ' '
extern const char SepCharComma; // ����: ','
extern const char *RefSource[];     // �ο�Դ
extern const char *RefDTName[]; //  �ο�Դʱ������
extern QMap<QString, QString> TamingState;  // ѱ��״̬

// �û���������
struct st_UserPwd
{
	QMap<QString, QString> user2pwd;
	QString curUser;
};
extern st_UserPwd userPwd;

/****************************************************************************************
����֡����
*****************************************************************************************/
struct st_Status
{
	QString lockState;
	QString oscType;
	int oscLock;
	int curRef;
	int irig_b;
	int day;
	int hour;
	int minute;
	int second;
};

struct st_Gnsstime
{
	QString receiverTime;
	QString UTCTime;
	QString acbTime;
	QString dcbTime;
	QString inputTime;
};

struct st_RefAvailInfo
{
	bool bdsuse{ false };
	bool gpsuse{ false };
	bool glouse{ false };
	bool acbuse{ false };
	bool dcbuse{ false };
};

/****************************************************************************************
���ջ���֡����
*****************************************************************************************/
struct st_Gnsssta
{
	char satelliteType[4] = { '\0' };	// ��������
	int visSatellites{ 0 };					// �ɼ�������
	int avlSatellites{ 0 };					// ����������
	int d3PosFlag{ 0 };					// 3D��λ��־λ
	float pdop{ 0.0 };						// PDOPֵ
	int rmcStateFlag{0};					// RMC״̬��־λ
	int snrFlag{0};							// �����(Signal to noise ratio)��־λ
	QString longitude;					// ������ e:���� w:����
	float lngValue{ 0.0 };				// ����ֵ
	QString latitude;						// �ϱ�γ s:��γ n:��γ
	float latValue{ 0.0 };				// γ��ֵ
	float elevation{ 0.0 };				// �߳�
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

/****************************************************************************************
����֡����
*****************************************************************************************/