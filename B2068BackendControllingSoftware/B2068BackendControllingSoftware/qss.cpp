#include "qss.h"

/****************************************************************************************
QPushButton
*****************************************************************************************/
const QString QSS_PushButton =
"QPushButton {"
		"border-image: url(:/BackendControlling/images/button_normal.png) 5px;"
		"border-width: 5px;"
		"color: rgb(240, 240, 240);"
		"font: bold normal 12px \"arial\";"
		"min-width: 35px;"
	"}"
	"QPushButton:hover {"
		"border-image: url(:/BackendControlling/images/button_hover.png) 5px;"
	"}"
	"QPushButton:pressed {"
		"border-image: url(:/BackendControlling/images/button_pressed.png) 5px;"
		"padding-top: 2px;"
	"}"
	"QPushButton:disabled {"
		"border-image: url(:/BackendControlling/images/button_disable.png) 5px;"
		"color: rgb(200, 200, 200);"
	"}";

/****************************************************************************************
QMessageBox
*****************************************************************************************/
const QString QSS_MsgBox =
	"QMessageBox {"
		"background-image: url(:/BackendControlling/images/bg_signal.png);"
	"}";

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
		"min-width: 62.3px;" //20ex;"
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
通信界面
*****************************************************************************************/
const QString QSS_CommWidget =
	"QWidget#%1 {border-image: url(:/BackendControlling/images/timg.jpg);}"
	"QLabel, QPushButton, QGroupBox {"
		"font: bold normal 16px \"arial\";"
	"}"
	"QLineEdit, QComboBox  {"
		"font: bold normal 16px \"times\";"
	"}"
	"QLineEdit, QComboBox, QGroupBox  {"
		"color: #0306A8;"
	"}"
	"QLineEdit[echoMode=\"2\"] {"
		"lineedit-password-character: 42;"
	"}"
	"QComboBox{ "
		"padding-left: 35px;"
	"}"
	"QLabel[statusFlag=\"true\"] {"
		//"font: 16px \"arial\";"
		"color: 	#FF4040;"		
	"}";

/****************************************************************************************
登录界面
*****************************************************************************************/
const QString QSS_SignInWidget =
	"QWidget#%1 {border-image: url(:/BackendControlling/images/timg.jpg);}"
	"QLabel, QPushButton {"
		"font: bold normal 16px \"times\";"
	"}"
	"QLineEdit, QComboBox  {"
		"font: bold normal 16px \"times\";"
	"}"
	"QLineEdit, QComboBox  {"
		"color: #0306A8;"
	"}"
	"QLineEdit[echoMode=\"2\"] {"
		"lineedit-password-character: 42;"
	"}"
	"QComboBox{ "
		"padding-left: 35px;"
	"}";

/****************************************************************************************
左侧界面
*****************************************************************************************/
const QString QSS_LeftWidget = "QWidget#%1 { background-color: #424642; }";
const QString QSS_LeftWidgetDefaultButton =
	"QPushButton {"
		"font: bold normal 18px \"arial\";"
		"color: white;"
	"}"
	"QPushButton:flat {"
		"border: none;"
	"}"
	"QPushButton:hover {"
		"border-image: url(:/BackendControlling/images/left_btn.png);"
	"}";
const QString QSS_LeftWidgetToggledButton = 
	"QPushButton:flat {"
		"border-image: url(:/BackendControlling/images/left_btn.png);"
	"}";

/****************************************************************************************
主界面
*****************************************************************************************/
const QString QSS_MainTabBackground = "QWidget#%1 {border-image: url(:/BackendControlling/images/timg (1).png); }";
// 字体为"微软雅黑"
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
// 字体为Times New Roman
const QString QSS_TimesMainTabLabel =
	"QLabel#%1 {"
		"color: white;"
		"font: bold normal 16px \"arial\";"
	"}"

	"QLabel#%2 {"
		"color: white;"
		"font: bold normal 20px \"arial\";"
	"}"

	"QLabel#%3 {"
		"color: white;"
		"font: bold normal 104px \"times\";"
	"}";

