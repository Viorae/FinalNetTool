#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore>
class Config
{
public:
    static QString ConfigFile;
    //TCP Client
    static QString TcpClientBindIP;
    static int TcpClientBindPort;
    static QString TcpServerIP;
    static int TcpServerPort;
    static bool AutoPort;
    //TCP Server
    static QString TcpServerBindIP;
    static int TcpServerBindPort;
    static bool TcpServerSendAll;
    //UDP Client
    static QString UdpClientBindIP;
    static int UdpClientBindPort;
    static QString UdpTargetIP;
    static int UdpTargetPort;
    static bool UdpSendAll;
    static bool UdpNewConn;

    static void readConfig();
    static void writeConfig();
};

#endif // CONFIG_H
