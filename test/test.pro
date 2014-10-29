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

win32 {
    kdis_lib_path = C:/Program Files (x86)/KDIS/lib
    kdis_inc_path = C:/Program Files (x86)/KDIS/include
    INCLUDEPATH += ../..
}

QMAKE_CXXFLAGS += -isystem \"$$kdis_inc_path\"

INCLUDEPATH += $$quote($$kdis_inc_path)
DEPENDPATH += $$quote($$kdis_inc_path)

#INCLUDEPATH += ../../KDIS/include
INCLUDEPATH += ../RDM

LIBS += -L../../gtest

LIBS += -lgtest
LIBS += -lgtest_main
LIBS += ../../build/RDM/release/librdm.a
LIBS += $$quote($$kdis_lib_path/libkdis.a)
win32:LIBS += -lws2_32

SOURCES += main.cpp \
    NetworkPduSourceTest.cpp
