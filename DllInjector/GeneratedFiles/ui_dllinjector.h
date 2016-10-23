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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DllInjectorClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QRadioButton *injectOperation;
    QRadioButton *freeOperation;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *createRemoteThreadRadioButton;
    QRadioButton *setWindowsHookExRadioButton;
    QRadioButton *windowsRegistryRadioButton;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *dllNameLineEdit;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *selectDllButton;
    QHBoxLayout *horizontalLayout;
    QLineEdit *processNameLineEdit;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *selectProcessButton;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *runButton;
    QSpacerItem *horizontalSpacer_4;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *DllInjectorClass)
    {
        if (DllInjectorClass->objectName().isEmpty())
            DllInjectorClass->setObjectName(QStringLiteral("DllInjectorClass"));
        DllInjectorClass->resize(443, 514);
        DllInjectorClass->setMinimumSize(QSize(443, 514));
        DllInjectorClass->setMaximumSize(QSize(443, 514));
        QFont font;
        font.setPointSize(10);
        DllInjectorClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral("icons/Malware-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        DllInjectorClass->setWindowIcon(icon);
        DllInjectorClass->setStyleSheet(QStringLiteral(""));
        centralWidget = new QWidget(DllInjectorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        injectOperation = new QRadioButton(groupBox_2);
        injectOperation->setObjectName(QStringLiteral("injectOperation"));
        injectOperation->setChecked(true);

        verticalLayout->addWidget(injectOperation);

        freeOperation = new QRadioButton(groupBox_2);
        freeOperation->setObjectName(QStringLiteral("freeOperation"));

        verticalLayout->addWidget(freeOperation);


        verticalLayout_4->addWidget(groupBox_2);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QStringLiteral(""));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        createRemoteThreadRadioButton = new QRadioButton(groupBox);
        createRemoteThreadRadioButton->setObjectName(QStringLiteral("createRemoteThreadRadioButton"));
        createRemoteThreadRadioButton->setChecked(true);

        verticalLayout_2->addWidget(createRemoteThreadRadioButton);

        setWindowsHookExRadioButton = new QRadioButton(groupBox);
        setWindowsHookExRadioButton->setObjectName(QStringLiteral("setWindowsHookExRadioButton"));

        verticalLayout_2->addWidget(setWindowsHookExRadioButton);

        windowsRegistryRadioButton = new QRadioButton(groupBox);
        windowsRegistryRadioButton->setObjectName(QStringLiteral("windowsRegistryRadioButton"));

        verticalLayout_2->addWidget(windowsRegistryRadioButton);


        verticalLayout_4->addWidget(groupBox);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        dllNameLineEdit = new QLineEdit(groupBox_3);
        dllNameLineEdit->setObjectName(QStringLiteral("dllNameLineEdit"));
        dllNameLineEdit->setReadOnly(true);

        horizontalLayout_2->addWidget(dllNameLineEdit);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        selectDllButton = new QPushButton(groupBox_3);
        selectDllButton->setObjectName(QStringLiteral("selectDllButton"));
        selectDllButton->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_2->addWidget(selectDllButton);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        processNameLineEdit = new QLineEdit(groupBox_3);
        processNameLineEdit->setObjectName(QStringLiteral("processNameLineEdit"));
        processNameLineEdit->setReadOnly(true);

        horizontalLayout->addWidget(processNameLineEdit);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        selectProcessButton = new QPushButton(groupBox_3);
        selectProcessButton->setObjectName(QStringLiteral("selectProcessButton"));
        selectProcessButton->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout->addWidget(selectProcessButton);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_4->addWidget(groupBox_3);

        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setReadOnly(true);

        verticalLayout_4->addWidget(textEdit);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        runButton = new QPushButton(centralWidget);
        runButton->setObjectName(QStringLiteral("runButton"));
        runButton->setCursor(QCursor(Qt::PointingHandCursor));
        runButton->setStyleSheet(QLatin1String("QPushButton: \n"
"{\n"
"cursor: Pointing Hand;\n"
"}"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("icons/Infect-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        runButton->setIcon(icon1);
        runButton->setIconSize(QSize(30, 30));

        horizontalLayout_3->addWidget(runButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout_4->addLayout(horizontalLayout_3);

        DllInjectorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DllInjectorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 443, 21));
        DllInjectorClass->setMenuBar(menuBar);

        retranslateUi(DllInjectorClass);

        QMetaObject::connectSlotsByName(DllInjectorClass);
    } // setupUi

    void retranslateUi(QMainWindow *DllInjectorClass)
    {
        DllInjectorClass->setWindowTitle(QApplication::translate("DllInjectorClass", "Dll Injector", 0));
        groupBox_2->setTitle(QApplication::translate("DllInjectorClass", "Select operation", 0));
        injectOperation->setText(QApplication::translate("DllInjectorClass", "Inject DLL", 0));
        freeOperation->setText(QApplication::translate("DllInjectorClass", "Free DLL", 0));
        groupBox->setTitle(QApplication::translate("DllInjectorClass", "Select the method of Injection", 0));
        createRemoteThreadRadioButton->setText(QApplication::translate("DllInjectorClass", "Inject using CreateRemoteThread", 0));
        setWindowsHookExRadioButton->setText(QApplication::translate("DllInjectorClass", "Inject using SetWindowsHookEx", 0));
        windowsRegistryRadioButton->setText(QApplication::translate("DllInjectorClass", "Inject using Windows registry", 0));
        groupBox_3->setTitle(QApplication::translate("DllInjectorClass", "Settings", 0));
        selectDllButton->setText(QApplication::translate("DllInjectorClass", "Select DLL", 0));
        selectProcessButton->setText(QApplication::translate("DllInjectorClass", "Select process", 0));
#ifndef QT_NO_TOOLTIP
        runButton->setToolTip(QApplication::translate("DllInjectorClass", "Start operation", 0));
#endif // QT_NO_TOOLTIP
        runButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DllInjectorClass: public Ui_DllInjectorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLLINJECTOR_H
