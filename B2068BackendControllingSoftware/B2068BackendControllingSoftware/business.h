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

// °å¿¨µØÖ·
const unsigned char g_BoardAddr[COUNT_OF_GENERAL_ADDR][2] = {
	0x00,0x00,
	0x60,0x00,
	0x80,0x00,
	0x81,0x00,
	0x82,0x00,
	0x83,0x00,
	0x84,0x00,
	0xFF,0x00
};