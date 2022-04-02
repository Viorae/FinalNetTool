#include "iploader.h"
#include <QtNetwork>
ipLoader::ipLoader(){};
void ipLoader::loadip(QComboBox *cbox){
    QStringList ips;
    QList<QNetworkInterface> netInterfaces=QNetworkInterface::allInterfaces();
    foreach (const QNetworkInterface  &netInterface, netInterfaces) {

        QString humanReadableName = netInterface.humanReadableName().toLower();
        if (humanReadableName.startsWith("vmware network adapter") || humanReadableName.startsWith("npcap loopback adapter")) {
            continue;
        }

        bool flag = (netInterface.flags() == (QNetworkInterface::IsUp | QNetworkInterface::IsRunning | QNetworkInterface::CanBroadcast | QNetworkInterface::CanMulticast));
        if (flag) {
            QList<QNetworkAddressEntry> addrs = netInterface.addressEntries();
            foreach (QNetworkAddressEntry addr, addrs) {

                if (addr.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    QString ip4 = addr.ip().toString();
                    if (ip4 != "127.0.0.1") {
                        ips << ip4;
                    }
                }
            }
        }
    }
    cbox->clear();
    cbox->addItems(ips);
    if (!ips.contains("127.0.0.1")) {
        cbox->addItem("127.0.0.1");
    }
}
