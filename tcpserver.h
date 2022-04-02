#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "iploader.h"
#include <QWidget>
#include "util.h"
#include <QtNetwork>
namespace Ui {
class TcpServer;
}

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit TcpServer(QWidget *parent = nullptr);
    ~TcpServer();
private slots:
    void append(const QString &type,const QString &data);
    void newconnect();
    void error(QTcpSocket* conn);
//    void writeData(QTcpSocket* conn,const QString &data);
//    void writeData(const QString &data);
    void readData(QTcpSocket* conn);
    void autoRead(QDateTime currentTime,QString currentLine);
    void initConfig();
    void saveConfig();

    void disconnect_slot(QTcpSocket* conn);
    void on_listenbtn_clicked();

    void on_btnSave_clicked();

    void on_btnClear_clicked();

    void on_btnRemove_clicked();

    void on_btnSend_clicked();

    void on_saveconfigbtn_clicked();

    void on_replaybtn_clicked();



    void on_autobtn_clicked();

private:
    Ui::TcpServer *ui;
    bool isListening;
    bool isReplaying;
    ipLoader *iploader;
    QTextStream *in;
    QFile *file;
    QTcpServer *qtcpserver;
    QTimer *timer;
    QList<QTcpSocket *> clients;
};

#endif // TCPSERVER_H
