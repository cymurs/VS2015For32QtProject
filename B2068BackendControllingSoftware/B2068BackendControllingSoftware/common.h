#pragma once
#include <QString>
// 解决界面显示中文乱码
#pragma execution_character_set("utf-8")  

/****************************************************************************************
qss辅助字符串
*****************************************************************************************/
extern const QString valueLabelQss;
extern const QString valueTableViewQss;

/****************************************************************************************/
const int MaxAvlSatellites = 10;

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