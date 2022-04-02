#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include "udpserver.h"
mainWindow::mainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(new TcpServer, "TCP Server");
    ui->tabWidget->addTab(new TcpClient, "TCP Client");
    ui->tabWidget->addTab(new UdpServer, "UDP Client");
}

mainWindow::~mainWindow()
{
    delete ui;
}

