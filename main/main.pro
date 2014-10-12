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
CONFIG   -= staticlib

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ../KDIS/include \
    ../RDM

LIBS += "../../build-dis_receive-Desktop_Qt_5_3_MinGW_32bit-Release/RDM/release/librdm.a"
LIBS += "C:\Users\Comp\school_work\CSE\dis_playback\KDIS\lib\libkdis.a"
LIBS += "-lws2_32"
