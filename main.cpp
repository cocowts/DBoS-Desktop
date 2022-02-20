#include "mainwindow.h"

#include "commonhelper.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CommonHelper::setStyleSheet(":/misc/resource/style/default.qss", qApp);

    MainWindow w;

    w.show();

    return a.exec();
}
