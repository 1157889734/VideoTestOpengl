/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QWidget *wnd1;
    QWidget *wnd2;
    QWidget *wnd3;
    QWidget *wnd4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(978, 515);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        wnd1 = new QWidget(centralWidget);
        wnd1->setObjectName(QString::fromUtf8("wnd1"));

        gridLayout->addWidget(wnd1, 0, 0, 1, 1);

        wnd2 = new QWidget(centralWidget);
        wnd2->setObjectName(QString::fromUtf8("wnd2"));

        gridLayout->addWidget(wnd2, 0, 1, 1, 1);

        wnd3 = new QWidget(centralWidget);
        wnd3->setObjectName(QString::fromUtf8("wnd3"));

        gridLayout->addWidget(wnd3, 1, 0, 1, 1);

        wnd4 = new QWidget(centralWidget);
        wnd4->setObjectName(QString::fromUtf8("wnd4"));

        gridLayout->addWidget(wnd4, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
