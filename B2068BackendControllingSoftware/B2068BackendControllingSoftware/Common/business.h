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

// �忨��ַ
const unsigned char g_BoardAddr[COUNT_OF_GENERAL_ADDR][2] = {
	0x00,0x00,   // ͨ�õ�ַ
	0x60,0x00,   // PC��
	0x80,0x00,   // ���ذ�
	0x81,0x00,   // �����1
	0x82,0x00,   // �����2
	0x83,0x00,   // ���ջ���
	0x84,0x00,   // ��ʾ��
	0xFF,0x00    // �㲥��ַ
};