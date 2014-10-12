#-------------------------------------------------
#
# Project created by QtCreator 2014-10-11T22:13:52
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = dis_debugger_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../KDIS/include
INCLUDEPATH += ../RDM

LIBS += "C:\Users\Comp\school_work\CSE\dis_playback\KDIS\lib\libkdis.a"
LIBS += "C:\Users\Comp\school_work\CSE\test\gtest-1.7.0\libgtest.a"
LIBS += "C:\Users\Comp\school_work\CSE\test\gtest-1.7.0\libgtest_main.a"
LIBS += "../../build-dis_receive-Desktop_Qt_5_3_MinGW_32bit-Release/RDM/release/librdm.a"
LIBS += "C:\Users\Comp\school_work\CSE\dis_playback\KDIS\lib\libkdis.a"
LIBS += "-lws2_32"

SOURCES += main.cpp \
    NetworkPduSourceTest.cpp
