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

INCLUDEPATH += ../../KDIS/include \
    ../RDM

LIBS += ../../build-dis_receive-Desktop_Qt_5_3_MinGW_32bit-Release/RDM/release/librdm.a
LIBS += ../../KDIS/lib/libkdis.a
LIBS += -lws2_32

SOURCES += main.cpp
