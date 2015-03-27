#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T00:12:18
#
#-------------------------------------------------

QT       += core

QT       -= gui

DEFINES += DIS_VERSION=6

TARGET = dis_playback
CONFIG += console
CONFIG -= app_bundle
CONFIG += debug

TEMPLATE = app

win32 {
    kdis_inc_path = C:/Program Files (x86)/KDIS/include
    INCLUDEPATH += $$quote($$kdis_inc_path)
    QMAKE_CXXFLAGS += -isystem \"$$kdis_inc_path\"
    CONFIG(debug, debug|release){
        QMAKE_LFLAGS += -g
        kdis_lib_path = C:/Program Files (x86)/KDIS/lib/debug
        kdis_lib_name = libkdisd.a
    }
    CONFIG(release, debug|release){
        kdis_lib_path = C:/Program Files (x86)/KDIS/lib/release
        kdis_lib_name = libkdis.a
    }
    LIBS += $$quote($$kdis_lib_path/$$kdis_lib_name)
    LIBS += "-lws2_32"
}

unix {
    LIBS += -lkdis
}

SOURCES += main.cpp
