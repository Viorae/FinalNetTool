#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>

namespace Ui {
class TcpServer;
}

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit TcpServer(QWidget *parent = nullptr);
    ~TcpServer();

private:
    Ui::TcpServer *ui;
};

#endif // TCPSERVER_H
