#pragma once

#include <QObject>

class TimePositionDatabase : public QObject
{
	Q_OBJECT

public:
	TimePositionDatabase(QObject *parent = nullptr);
	~TimePositionDatabase();

	void selectFromMasterBoard(unsigned char chCmd, const QString &strData, bool bResend = false);
	void selectFromNetBoard(unsigned char chCmd, const QString &strData, BoardAddrFlag targetAddr, bool bResend = false);
	void selectFromReceiverBoard(unsigned char chCmd, const QString &strData, bool bResend = false);
	void selectFromDisplayBoard(unsigned char chCmd, const QString &strData, bool bResend = false);
};
