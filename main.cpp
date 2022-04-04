#include "mainwindow.h"
#include <QStyleFactory>
#include <QApplication>
#include "config.h"
int main(int argc, char *argv[])
{

    qApp->setStyle(QStyleFactory::create("Fusion"));
    //qApp->setStyleSheet("QTextEdit#textEdit{background-color: white;border: 2px solid black;border-radius:5px;}");
    //qApp->setStyleSheet("QFrame#frame{border: 0.5px solid black;background-color:#D1D0D0}");
    QApplication a(argc, argv);
    //qDebug()<<qApp->applicationDirPath()<<"   "<<qApp->applicationFilePath();
    Config::ConfigFile = QString("%1/config.ini").arg(qApp->applicationDirPath());
    Config::readConfig();
    mainWindow w;

    w.show();
    return a.exec();
}
