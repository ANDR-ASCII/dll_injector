/********************************************************************************
** Form generated from reading UI file 'selectprocess.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTPROCESS_H
#define UI_SELECTPROCESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectProcess
{
public:
    QVBoxLayout *verticalLayout;
    QTreeView *processView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *selectButton;

    void setupUi(QWidget *SelectProcess)
    {
        if (SelectProcess->objectName().isEmpty())
            SelectProcess->setObjectName(QStringLiteral("SelectProcess"));
        SelectProcess->setWindowModality(Qt::ApplicationModal);
        SelectProcess->resize(779, 563);
        SelectProcess->setCursor(QCursor(Qt::ArrowCursor));
        QIcon icon;
        icon.addFile(QStringLiteral("../icons/Malware-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        SelectProcess->setWindowIcon(icon);
        SelectProcess->setStyleSheet(QLatin1String("QTreeView {\n"
"    show-decoration-selected: 1;\n"
"    border-radius: 5px;\n"
"    padding: 1px;\n"
"    border: 1px solid rgb(170, 170, 170);\n"
"}\n"
"\n"
"QTreeView::item {\n"
"     border: 1px solid rgb(170, 170, 170);\n"
"     border-top-color: transparent;\n"
"     border-bottom-color: transparent;\n"
"}\n"
"\n"
"QTreeView::item:hover {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1);\n"
"    border: 1px solid #bfcde4;\n"
"}\n"
"\n"
"QTreeView::item:selected {\n"
"    border: 1px solid #567dbc;\n"
"}\n"
"\n"
"QTreeView::item:selected:active{\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6ea1f1, stop: 1 #567dbc);\n"
"}\n"
"\n"
"QTreeView::item:selected:!active {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6b9be8, stop: 1 #577fbf);\n"
"}\n"
"\n"
"QTreeView::branch {\n"
"        background: palette(base);\n"
"}\n"
"\n"
"QTreeView::branch:has-siblings:!adjoins-item {\n"
"        background: cyan;\n"
"}"
                        "\n"
"\n"
"QTreeView::branch:closed:has-children:has-siblings {\n"
"        background: pink;\n"
"}\n"
"\n"
"QTreeView::branch:has-children:!has-siblings:closed {\n"
"        background: gray;\n"
"}\n"
"\n"
"QTreeView::branch:open:has-children:has-siblings {\n"
"        background: magenta;\n"
"}\n"
"\n"
"QTreeView::branch:open:has-children:!has-siblings {\n"
"        background: green;\n"
"}\n"
"\n"
"QPushButton\n"
"{\n"
"background-color: rgb(225, 225, 225);\n"
"color: black;\n"
"border-radius: 5px;\n"
"border: 1px solid;\n"
"border-color: rgb(170, 170, 170);\n"
"padding: 7px;\n"
"}\n"
"\n"
"font: 10pt \"MS Shell Dlg 2\";"));
        verticalLayout = new QVBoxLayout(SelectProcess);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        processView = new QTreeView(SelectProcess);
        processView->setObjectName(QStringLiteral("processView"));

        verticalLayout->addWidget(processView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        selectButton = new QPushButton(SelectProcess);
        selectButton->setObjectName(QStringLiteral("selectButton"));
        selectButton->setCursor(QCursor(Qt::PointingHandCursor));
        selectButton->setStyleSheet(QStringLiteral(""));

        horizontalLayout->addWidget(selectButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SelectProcess);

        QMetaObject::connectSlotsByName(SelectProcess);
    } // setupUi

    void retranslateUi(QWidget *SelectProcess)
    {
        SelectProcess->setWindowTitle(QApplication::translate("SelectProcess", "Select Process", 0));
        selectButton->setText(QApplication::translate("SelectProcess", "Select", 0));
    } // retranslateUi

};

namespace Ui {
    class SelectProcess: public Ui_SelectProcess {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTPROCESS_H
