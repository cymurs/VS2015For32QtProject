#pragma once

#include <QWidget>

class CommWidget;
class SignInWidget;
class MainTab;
class CentralWidget;

class ChiefWidget : public QWidget
{
	Q_OBJECT

public:
	ChiefWidget(QWidget *parent = Q_NULLPTR);
	~ChiefWidget();

protected:
	void	closeEvent(QCloseEvent *event) override;
	bool isQuit();

private:
	void connectSlots();
	void queryMasterBoardInfo();
	void queryNetBoardInfo();
	void queryDisplayBoardInfo();
	void queryReceiverBoardInfo();
	void queryBoardInfo();	
	void loadConfig();
	void storeConfig();

private slots:
	void slotOnConnect();
	void slotOnSignIn(bool signin);
	void slotOnLoginTimeout();
	void slotOnMainFadeOut();
	void slotOnAnimationFinished();
	void slotOnGoHome();
	void slotOnChangeParams();

private:
	CommWidget *m_comm;
	SignInWidget *m_signIn;
	MainTab *m_main;
	CentralWidget *m_center;
	QStackedLayout *m_baseLayout;
	QPropertyAnimation *m_animation;

	bool m_firstTime;
	bool m_loginChange;  // 更改参数 确认
};
