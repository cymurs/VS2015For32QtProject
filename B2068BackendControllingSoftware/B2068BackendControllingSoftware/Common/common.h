#pragma once
#include <QString>
#include <QMap>
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

/****************************************************************************************/
extern const char *ConfigFile;

// 用户名和密码
struct st_UserPwd
{
	QMap<QString, QString> user2pwd;
	QString curUser;
};
extern st_UserPwd userPwd;

// 帧数据
struct st_Gnsssta
{
	char satelliteType[4] = { '\0' };	// 卫星类型
	int visSatellites{ 0 };					// 可见卫星数
	int avlSatellites{ 0 };					// 可用卫星数
	int d3PosFlag{ 0 };					// 3D定位标志位
	float pdop{ 0.0 };						// PDOP值
	int rmcStateFlag{0};					// RMC状态标志位
	int snrFlag{0};							// 信噪比(Signal to noise ratio)标志位
};

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