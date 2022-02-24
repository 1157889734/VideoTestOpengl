/********************************************************************************
** Form generated from reading UI file 'testform.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTFORM_H
#define UI_TESTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TestForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_play;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *widget_MonitorWin1;
    QLabel *widget_MonitorWin2;
    QLabel *widget_MonitorWin3;
    QLabel *widget_MonitorWin4;
    QWidget *page_2;
    QWidget *widget_play2;
    QWidget *page_3;
    QFrame *frame;
    QPushButton *pushButton_3;
    QToolButton *toolButton;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_4;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *TestForm)
    {
        if (TestForm->objectName().isEmpty())
            TestForm->setObjectName(QString::fromUtf8("TestForm"));
        TestForm->resize(1920, 1080);
        TestForm->setMinimumSize(QSize(1024, 768));
        TestForm->setStyleSheet(QString::fromUtf8("border:1px solid rgb(0,0,0);background-color:rgb(255,255,255);"));
        verticalLayout_2 = new QVBoxLayout(TestForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(9, -1, -1, -1);
        stackedWidget = new QStackedWidget(TestForm);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_3 = new QVBoxLayout(page);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        widget_play = new QWidget(page);
        widget_play->setObjectName(QString::fromUtf8("widget_play"));
        widget_play->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(widget_play);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget_MonitorWin1 = new QLabel(widget_play);
        widget_MonitorWin1->setObjectName(QString::fromUtf8("widget_MonitorWin1"));
        widget_MonitorWin1->setStyleSheet(QString::fromUtf8("background-color:#000"));

        gridLayout->addWidget(widget_MonitorWin1, 0, 0, 1, 1);

        widget_MonitorWin2 = new QLabel(widget_play);
        widget_MonitorWin2->setObjectName(QString::fromUtf8("widget_MonitorWin2"));

        gridLayout->addWidget(widget_MonitorWin2, 0, 1, 1, 1);

        widget_MonitorWin3 = new QLabel(widget_play);
        widget_MonitorWin3->setObjectName(QString::fromUtf8("widget_MonitorWin3"));

        gridLayout->addWidget(widget_MonitorWin3, 1, 0, 1, 1);

        widget_MonitorWin4 = new QLabel(widget_play);
        widget_MonitorWin4->setObjectName(QString::fromUtf8("widget_MonitorWin4"));

        gridLayout->addWidget(widget_MonitorWin4, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_3->addWidget(widget_play);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        widget_play2 = new QWidget(page_2);
        widget_play2->setObjectName(QString::fromUtf8("widget_play2"));
        widget_play2->setGeometry(QRect(2, 2, 960, 480));
        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        frame = new QFrame(page_3);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(130, 340, 120, 80));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        pushButton_3 = new QPushButton(page_3);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(330, 360, 89, 25));
        toolButton = new QToolButton(page_3);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setGeometry(QRect(370, 430, 26, 24));
        checkBox = new QCheckBox(page_3);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(200, 450, 92, 23));
        checkBox_2 = new QCheckBox(page_3);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(490, 410, 92, 23));
        stackedWidget->addWidget(page_3);

        verticalLayout_2->addWidget(stackedWidget);

        widget_2 = new QWidget(TestForm);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton = new QPushButton(widget_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(128, 32));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(128, 32));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_4 = new QPushButton(widget_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setMinimumSize(QSize(128, 32));

        horizontalLayout->addWidget(pushButton_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(widget_2);


        retranslateUi(TestForm);

        QMetaObject::connectSlotsByName(TestForm);
    } // setupUi

    void retranslateUi(QWidget *TestForm)
    {
        TestForm->setWindowTitle(QCoreApplication::translate("TestForm", "Form", nullptr));
        widget_MonitorWin1->setText(QString());
        widget_MonitorWin2->setText(QString());
        widget_MonitorWin3->setText(QString());
        widget_MonitorWin4->setText(QString());
        pushButton_3->setText(QCoreApplication::translate("TestForm", "PushButton", nullptr));
        toolButton->setText(QCoreApplication::translate("TestForm", "...", nullptr));
        checkBox->setText(QCoreApplication::translate("TestForm", "CheckBox", nullptr));
        checkBox_2->setText(QCoreApplication::translate("TestForm", "CheckBox", nullptr));
        pushButton->setText(QCoreApplication::translate("TestForm", "1", nullptr));
        pushButton_2->setText(QCoreApplication::translate("TestForm", "2", nullptr));
        pushButton_4->setText(QCoreApplication::translate("TestForm", "3", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TestForm: public Ui_TestForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTFORM_H
