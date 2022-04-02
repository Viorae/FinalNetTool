QT       += core gui
QT += network
QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    config.cpp \
    finddialog.cpp \
    form.cpp \
    iploader.cpp \
    main.cpp \
    mainwindow.cpp \
    tcpclient.cpp \
    tcpserver.cpp \
    udpserver.cpp \
    util.cpp

HEADERS += \
    config.h \
    finddialog.h \
    form.h \
    iploader.h \
    mainwindow.h \
    tcpclient.h \
    tcpserver.h \
    udpserver.h \
    util.h

FORMS += \
    finddialog.ui \
    form.ui \
    mainwindow.ui \
    tcpclient.ui \
    tcpserver.ui \
    udpserver.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
