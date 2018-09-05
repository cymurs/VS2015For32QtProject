#pragma once

#include <QWidget>

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

private:
	void connectSlots();
	void loadConfig();
	void storeConfig();

private slots:
	void slotOnSignIn(bool signin);
	void slotOnLoginTimeout();

private:
	SignInWidget *m_signIn;
	MainTab *m_main;
	CentralWidget *m_center;
	QStackedLayout *m_baseLayout;

	bool m_firstTime;
};
