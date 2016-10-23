#ifndef DLLINJECTOR_H
#define DLLINJECTOR_H

#include <QtWidgets/QMainWindow>
#include "ui_dllinjector.h"

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
