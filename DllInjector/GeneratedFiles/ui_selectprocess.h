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
        SelectProcess->setStyleSheet(QStringLiteral("font: 10pt \"MS Shell Dlg 2\";"));
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
