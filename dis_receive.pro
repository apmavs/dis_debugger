#-------------------------------------------------
#
# Project created by QtCreator 2014-10-02T23:16:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = dis_receive
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    NetworkPduSource.cpp \
    DataModelController.cpp \
    PduSource.cpp

INCLUDEPATH += KDIS\include

LIBS += "C:\Users\Comp\school_work\CSE\dis_playback\KDIS\lib\libkdis.a"
LIBS += "-lws2_32"

HEADERS += \
    NetworkPduSource.h \
    PduObserver.h \
    DataModelController.h \
    PduSource.h
