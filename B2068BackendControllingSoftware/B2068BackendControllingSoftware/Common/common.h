#pragma once
#include <QString>
#include <QMap>
#include <QVariant>
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
const int ResendInterval = 1100;  // ms

/****************************************************************************************/
extern const char *ConfigFile;
extern const char SepCharSpace; // �ո�: ' '
extern const char SepCharComma; // ����: ','
extern const char *SepCrLf;
extern const char *RefSource[];     // �ο�Դ
extern const char *RefDTName[]; //  �ο�Դʱ������
extern QMap<QString, QString> TamingState;  // ѱ��״̬

// ������Ϣ  struct����C�ķ�ʽ����
union un_ConnectInfo
{
	struct { 
		char comName[64];
		int baud;
	}comConn;
	struct {
		char netIP[64];
		int netPort; 
	}netConn;
};
extern un_ConnectInfo connInfo;

// �û���������
struct st_UserPwd
{
	QMap<QString, QString> user2pwd;
	QString curUser;
};
extern st_UserPwd userPwd;

// �ο�Դ
enum RSFlag 
{
	NOTPOS = 0,
	GPS,
	GLONASS,
	BDS,
	BAC,
	BDC = 5,
	SETTIME = 8
};

// ���ȼ�
struct st_TimeSrcPriority {
	char model;
	char priorFirst;
	char priorSecond;
	char priorThird;
	char priorFourth;
	char priorFifth;
	char curRefSrc;
};

/****************************************************************************************
����֡����
*****************************************************************************************/
struct st_Status
{
	int lockState;
	QString oscType;
	int oscLock;
	int curRef;
	int irig_b;
	int day;
	int hour;
	int minute;
	int second;
};
Q_DECLARE_METATYPE(st_Status)

struct st_Gnsstime
{
	QString receiverTime;
	QString UTCTime;
	QString acbTime;
	QString dcbTime;
	QString inputTime;
};
Q_DECLARE_METATYPE(st_Gnsstime)  // �ǵ�����ǰ��� #include <QVariant>

struct st_RefAvailInfo
{
	bool gpsuse{ false };
	bool glouse{ false };
	bool bdsuse{ false };	
	bool acbuse{ false };
	bool dcbuse{ false };
};
Q_DECLARE_METATYPE(st_RefAvailInfo)

struct st_MasterVer
{
	float mainv{ 0.0 };
	float net1v{ 0.0 };
	float net2v{ 0.0 };
	float refv{ 0.0 };
	float viewv{ 0.0 };
	QString firmware;
	QString hardware;
};
Q_DECLARE_METATYPE(st_MasterVer)

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
Q_DECLARE_METATYPE(st_Gnsssta)

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
Q_DECLARE_METATYPE(st_Gnssgsv)

/****************************************************************************************
�����֡����
*****************************************************************************************/
struct st_NetInfo {
	QString ip;                    // ����IP
	QString submask;		  // ��������
	QString gateway;         // ��������
	ushort worklocal{ 0 };			  // �����˿�
	ushort worktarget{ 0 };		  // Ŀ��˿�
	QString remoteip;	      // Ŀ��IP
	QString groupip;          // �鲥����IP
	ushort grouplocal{ 0 };         // �鲥�����˿�
	ushort grouptarget{ 0 };       // �鲥Ŀ��˿�
	QString remotegroupip;  // �鲥Ŀ��IP
	QString broadcastip;        // �㲥IP
	ushort broadcastport{ 0 };		  // �㲥�˿�
	ushort debugrecv{ 0 };              // ��ؽ��ն˿�
	ushort debugsend{ 0 };             // ��ط��Ͷ˿�
	int delay{ 0 };                           // ������ʱ
	QString macaddr;             // ���������ַ
};
Q_DECLARE_METATYPE(st_NetInfo)

/****************************************************************************************
��ʾ֡����
*****************************************************************************************/