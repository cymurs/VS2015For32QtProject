#include "qss.h"

/****************************************************************************************
TabWidget Style Sheet
*****************************************************************************************/
const QString QSS_TabWidget = 
	"QTabWidget::pane { "/* The tab widget frame */
		"border-top: 2px solid #C2C7CB;"
		"position: absolute;"
		"top: -0.5em;"
	"}"

	"QTabWidget::tab-bar{"
		"alignment: center;"
	"}"

	"QTabBar::tab{"
		"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
														"stop : 0 #E1E1E1, stop: 0.4 #DDDDDD,"
														"stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);"
		"border: 2px solid #C4C4C3;"
		"border-bottom-color: #C2C7CB;" /* same as the pane color */
		"border-top-left-radius: 4px;"
		"border-top-right-radius: 4px;"
		"min-width: 20ex;"
		"padding: 2px;"
	"}"

	"QTabBar::tab:selected, QTabBar::tab:hover{"
		"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
														"stop : 0 #fafafa, stop: 0.4 #f4f4f4,"
														"stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);"
	"}"

	"QTabBar::tab:selected{"
		"border-color: #9B9B9B;"
		"border-bottom-color: #C2C7CB;" /* same as pane color */
	"}";

/****************************************************************************************
������
*****************************************************************************************/
const QString QSS_MainTabBackground = "QWidget#%1 {border-image: url(:/BackendControlling/images/timg (1).png); }";
// ����Ϊ"΢���ź�"
const QString QSS_YaHeiMainTabLabel =
	"QLabel#%1 {"
		"color: white;"
		"font: bold normal 16px \"Microsoft YaHei\";"
	"}"

	"QLabel#%2 {"
		"color: white;"
		"font: bold normal 20px \"Microsoft YaHei\";"
	"}"

	"QLabel#%3 {"
		"color: white;"
		"font: bold normal 104px \"times\";"
	"}";
// ����ΪTimes New Roman
const QString QSS_TimesMainTabLabel =
	"QLabel#%1 {"
		"color: white;"
		"font: bold normal 16px \"times\";"
	"}"

	"QLabel#%2 {"
		"color: white;"
		"font: bold normal 20px \"times\";"
	"}"

	"QLabel#%3 {"
		"color: white;"
		"font: bold normal 104px \"times\";"
	"}";

/****************************************************************************************
ʱ��Դ
*****************************************************************************************/
const QString QSS_YaHeiTimeSrcRadio =
	"QRadioButton#%1 {"
		"font: bold normal 20px \"Microsoft YaHei\";"
	"}"
	"QRadioButton#%1::indicator {"
		"width: 20px;"
		"height: 20px;"
	"}"
	"QRadioButton::indicator::checked {"
		"image: url(:/BackendControlling/images/full.png);"
	"}"
	"QRadioButton::indicator::unchecked {"
		"image: url(:/BackendControlling/images/empty.png);"
	"}"
	"QRadioButton#%1::indicator::unchecked:hover {"
		"background: grey;"
		"border-radius: 10px;"
	"}"

	"QRadioButton#%2 {"
		"font: bold normal 15px \"Microsoft YaHei\";"
	"}"
	"QRadioButton#%2::indicator {"
		"width: 16px;"
		"height: 16px;"
	"}"
	"QRadioButton#%2::indicator::unchecked:hover {"
		"background: grey;"
		"border-radius: 8px;"
	"}";

const QString QSS_TimesTimeSrcRadio =
	"QRadioButton#%1 {"
		"font: bold normal 20px \"times\";"
	"}"
	"QRadioButton#%1::indicator {"
		"width: 20px;"
		"height: 20px;"
	"}"
	"QRadioButton::indicator::checked {"
		"image: url(:/BackendControlling/images/full.png);"
	"}"
	"QRadioButton::indicator::unchecked {"
		"image: url(:/BackendControlling/images/empty.png);"
	"}"
	"QRadioButton#%1::indicator::unchecked:hover {"
		"background: grey;"
		"border-radius: 10px;"
	"}"

	"QRadioButton#%2 {"
		"font: bold normal 15px \"times\";"
	"}"
	"QRadioButton#%2::indicator {"
		"width: 16px;"
		"height: 16px;"
	"}"
	"QRadioButton#%2::indicator::unchecked:hover {"
		"background: grey;"
		"border-radius: 8px;"
	"}";

const QString QSS_TimeSrcLabel =
	"QLabel#%1, QLineEdit, QComboBox {"
		"font: 16px \"Microsoft YaHei\";"
	"}"
	"QLabel#%2, QPushButton {"
		"font: bold normal 15px \"Microsoft YaHei\";"
	"}";

/****************************************************************************************
��������
*****************************************************************************************/
const QString QSS_ComSettingsLabel =
	"QLabel, QPushButton {"
		"font: bold normal 16px \"Microsoft YaHei\";"
	"}"
	"QComboBox {"
		"font: 16px \"Microsoft YaHei\";"
		"color: #0306A8;"
	"}";

/****************************************************************************************
��������
*****************************************************************************************/
const QString QSS_NetSettings =
	"QRadioButton, QLabel {"
		"font: bold normal 20px \"Microsoft YaHei\";"
	"}"
	"QRadioButton::indicator {"
		"width: 20px;"
		"height: 20px;"
	"}"
	"QRadioButton::indicator::checked {"
		"image: url(:/BackendControlling/images/full.png);"
	"}"
	"QRadioButton::indicator::unchecked {"
		"image: url(:/BackendControlling/images/empty.png);"
	"}"
	"QRadioButton::indicator::unchecked:hover {"
		"background: grey;"
		"border-radius: 10px;"
	"}"
	"QPushButton {"
		"font: bold normal 16px \"Microsoft YaHei\";"
	"}"
	"QLineEdit {"
		"font: 16px \"Microsoft YaHei\";"
		"color: #0306A8;"
	"}";

/****************************************************************************************
״̬����
*****************************************************************************************/
const QString QSS_StateParams =
	"QTabWidget::tab-bar {"
		"left: 5px;" /* move to the right by 5px */
		"top: 0.5em;"
	"}"
	"QTabWidget::pane {"
		"margin-top: 0.5em;" 
	"}"
	"QLabel, QTableWidget, QTableView, QHeaderView {"
		"font: bold normal 16px \"times\";"
	"}"
	"QLabel#%1, QTableView#%2  {"
		"color: #0306A8;"
	"}";

/****************************************************************************************
�豸����
*****************************************************************************************/
const QString QSS_DeviceOverview =
	"QLabel, QTableWidget, QHeaderView {"
		"font: bold normal 16px \"times\";"
	"}"
	"QLabel#%1  {"
		"color: #0306A8;"
	"}";

/****************************************************************************************
����״̬
*****************************************************************************************/
const QString QSS_BDSState =
"";