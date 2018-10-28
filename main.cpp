#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QFontDatabase>
#include <QFile>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    int ret = 0;
    {
        QFontDatabase::addApplicationFont(":/CENTURY_GOTHIC.TTF");
        qDebug() << QString("Font loading: %1").arg(ret);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
