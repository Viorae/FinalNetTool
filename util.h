#ifndef UTIL_H
#define UTIL_H


#include <QObject>
class util
{
public:
    util();
    static char hexStrToChar(char data);
    static QByteArray hexStrToByteArray(const QString &data);
    static QString byteArrayToHexStr(const QByteArray &data);
};

#endif // UTIL_H
