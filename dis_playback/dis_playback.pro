#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T00:12:18
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = dis_playback
CONFIG += console
CONFIG -= app_bundle
CONFIG += debug

TEMPLATE = app

win32 {
    INCLUDEPATH += KDIS\include
    LIBS += "C:\Users\Comp\school_work\CSE\dis_playback\KDIS\lib\libkdis.a"
    LIBS += "-lws2_32"
}

unix {
    LIBS += -lkdis
}

SOURCES += main.cpp
