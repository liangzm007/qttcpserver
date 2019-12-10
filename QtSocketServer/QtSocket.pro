#-------------------------------------------------
#
# Project created by QtCreator 2019-12-10T09:24:45
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtSocketServer
TEMPLATE = app


SOURCES += main.cpp\
        QServerDlg.cpp \
    QSockServer.cpp

HEADERS  += QServerDlg.h \
    QSockServer.h

FORMS    += qserverdlg.ui
