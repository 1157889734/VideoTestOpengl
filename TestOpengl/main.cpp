#include "mainwindow.h"
#include <QApplication>
#include "testform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MainWindow w;
    TestForm w;
    w.showFullScreen();

    return a.exec();
}
