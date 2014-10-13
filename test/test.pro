#-------------------------------------------------
#
# Project created by QtCreator 2014-10-11T22:13:52
#
#-------------------------------------------------

QT       += core

QT       -= gui

DEFINES += DIS_VERSION=6

TARGET = dis_debugger_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../KDIS/include
INCLUDEPATH += ../..
INCLUDEPATH += ../RDM

LIBS += ../../gtest/libgtest.a
LIBS += ../../gtest/libgtest_main.a
LIBS += ../../build-dis_receive-Desktop_Qt_5_3_MinGW_32bit-Release/RDM/release/librdm.a
LIBS += ../../KDIS/lib/libkdis.a
LIBS += -lws2_32

SOURCES += main.cpp \
    NetworkPduSourceTest.cpp
