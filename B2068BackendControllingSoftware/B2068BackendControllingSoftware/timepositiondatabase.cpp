#include "stdafx.h"
#include "timepositiondatabase.h"
#include "transportthread.h"

TimePositionDatabase::TimePositionDatabase(QObject *parent)
	: QObject(parent)
{
}

TimePositionDatabase::~TimePositionDatabase()
{
}

void TimePositionDatabase::selectFromMasterBoard(unsigned char chCmd, const QString &strData, bool bResend /*= false*/)
{
	TransportThread *pTransport = TransportThread::Get();
	pTransport->SetFrameDataFormat(false, false, true);
	pTransport->SetSourceAddr(g_BoardAddr[PCAddr][0], g_BoardAddr[PCAddr][1], 0x00);
	pTransport->SetTargetAddr(g_BoardAddr[MasterControlAddr][0], g_BoardAddr[MasterControlAddr][1], 0x00);
	pTransport->SetCommand(chCmd);
	pTransport->SendComNetData(chCmd, strData.toLatin1().data(), strData.size(), bResend);
}

void TimePositionDatabase::selectFromNetBoard(unsigned char chCmd, const QString &strData, BoardAddrFlag targetAddr, bool bResend /*= false*/)
{
	TransportThread *pTransport = TransportThread::Get();
	pTransport->SetFrameDataFormat(false, false, true);
	pTransport->SetSourceAddr(g_BoardAddr[PCAddr][0], g_BoardAddr[PCAddr][1], 0x00);
	pTransport->SetTargetAddr(g_BoardAddr[targetAddr][0], g_BoardAddr[targetAddr][1], 0x00);
	pTransport->SetCommand(chCmd);
	pTransport->SendComNetData(chCmd, strData.toLatin1().data(), strData.size(), bResend);
}

void TimePositionDatabase::selectFromReceiverBoard(unsigned char chCmd, const QString &strData, bool bResend /*= false*/)
{
	TransportThread *pTransport = TransportThread::Get();
	pTransport->SetFrameDataFormat(false, false, true);
	pTransport->SetSourceAddr(g_BoardAddr[PCAddr][0], g_BoardAddr[PCAddr][1], 0x00);
	pTransport->SetTargetAddr(g_BoardAddr[ReceiverAddr][0], g_BoardAddr[ReceiverAddr][1], 0x00);
	pTransport->SetCommand(chCmd);
	pTransport->SendComNetData(chCmd, strData.toLatin1().data(), strData.size(), bResend);
}

void TimePositionDatabase::selectFromDisplayBoard(unsigned char chCmd, const QString &strData, bool bResend /*= false*/)
{
	TransportThread *pTransport = TransportThread::Get();
	pTransport->SetFrameDataFormat(false, false, true);
	pTransport->SetSourceAddr(g_BoardAddr[PCAddr][0], g_BoardAddr[PCAddr][1], 0x00);
	pTransport->SetTargetAddr(g_BoardAddr[DisplayAddr][0], g_BoardAddr[DisplayAddr][1], 0x00);
	pTransport->SetCommand(chCmd);
	pTransport->SendComNetData(chCmd, strData.toLatin1().data(), strData.size(), bResend);
}
