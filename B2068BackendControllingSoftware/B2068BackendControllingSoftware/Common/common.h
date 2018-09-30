#pragma once
#include <QString>
#include <QMap>
#include <QVariant>
// 解决界面显示中文乱码
#pragma execution_character_set("utf-8")  

/****************************************************************************************
子控件位置属性
*****************************************************************************************/
const int LeftMargin = 20;			// 左边距
const int TopMargin = 10;		// 顶边距
const int BottomMargin = 20;	// 底边距
const int LblWidth = 80;			//	标签宽度
const int LblHeight = 20;			// 标签高度
const int LeWidth = 240;        // 输入框宽度

/****************************************************************************************
qss辅助字符串
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
extern const char SepCharSpace; // 空格: ' '
extern const char SepCharComma; // 逗号: ','
extern const char *SepCrLf;
extern const char *RefSource[];     // 参考源
extern const char *RefDTName[]; //  参考源时间名称
extern QMap<QString, QString> TamingState;  // 驯服状态

// 连接信息  struct需以C的方式声明
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

// 用户名和密码
struct st_UserPwd
{
	QMap<QString, QString> user2pwd;
	QString curUser;
};
extern st_UserPwd userPwd;

// 参考源
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

// 优先级
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
主板帧数据
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
Q_DECLARE_METATYPE(st_Gnsstime)  // 记得在最前面加 #include <QVariant>

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
接收机板帧数据
*****************************************************************************************/
struct st_Gnsssta
{
	char satelliteType[4] = { '\0' };	// 卫星类型
	int visSatellites{ 0 };					// 可见卫星数
	int avlSatellites{ 0 };					// 可用卫星数
	int d3PosFlag{ 0 };					// 3D定位标志位
	float pdop{ 0.0 };						// PDOP值
	int rmcStateFlag{0};					// RMC状态标志位
	int snrFlag{0};							// 信噪比(Signal to noise ratio)标志位
	QString longitude;					// 东西经 e:东经 w:西经
	float lngValue{ 0.0 };				// 经度值
	QString latitude;						// 南北纬 s:南纬 n:北纬
	float latValue{ 0.0 };				// 纬度值
	float elevation{ 0.0 };				// 高程
};
Q_DECLARE_METATYPE(st_Gnsssta)

struct st_Gnssgsv
{
	char satelliteType[4] = { '\0' };	// 卫星类型
	int frameTotal{0};				// 数据总帧数
	int frameNo{0};				// 当前帧数
	int satellitesTotal{ 0 };		// 卫星总颗数
	int satelliteNo[MaxAvlSatellites] = { 0 };			// 卫星号
	int elevation[MaxAvlSatellites] = { 0 };			// 仰角
	int azimuth[MaxAvlSatellites] = { 0 };				// 方位角
	int snr[MaxAvlSatellites] = { 0 };						// 信噪比
	int count{0};														// 可用卫星个数
};
Q_DECLARE_METATYPE(st_Gnssgsv)

/****************************************************************************************
网络板帧数据
*****************************************************************************************/
struct st_NetInfo {
	QString ip;                    // 本机IP
	QString submask;		  // 本机掩码
	QString gateway;         // 本机网关
	ushort worklocal{ 0 };			  // 本机端口
	ushort worktarget{ 0 };		  // 目标端口
	QString remoteip;	      // 目标IP
	QString groupip;          // 组播本机IP
	ushort grouplocal{ 0 };         // 组播本机端口
	ushort grouptarget{ 0 };       // 组播目标端口
	QString remotegroupip;  // 组播目标IP
	QString broadcastip;        // 广播IP
	ushort broadcastport{ 0 };		  // 广播端口
	ushort debugrecv{ 0 };              // 监控接收端口
	ushort debugsend{ 0 };             // 监控发送端口
	int delay{ 0 };                           // 网络延时
	QString macaddr;             // 本机物理地址
};
Q_DECLARE_METATYPE(st_NetInfo)

/****************************************************************************************
显示帧数据
*****************************************************************************************/