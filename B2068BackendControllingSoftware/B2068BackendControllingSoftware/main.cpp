#include "stdafx.h"
#include "backendcontrolling.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BackendControlling w;
	w.show();
	return a.exec();
}
