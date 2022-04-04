#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <QtNetwork>
#include <QWidget>
#include "iploader.h"
#include <QtWidgets>
#include "config.h"
#include "util.h"
namespace Ui {
class UdpServer;
}

class UdpServer : public QWidget
{
    Q_OBJECT

public:
    explicit UdpServer(QWidget *parent = nullptr);
    ~UdpServer();

private:
    QUdpSocket *socket;
    bool isBinded;
    bool isReplaying;
    QTimer *timer;
    QTextStream *in;
    QFile *file;
    ipLoader *iploader;
    Ui::UdpServer *ui;
private slots:
    void autoRead(QDateTime currentTime,QString currentLine);

    void error();
    void readData();
    void initConfig();
    void saveConfig();
    void sendData(const QString &ip, int port, const QString &data);
    void append(const QString &type, const QString &data);
    void on_btnSend_clicked();
    void on_btnRemove_clicked();
    void on_btnClear_clicked();
    void on_btnSave_clicked();
    void on_btnListen_clicked();
    void on_saveconfigbtn_clicked();
    void on_replaybtn_clicked();
    void on_autobtn_clicked();
};


#endif // UDPSERVER_H