/****************************************************************************************
时间源
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
		"font: bold normal 20px \"arial\";"
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
		"font: bold normal 16px \"arial\";"		
	"}"
	"QRadioButton#%2::indicator {"
		"width: 16px;"
		"height: 16px;"
	"}"
	"QRadioButton#%2::indicator::unchecked:hover {"
		"background: grey;"
		"border-radius: 8px;"
	"}";
	
const QString QSS_TimeSrcOthers =
	"*[autoField=\"true\"] { color: #4F004F; }"
	"*[manualField=\"true\"] { color: #0306A8; }"
	"QLabel#%1, QLineEdit {"
		"font: bold normal 16px \"arial\";"
		"color: #0306A8;"
	"}"
	"QLabel#%2, QPushButton, QComboBox {"
		"font: bold normal 16px \"arial\";"
		
	"}"
	// 字体居中折中办法
	"QComboBox {"
		"padding-left: 20px;"
		"max-width: 40px;"
	//"text-align: center;"
	"}"
	"QPushButton {"
		"max-width: 55px;"
	"}";


/****************************************************************************************
串口设置
*****************************************************************************************/
const QString QSS_ComSettingsLabel =
	"QLabel, QPushButton {"
		"font: bold normal 16px \"arial\";"
	"}"
	"QComboBox {"
		"font: bold normal 16px \"arial\";"
		"color: #0306A8;"
		"padding-left: 15px;"
	"}";

/****************************************************************************************
网络设置
*****************************************************************************************/
const QString QSS_NetSettings =
	"QRadioButton, QLabel {"
		"font: bold normal 20px \"arial\";"
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
		"font: bold normal 16px \"arial\";"
		"min-width: %1px;"
		"min-height: %2px;"
	"}"
	"QLineEdit {"
		"font: bold normal 16px \"arial\";"
		"color: #0306A8;"
	"}"
	"QGroupBox[netField=\"true\"] {"
		"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
																"stop: 0 #2CBB2C, stop: 1 #00CC00); "
		"border: 0px;"
	"}"
	"QGroupBox[commField=\"true\"] {"
	/*"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
															"stop: 0 #FFE500, stop: 1 #E4E700); "*/
		"background-color: #E4E700;"
		"border: 0px;"
	"}";

/****************************************************************************************
状态参数
*****************************************************************************************/
const QString QSS_StateParams =
	"QTabWidget::tab-bar {"
		"left: 4px;" /* move to the right by 5px */		
		"top: 0.5em;"
	"}"
	"QTabWidget::pane {"
		"margin-top: 0.5em;" 
	"}"
	"QLabel, QTableWidget, QTableView, QHeaderView, QPushButton, QCheckBox {"
		"font: bold normal 16px \"arial\";"
	"}"
	"QComboBox, QLineEdit, QTextEdit  {"
		//"font: 16px \"arial\";"
		"font: bold normal 16px \"arial\";"
	"}"	
	"QLabel#%1, QTableView#%2, QComboBox, QLineEdit, QTextEdit  {"
		"color: #0306A8;"
	"}"
	"QLabel#%3 {"
		"font: 16px \"arial\";"
		"color: lightgray;"
	"}"
	"QLineEdit[echoMode=\"2\"] {"
		"lineedit-password-character: 42;"
	"}"
	"QCheckBox::indicator {"
		"width: 20px;"
		"height: 20px;"
	"}"
	"QCheckBox::indicator:checked {"
		"image: url(:/BackendControlling/images/selected.png);"
	"}"
	"QCheckBox::indicator:unchecked {"
		"image: url(:/BackendControlling/images/unselected.png);"
	"}"
	"QCheckBox::indicator::unchecked:hover {"
		"background: grey;"
		"border-radius: 10px;"
	"}"
	"QComboBox[letterField=\"true\"] {"
		"padding-left: 12px;"
	//"max-width: 40px;"	
	"}"
	"QComboBox[numField=\"true\"] {"
		"padding-left: 18px;"	
	"}";

const QString QSS_StateMain =
	"QPushButton {"
		"font: bold normal 16px \"arial\";"
		"min-width: %1px;"
		"min-height: %2px;"
	"}";

/****************************************************************************************
设备总览
*****************************************************************************************/
const QString QSS_DeviceOverview =
	"QLabel, QTableWidget, QHeaderView {"
		"font: bold normal 16px \"arial\";"
	"}"
	"QLabel#%1  {"
		"color: #0306A8;"
	"}";
