#include "tcpclient.h"
#include "ui_tcpclient.h"
#include "config.h"

TcpClient::TcpClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    iploader=new ipLoader();
    iploader->loadip(ui->cboxaddr);
//    loadip(ui->cboxaddr);
    initConfig();
    finddialog=new FindDialog();
    isConnected=false;
    socket=new QTcpSocket(this);
    connect(finddialog,SIGNAL(reset()),this,SLOT(reset()));
    connect(finddialog,SIGNAL(search(QString)),this,SLOT(search(QString)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(error()));
}
void TcpClient::initConfig(){
    ui->ckAuto->setChecked(Config::AutoPort);
    ui->clientport->setText(QString::number(Config::TcpServerPort));
    ui->cboxaddr->setCurrentIndex(ui->cboxaddr->findText(Config::TcpClientBindIP));
    ui->serveraddr->setText(Config::TcpServerIP);
    ui->serverport->setText(QString::number(Config::TcpServerPort));
}
void TcpClient::saveConfig()
{
    Config::TcpClientBindIP = ui->cboxaddr->currentText();
    Config::TcpClientBindPort = ui->clientport->text().toInt();
    Config::TcpServerIP = ui->serveraddr->text();
    Config::TcpServerPort = ui->serverport->text().toInt();
    Config::AutoPort=ui->ckAuto->isChecked();
    Config::writeConfig();
}
void TcpClient::error(){
    append("Error",socket->errorString());
}
void TcpClient::append(const QString &type,const QString &data){
    QString strData = data;
//    strData = strData.replace("\r", "");
//    strData = strData.replace("\n", "");

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
void TcpClient::search(QString searchstr){
        QTextCharFormat fmt;

        QTextCursor tempcursor=ui->txtMain->document()->find(searchstr);
        fmt=tempcursor.charFormat();
        fmt.setBackground(Qt::yellow);
        tempcursor.setCharFormat(fmt);
        cursorlist.append(tempcursor);

        while(tempcursor.hasSelection()){
            tempcursor=ui->txtMain->document()->find(searchstr,tempcursor);
            fmt=tempcursor.charFormat();
            fmt.setBackground(Qt::yellow);
            tempcursor.setCharFormat(fmt);
            cursorlist.append(tempcursor);
        }

}
void TcpClient::reset(){
//    int end=ui->txtMain->document()->toPlainText().length();
//    QTextCursor cursor(ui->txtMain->document());
//    cursor.setPosition(0, QTextCursor::MoveAnchor);
//    cursor.setPosition(end, QTextCursor::KeepAnchor);
//    QTextCharFormat fmt=cursor.charFormat();
//    fmt.setBackground(Qt::white);
//    cursor.setCharFormat(fmt);
    QTextCharFormat fmt;
    foreach(QTextCursor cursor,cursorlist){
        fmt=cursor.charFormat();
        fmt.setBackground(Qt::white);
        cursor.setCharFormat(fmt);
    }

    cursorlist.clear();
}
void TcpClient::readData(){
    QByteArray data = socket->readAll();
    if (data.length() <= 0) {
        return;
    }
    QString buffer;
    if(ui->ckhexread->isChecked()){
        buffer=util::byteArrayToHexStr(data);
    }
    else{
        buffer=QString(data);
    }
    append("Receive",QString("收%1 %2:%3 %4").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH::mm::ss")).arg(socket->peerAddress().toString()).arg(socket->peerPort()).arg(buffer));
}
void TcpClient::disconnected(){
    isConnected=false;
    ui->btnConnect->setText("连接");
    append("Receive","服务器断开");
}
void TcpClient::connected(){
    isConnected=true;
    ui->btnConnect->setText("断开");
    append("Receive","连接成功");
    append("Receive",QString("连接服务器地址：%1 ，端口：%2").arg(socket->peerAddress().toString()).arg(socket->peerPort()));

}
TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::on_btnSend_clicked()
{
    if(!isConnected){
        return;
    }
    QString data=ui->textEdit->toPlainText();
    if(data.length()<=0){
        return;
    }
    QByteArray buffer;
    if(ui->ckhexsend->isChecked()){
        buffer=util::hexStrToByteArray(data);
    }
    else{
        buffer=data.toUtf8();
    }

    socket->write(buffer);
    append("Send",QString("发%1 %2:%3 %4").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH::mm::ss")).arg(socket->peerAddress().toString()).arg(socket->peerPort()).arg(data));
}


void TcpClient::on_btnClear_clicked()
{
    ui->txtMain->clear();
}


void TcpClient::on_btnSave_clicked()
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


void TcpClient::on_btnConnect_clicked()
{
    if(!isConnected)
    {
        if(!ui->ckAuto->isChecked())
        {
            socket->bind(QHostAddress(ui->cboxaddr->currentText()),ui->clientport->text().toInt());
        }

        socket->connectToHost(ui->serveraddr->text(),ui->serverport->text().toInt());
    }
    else{
        socket->abort();
    }
}


void TcpClient::on_ckAuto_stateChanged(int arg1)
{

    ui->clientport->setEnabled(!ui->ckAuto->checkState());
}


void TcpClient::on_searchbtn_clicked()
{
    finddialog->show();
}


void TcpClient::on_saveconfigbtn_clicked()
{
    saveConfig();
}

void TcpClient::autoRead(QDateTime currentTime,QString currentLine){
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
void TcpClient::on_replaybtn_clicked()
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

