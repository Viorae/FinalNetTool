#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QtNetwork>
#include <QWidget>
#include "util.h"
#include <QtWidgets>
#include <QTextCursor>
#include "iploader.h"
#include "finddialog.h"
namespace Ui {
class TcpClient;
}

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
private slots:
    void search(QString searchstr);
    void reset();
    void connected();
    void readData();
    void error();
    void autoRead(QDateTime currentTime,QString currentLine);
    void disconnected();
    void append(const QString &type,const QString &data);
    void on_btnSend_clicked();
    void on_btnClear_clicked();
    void on_btnSave_clicked();
    void on_btnConnect_clicked();
    void on_ckAuto_stateChanged(int arg1);
    void saveConfig();
    void on_searchbtn_clicked();
    void initConfig();
    void on_saveconfigbtn_clicked();
    void on_replaybtn_clicked();
    void on_autobtn_clicked();
private:
    FindDialog *finddialog;
    Ui::TcpClient *ui;
    ipLoader *iploader;
    bool isConnected;
    bool isReplaying;
    QTextStream *in;
    QFile *file;
    QTimer *timer;
    QTcpSocket *socket;
    QList<QTextCursor> cursorlist;
};

#endif // TCPCLIENT_H
