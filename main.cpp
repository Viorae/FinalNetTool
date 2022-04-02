#include "mainwindow.h"
#include <QStyleFactory>
#include <QApplication>
#include "config.h"
int main(int argc, char *argv[])
{

    qApp->setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);
    //qDebug()<<qApp->applicationDirPath()<<"   "<<qApp->applicationFilePath();
    Config::ConfigFile = QString("%1/config.ini").arg(qApp->applicationDirPath());
    Config::readConfig();

    mainWindow w;
    w.show();
    return a.exec();
}
