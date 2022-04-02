#include "config.h"

QString Config::ConfigFile = "config.ini";
//TcpClient
QString Config::TcpClientBindIP="127.0.0.1";
int Config::TcpClientBindPort=6001;
QString Config::TcpServerIP="127.0.0.1";
int Config::TcpServerPort=6000;
bool Config::AutoPort=true;
//TcpServer
QString Config::TcpServerBindIP="127.0.0.1";
int Config::TcpServerBindPort=6000;
bool Config::TcpServerSendAll=true;
//UdpClient
QString Config::UdpClientBindIP="127.0.0.1";
int Config::UdpClientBindPort=6000;
QString Config::UdpTargetIP="127.0.0.1";
int Config::UdpTargetPort=6001;
bool Config::UdpSendAll=false;
bool Config::UdpNewConn=true;

void Config::readConfig(){
    QSettings set(ConfigFile, QSettings::IniFormat);

    set.beginGroup("TcpClientConfig");
    Config::TcpClientBindIP=set.value("TcpClientBindIP", Config::TcpClientBindIP).toString();
    Config::TcpClientBindPort=set.value("TcpClientBindPort", Config::TcpClientBindPort).toInt();
    Config::TcpServerIP=set.value("TcpServerIP", Config::TcpServerIP).toString();
    Config::TcpServerPort=set.value("TcpServerPort", Config::TcpServerPort).toInt();
    set.endGroup();

    set.beginGroup("TcpServerConfig");
    Config::TcpServerBindIP=set.value("TcpServerBindIP",Config::TcpServerBindIP).toString();
    Config::TcpServerBindPort=set.value("TcpServerBindPort",Config::TcpServerBindPort).toInt();
    Config::TcpServerSendAll=set.value("TcpServerSendAll",Config::TcpServerSendAll).toBool();
    set.endGroup();

    set.beginGroup("UdpClientConfig");
    Config::UdpClientBindIP=set.value("UdpClientBindIP",Config::UdpClientBindIP).toString();
    Config::UdpClientBindPort=set.value("UdpClientBindPort",Config::UdpClientBindPort).toInt();
    Config::UdpTargetIP=set.value("UdpTargetIP",Config::UdpTargetIP).toString();
    Config::UdpTargetPort=set.value("UdpTargetPort",Config::UdpTargetPort).toInt();
    Config::UdpSendAll=set.value("UdpSendAll",Config::UdpSendAll).toBool();
    Config::UdpNewConn=set.value("UdpNewConn",Config::UdpNewConn).toBool();
    set.endGroup();


}

void Config::writeConfig(){
    QSettings set(ConfigFile, QSettings::IniFormat);

    set.beginGroup("TcpClientConfig");
    set.setValue("TcpClientBindIP",Config::TcpClientBindIP);
    set.setValue("TcpClientBindPort",Config::TcpClientBindPort);
    set.setValue("TcpServerIP",Config::TcpServerIP);
    set.setValue("TcpServerPort",Config::TcpServerPort);
    set.setValue("AutoPort",Config::AutoPort);
    set.endGroup();

    set.beginGroup("TcpServerConfig");
    set.setValue("TcpServerBindIP",Config::TcpServerBindIP);
    set.setValue("TcpServerBindPort",Config::TcpServerBindPort);
    set.setValue("TcpServerSendAll",Config::TcpServerSendAll);
    set.endGroup();

    set.beginGroup("UdpClientConfig");
    set.setValue("UdpClientBindIP",Config::UdpClientBindIP);
    set.setValue("UdpClientBindPort",Config::UdpClientBindPort);
    set.setValue("UdpTargetIP",Config::UdpTargetIP);
    set.setValue("UdpTargetPort",Config::UdpTargetPort);
    set.setValue("UdpNewConn",Config::UdpNewConn);
    set.setValue("UdpSendAll",Config::UdpSendAll);
    set.endGroup();


    QFile file(Config::ConfigFile);
    if(file.size()==0){
        writeConfig();
        return;
    }
}
