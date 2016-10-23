/********************************************************************************
** Form generated from reading UI file 'dllinjector.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLLINJECTOR_H
#define UI_DLLINJECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DllInjectorClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DllInjectorClass)
    {
        if (DllInjectorClass->objectName().isEmpty())
            DllInjectorClass->setObjectName(QStringLiteral("DllInjectorClass"));
        DllInjectorClass->resize(600, 400);
        QIcon icon;
        icon.addFile(QStringLiteral("icons/Malware-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        DllInjectorClass->setWindowIcon(icon);
        centralWidget = new QWidget(DllInjectorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        DllInjectorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DllInjectorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        DllInjectorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(DllInjectorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DllInjectorClass->setStatusBar(statusBar);

        retranslateUi(DllInjectorClass);

        QMetaObject::connectSlotsByName(DllInjectorClass);
    } // setupUi

    void retranslateUi(QMainWindow *DllInjectorClass)
    {
        DllInjectorClass->setWindowTitle(QApplication::translate("DllInjectorClass", "DllInjector", 0));
    } // retranslateUi

};

namespace Ui {
    class DllInjectorClass: public Ui_DllInjectorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLLINJECTOR_H
