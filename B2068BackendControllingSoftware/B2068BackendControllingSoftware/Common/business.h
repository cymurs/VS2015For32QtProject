#pragma once

const int COUNT_OF_GENERAL_ADDR = 8;

enum BoardAddrFlag {
	GeneralAddr = 0,
	PCAddr = 1,
	MasterControlAddr,
	Net1Addr,
	Net2Addr,
	ReceiverAddr,
	DisplayAddr,
	BroadcastAddr
};

// 板卡地址
const unsigned char g_BoardAddr[COUNT_OF_GENERAL_ADDR][2] = {
	0x00,0x00,   // 通用地址
	0x60,0x00,   // PC机
	0x80,0x00,   // 主控板
	0x81,0x00,   // 网络板1
	0x82,0x00,   // 网络板2
	0x83,0x00,   // 接收机板
	0x84,0x00,   // 显示板
	0xFF,0x00    // 广播地址
};