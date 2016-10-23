#ifndef DLLINJECTOR_H
#define DLLINJECTOR_H

#include "ui_dllinjector.h"
#include <QtWidgets/QMainWindow>

class DllInjector : public QMainWindow
{
	Q_OBJECT

public:
	DllInjector(QWidget *parent = 0);
	~DllInjector();

private:
	Ui::DllInjectorClass ui;
};

#endif // DLLINJECTOR_H
