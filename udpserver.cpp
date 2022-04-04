#include "udpserver.h"
#include "ui_udpserver.h"

UdpServer::UdpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UdpServer)
{
    ui->setupUi(this);
    //    loadip(ui->ipcbox);
    timer=new QTimer(this);
    socket = new QUdpSocket(this);
    iploader=new ipLoader();
    iploader->loadip(ui->ipcbox);
    initConfig();
    isBinded=false;
    connect(timer,&QTimer::timeout,this,&UdpServer::on_btnSend_clicked);
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(error()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
}
void UdpServer::initConfig(){
    ui->ipcbox->setCurrentIndex(ui->ipcbox->findText(Config::UdpClientBindIP));
    ui->port->setText(QString::number(Config::UdpClientBindPort));
    ui->targetAddr->setText(Config::UdpTargetIP);
    ui->targetPort->setText(QString::number(Config::UdpTargetPort));
    ui->ckSelectAll->setChecked(Config::UdpSendAll);
    ui->cknew->setChecked(Config::UdpNewConn);
}
void UdpServer::saveConfig(){
    Config::UdpClientBindIP=ui->ipcbox->currentText();
    Config::UdpClientBindPort=ui->port->text().toInt();
    Config::UdpTargetIP=ui->targetAddr->text();
    Config::UdpTargetPort=ui->targetPort->text().toInt();
    Config::UdpNewConn=ui->cknew->isChecked();
    Config::UdpSendAll=ui->ckSelectAll->isChecked();
    Config::writeConfig();
}
void UdpServer::append(const QString &type, const QString &data)
{

    //过滤回车换行符
    QString strData = data;
    strData = strData.replace("\r", "");
    strData = strData.replace("\n", "");

    if(type=="Send")
    {
        ui->txtMain->setTextColor(QColor("#22A3A9"));
    }
    else if(type=="Receive"){
        ui->txtMain->setTextColor(QColor("#753775"));
    }
    else if(type=="Error"){
        ui->txtMain->setTextColor(QColor("#D64D54"));
    }
    ui->txtMain->append(strData);
}
void UdpServer::error()
{
    append("Error", socket->errorString());
}
void UdpServer::readData()
{
    QHostAddress host;
    quint16 port;
    QByteArray data;
    QString buffer;

    while (socket->hasPendingDatagrams()) {
        data.resize(socket->pendingDatagramSize());
        socket->readDatagram(data.data(), data.size(), &host, &port);
        if(ui->ckhexread->isChecked())
        {
            buffer=util::byteArrayToHexStr(data);
        }
        else{
            buffer = QString(data);
        }

        QString ip = host.toString();
        ip = ip.replace("::ffff:", "");
        if (ip.isEmpty()) {
            continue;
        }

        QString str = QString("[%1:%2] %3").arg(ip).arg(port).arg(buffer);
        append("Receive", str);


        str = QString("%1:%2").arg(ip).arg(port);
        for (int i = 0; i < ui->listWidget->count(); i++) {
            QString s = ui->listWidget->item(i)->text();
            if (str == s) {
                return;
            }
        }
        ui->listWidget->addItem(str);
        ui->labCount->setText(QString("共 %1 个客户端").arg(ui->listWidget->count()));

    }
}
void UdpServer::sendData(const QString &ip, int port, const QString &data)
{
    QByteArray buffer;
    if(ui->ckhexsend->isChecked()){
        buffer=util::hexStrToByteArray(data);
    }
    else{
        buffer = data.toUtf8();
    }
    socket->writeDatagram(buffer, QHostAddress(ip), port);
    QString str = QString("[%1:%2] %3").arg(ip).arg(port).arg(data);
    append(0, str);
}
UdpServer::~UdpServer()
{
    delete ui;
}

void UdpServer::on_btnSend_clicked()
{
    QString data = ui->textEdit->toPlainText();
    if (data.length() <= 0) {
        return;
    }
    if(ui->cknew->isChecked()){
        sendData(ui->targetAddr->text(), ui->targetPort->text().toInt(), data);
    }
    else if (ui->ckSelectAll->isChecked()) {
        for (int i = 0; i < ui->listWidget->count(); i++) {
            QString str = ui->listWidget->item(i)->text();
            QStringList list = str.split(":");
            sendData(list.at(0), list.at(1).toInt(), data);
        }
    } else {
        int row = ui->listWidget->currentRow();
        if (row >= 0) {
            QString str = ui->listWidget->item(row)->text();
            QStringList list = str.split(":");
            sendData(list.at(0), list.at(1).toInt(), data);
        }
    }
}


void UdpServer::on_btnRemove_clicked()
{
    if (ui->ckSelectAll->isChecked()) {
        ui->listWidget->clear();
    } else {
        int row = ui->listWidget->currentRow();
        if (row >= 0) {
            delete ui->listWidget->takeItem(row);
        }
    }
}


void UdpServer::on_btnClear_clicked()
{
    ui->txtMain->clear();
}


void UdpServer::on_btnSave_clicked()
{
    QString data = ui->txtMain->toPlainText();
    if(data.length()<=0)
    {
        return;
    }
    QString filename=QString("%1/%2.txt").arg(qApp->applicationDirPath()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"));
    QFile file(filename);
    if(file.open(QFile::WriteOnly|QFile::Text))
    {
        file.write(data.toUtf8());
        file.close();
    }
}


void UdpServer::on_btnListen_clicked()
{
    if (!isBinded) {

        bool ok = socket->bind(QHostAddress(ui->ipcbox->currentText()), ui->port->text().toInt());
        if (ok) {
            isBinded=ok;
            append("Receive", "监听成功");
            ui->btnListen->setText("关闭");
        } else {
            append("Error", QString("监听失败: %1").arg(socket->errorString()));
        }
    } else {
        socket->abort();
        isBinded=false;
        ui->btnListen->setText("监听");
    }
}



void UdpServer::on_saveconfigbtn_clicked()
{
    saveConfig();
}

void UdpServer::autoRead(QDateTime currentTime,QString currentLine){
    if(!isReplaying){
        return;
    }
    ui->textEdit->setText(currentLine);
    on_btnSend_clicked();
    QString nextLine;
    QDateTime nextTime;
    QStringList fields;
    while(!in->atEnd()){
        nextLine=in->readLine();
        fields=nextLine.split("/");
        if(fields.at(0)=="发送"){
            nextTime=QDateTime::fromString(fields.at(1),"yyyy-MM-dd-HH:mm:ss");
            QString temp=fields.at(3);
            QTimer::singleShot(currentTime.secsTo(nextTime)*1000,this,[this,nextTime,temp]{autoRead(nextTime,temp);});
            return;
        }
    }
    file->close();
    isReplaying=false;
    ui->replaybtn->setText("回放");
    append("Send","回放完成");
}
void UdpServer::on_replaybtn_clicked()
{
    if(ui->replaybtn->text()=="回放"){
        QStringList files = QFileDialog::getOpenFileNames(this,"选择回放文件",qApp->applicationDirPath(),"文本文档 (*.txt)");
        if(files.isEmpty()){
            return;
        }
        file=new QFile(files.at(0));
        //QFile file(files.at(0));
        if(!file->open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", file->errorString());
        }
        in=new QTextStream(file);
        //QTextStream in(&file);

        //in=new QTextStream(&file);
        QString currentLine;
        QStringList fields;
        QDateTime firstTime;
        while(!in->atEnd())
        {
            currentLine=in->readLine();
            fields=currentLine.split("/");
            if(fields.at(0)=="发送"){
                qDebug()<<"called";
                firstTime=QDateTime::fromString(fields.at(1),"yyyy-MM-dd-HH:mm:ss");
                isReplaying=true;
                ui->replaybtn->setText("停止回放");
                autoRead(firstTime,fields.at(3));
                return;
            }
        }
    }
    else{
        ui->replaybtn->setText("回放");
        append("Error","回放中止");
        //        timer->stop();
        isReplaying=false;
        delete file;
        delete in;
    }
}


void UdpServer::on_autobtn_clicked()
{
    if(timer->isActive())
    {
        timer->stop();
        ui->autobtn->setText("定时发送");
    }
    else{
        ui->autobtn->setText("停止发送");
        timer->start(ui->intervaltxt->text().toInt());
    }
}

