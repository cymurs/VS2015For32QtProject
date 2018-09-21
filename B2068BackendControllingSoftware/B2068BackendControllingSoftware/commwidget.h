#pragma once

#include <QWidget>

class CommWidget : public QWidget
{
	Q_OBJECT

public:
	CommWidget(QWidget *parent = nullptr);
	~CommWidget();

	bool ConnectUseCom() const { return m_bUseCom;  }

protected:
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;   // 使得设置背景生效 的关键操作

	void setTextAlignCenter(QComboBox *comboBox);

private:
	QGroupBox *createComInfo();
	QGroupBox *createNetInfo();
	void connectSlots();
	void setValidator();
	void createWidgets();
	void checkConnect();

signals:
	void connectSuccess();

private slots:
	void slotOnSelectCom();
	void slotOnSelectNet();
	void slotOnConnectCom();
	void slotOnConnectNet();
	void slotOnSendResult(const QString &ret);
	void slotOnSendTimeOut(int excp);
	void slotOnCheckTimeOut();

private:
	QGroupBox *m_comGroup;
	QLabel *m_comNameLabel;
	QComboBox *m_comName;
	QLabel *m_comBaudLabel;
	QComboBox *m_comBaud;
	QPushButton *m_connCom;

	QGroupBox *m_netGroup;
	QLabel *m_addrLabel;
	QLineEdit *m_addr;
	QLabel *m_recvPortLabel;
	QLineEdit *m_recvPort;
	//QLabel *m_sendPortLabel;
	//QLineEdit *m_sendPort;
	QPushButton *m_connNet;

	QLabel *m_statusLabel;

	bool m_bUseCom;
	bool m_bConnectFailed;
	bool m_bHasResult;
	bool m_bNetUsable;
};
