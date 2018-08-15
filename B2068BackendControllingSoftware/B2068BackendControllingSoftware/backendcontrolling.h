#pragma once

#include <QtWidgets/QDialog>
#include "ui_backendcontrolling.h"

class BackendControlling : public QMainWindow
{
	Q_OBJECT

public:
	BackendControlling(QWidget *parent = Q_NULLPTR);

private:
	Ui::BackendControllingClass ui;
};
