#include "stdafx.h"
//#include "tabwidget.h"
#include "centralwidget.h"
#include "chiefwidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ChiefWidget w;
	w.show();
	return a.exec();
}
