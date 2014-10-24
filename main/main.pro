#-------------------------------------------------
#
# Project created by QtCreator 2014-10-02T23:16:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

DEFINES += DIS_VERSION=6

TARGET = dis_receive
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

win32 {
    kdis_lib_path = "C:/Program Files (x86)/KDIS/lib"
    INCLUDEPATH += "C:/Program Files (x86)/KDIS/include"
    DEPENDPATH += "C:/Program Files (x86)/KDIS/include"
}

INCLUDEPATH += ../RDM

PRE_TARGETDEPS += $$kdis_lib_path/libkdis.a
LIBS += ../../build/RDM/release/librdm.a
LIBS += $$kdis_lib_path/libkdis.a
win32:LIBS += -lws2_32

SOURCES += main.cpp
