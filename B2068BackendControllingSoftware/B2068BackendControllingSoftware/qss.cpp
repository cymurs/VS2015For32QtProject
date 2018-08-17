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


