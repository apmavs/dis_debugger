#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T09:12:51
#
#-------------------------------------------------

QT       += core gui xml

DEFINES += DIS_VERSION=6

TARGET = dis_visualization
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

win32 {
    kdis_lib_path = C:/Program Files (x86)/KDIS/lib
    kdis_inc_path = C:/Program Files (x86)/KDIS/include
}

QMAKE_CXXFLAGS += -isystem \"$$kdis_inc_path\"

INCLUDEPATH += ../RDM
INCLUDEPATH += $$quote($$kdis_inc_path)
DEPENDPATH += ../RDM
DEPENDPATH += $$quote($$kdis_inc_path)

PRE_TARGETDEPS += $$quote($$kdis_lib_path)/libkdis.a
LIBS += ../../build/RDM/release/librdm.a
LIBS += $$quote($$kdis_lib_path/libkdis.a)
win32:LIBS += -lws2_32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp\
        MainWindow.cpp \
    EntityDataList.cpp

HEADERS  += MainWindow.h \
    EntityDataList.h

FORMS    += MainWindow.ui
