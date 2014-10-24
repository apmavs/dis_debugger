#-------------------------------------------------
#
# Project created by QtCreator 2014-10-02T23:16:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

DEFINES += DIS_VERSION=6

TARGET = rdm
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = lib

CONFIG += staticlib

SOURCES += NetworkPduSource.cpp \
    DataModelController.cpp \
    PduSource.cpp

HEADERS += \
    NetworkPduSource.h \
    DataModelController.h \
    PduSource.h \
    PduObserver.h

win32 {
    INCLUDEPATH += "C:/Program Files (x86)/KDIS/include"
    LIBS += "-lws2_32"
}
