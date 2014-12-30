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
    CONFIG(debug, debug|release){
        kdis_lib_path = C:/Program Files (x86)/KDIS/lib/debug
        kdis_lib_name = libkdisd.a
    }
    CONFIG(release, debug|release){
        kdis_lib_path = C:/Program Files (x86)/KDIS/lib/release
        kdis_lib_name = libkdis.a
    }
    kdis_inc_path = C:/Program Files (x86)/KDIS/include
}

QMAKE_CXXFLAGS += -isystem \"$$kdis_inc_path\"

INCLUDEPATH += ../RDM
INCLUDEPATH += $$quote($$kdis_inc_path)
DEPENDPATH += ../RDM
DEPENDPATH += $$quote($$kdis_inc_path)

PRE_TARGETDEPS += $$quote($$kdis_lib_path)/$$kdis_lib_name
LIBS += ../../build/RDM/release/librdm.a
LIBS += $$quote($$kdis_lib_path/$$kdis_lib_name)
win32:LIBS += -lws2_32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp\
        MainWindow.cpp \
    EntityDataList.cpp \
    DatumItem.cpp \
    EntityDatumItem.cpp

HEADERS  += MainWindow.h \
    EntityDataList.h \
    DatumItem.h \
    EntityDatumItem.h

FORMS    += MainWindow.ui
