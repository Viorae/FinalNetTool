#include "tcpserver.h"
#include "ui_tcpserver.h"
#include "config.h"
TcpServer::TcpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    isListening=false;
    timer=new QTimer(this);
    iploader=new ipLoader();
    iploader->loadip(ui->cboxaddr);
    initConfig();
//    timer = new QTimer(this);
//    timer->setSingleShot(true);
    qtcpserver=new QTcpServer(this);
    connect(qtcpserver,SIGNAL(newConnection()),this,SLOT(newconnect()));
    connect(timer,&QTimer::timeout,this,&TcpServer::on_btnSend_clicked);
}
void TcpServer::initConfig(){
    ui->cboxaddr->setCurrentIndex(ui->cboxaddr->findText(Config::TcpServerBindIP));
    ui->port->setText(QString::number(Config::TcpServerPort));
    ui->ckSelectAll->setChecked(Config::TcpServerSendAll);
}
void TcpServer::saveConfig()
{
    Config::TcpServerBindIP = ui->cboxaddr->currentText();
    Config::TcpServerBindPort = ui->port->text().toInt();
    Config::TcpServerSendAll=ui->ckSelectAll->isChecked();
    Config::writeConfig();
}
void TcpServer::newconnect(){
    QTcpSocket *conn=qtcpserver->nextPendingConnection();
    QString ip=conn->peerAddress().toString();
    int port=conn->peerPort();
    connect(conn,&QAbstractSocket::errorOccurred,this,[this,conn]{error(conn);});
    connect(conn,&QTcpSocket::disconnected,this,[this,conn] { disconnect_slot(conn);});
    connect(conn,&QTcpSocket::readyRead,this,[this,conn]{readData(conn);});

    clients.append(conn);
    append("Receive",QString("客户端上线，地址：%1，端口：%2").arg(conn->peerAddress().toString()).arg(conn->peerPort()));
    QString str = QString("%1:%2").arg(ip).arg(port);
    ui->listWidget->addItem(str);
    ui->labCount->setText(QString("共 %1 个客户端").arg(ui->listWidget->count()));
}
void TcpServer::error(QTcpSocket* conn){
    //ui->txtMain->append("Error occurred, details not written yet :)");
    append("Receive",conn->errorString());
}
void TcpServer::readData(QTcpSocket* conn){
    QByteArray data=conn->readAll();
    if(data.length()<=0){
        return;
    }
    QString buffer;
    if(ui->ckhexread->isChecked()){
        buffer=util::byteArrayToHexStr(data);
    }
    else{
        buffer=QString(data);
    }
    append("Receive",QString("接收%1/%2:%3/%4").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH::mm::ss")).arg(conn->peerAddress().toString()).arg(conn->peerPort()).arg(buffer));
}
void TcpServer::disconnect_slot(QTcpSocket* conn){
    QString str = QString("%1:%2").arg(conn->peerAddress().toString()).arg(conn->peerPort());


    for (int i = 0; i <ui->listWidget->count(); i++) {
        if (ui->listWidget->item(i)->text()==str) {
            delete ui->listWidget->takeItem(i);
            break;
        }
    }
    clients.removeOne(conn);
    conn->deleteLater();
    ui->labCount->setText(QString("共 %1 个客户端").arg(ui->listWidget->count()));
    append("Receive","客户端离线");
}
void TcpServer::append(const QString &type,const QString &data){
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
//void TcpServer::writeData(const QString &data){
//    foreach(QTcpSocket *client,clients){
//        QByteArray buffer=data.toUtf8();
//        client->write(buffer);
//    }
//}
//void TcpServer::writeData(QTcpSocket* conn,const QString &data){
//    QByteArray buffer=data.toUtf8();
//    conn->write(buffer);
//}

TcpServer::~TcpServer()
{
    foreach (QTcpSocket *client, clients) {
        client->abort();
    }
    qtcpserver->close();
    delete ui;
}

void TcpServer::on_listenbtn_clicked()
{
    if(!isListening)
    {
        isListening=qtcpserver->listen(QHostAddress(ui->cboxaddr->currentText()),ui->port->text().toInt());
        if(isListening)
        {
            append("Send","监听成功");
            ui->listenbtn->setText("中断");
        }
        else{
            append("Send","监听失败");
        }
    }
    else{
        foreach (QTcpSocket *client, clients) {
            client->abort();
            qtcpserver->close();
        }
        isListening=false;
        ui->listenbtn->setText("监听");
        append("Error","停止监听");
    }
}


void TcpServer::on_btnSave_clicked()
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


void TcpServer::on_btnClear_clicked()
{
    ui->txtMain->clear();
}


void TcpServer::on_btnRemove_clicked()
{
    int row = ui->listWidget->currentRow();
    QString str = ui->listWidget->item(row)->text();
    QStringList list = str.split(":");
    foreach(QTcpSocket *client,clients)
    {
        if(list.at(0)==client->peerAddress().toString()&&list.at(1).toInt()==client->peerPort())
        {
            clients.removeOne(client);
            client->abort();
        }
    }
}


void TcpServer::on_btnSend_clicked()
{
    QByteArray buffer;
    if(!isListening){
        return;
    }
    QString data = ui->textEdit->toPlainText();
    if(ui->ckhexsend->isChecked()){
        buffer = util::hexStrToByteArray(data);
    }
    else{
        buffer=data.toUtf8();
    }
    if(ui->ckSelectAll->isChecked()){
        foreach(QTcpSocket *client,clients){

            client->write(buffer);
            append("Send",QString("发送/%1/%2:%3/%4").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH:mm:ss")).arg(client->peerAddress().toString()).arg(client->peerPort()).arg(data));
        }

    }
    else
    {
        int row = ui->listWidget->currentRow();
        if(row>=0){
            QString str = ui->listWidget->item(row)->text();
            QStringList list = str.split(":");
            foreach(QTcpSocket *client,clients)
            {

                if(list.at(0)==client->peerAddress().toString()&&list.at(1).toInt()==client->peerPort())
                {

                    client->write(buffer);
                    append("Send",QString("发送/%1/%2:%3/%4").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH::mm::ss")).arg(client->peerAddress().toString()).arg(client->peerPort()).arg(data));
                }
            }
        }

    }
}


void TcpServer::on_saveconfigbtn_clicked()
{
    saveConfig();
}

void TcpServer::autoRead(QDateTime currentTime,QString currentLine){
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
void TcpServer::on_replaybtn_clicked()
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



void TcpServer::on_autobtn_clicked()
{
    if(timer->isActive())
    {
        timer->stop();
    }
    else{
        timer->start(ui->intervaltxt->text().toInt());
    }
}

