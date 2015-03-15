#-------------------------------------------------
#
# Project created by QtCreator 2014-10-02T23:16:32
#
#-------------------------------------------------

QT       += core gui xml

DEFINES += DIS_VERSION=6

TARGET = DMLV
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

win32 {
    CONFIG(debug, debug|release){
        rdm_lib_path  = ../../build/RDM/debug
        rdm_lib_name  = librdm.a
        visualization_lib_path = ../../build/Visualization/debug
        visualization_lib_name = libdis_visualization.a
        kdis_lib_path = C:/Program Files (x86)/KDIS/lib/debug
        kdis_lib_name = libkdisd.a
        qwt_lib_name  = libqwtd.a
    }
    CONFIG(release, debug|release){
        rdm_lib_path  = ../../build/RDM/release
        rdm_lib_name  = librdm.a
        visualization_lib_path = ../../build/Visualization/release
        visualization_lib_name = libdis_visualization.a
        kdis_lib_path = C:/Program Files (x86)/KDIS/lib/release
        kdis_lib_name = libkdis.a
        qwt_lib_name  = libqwt.a
    }
    kdis_inc_path = C:/Program Files (x86)/KDIS/include
    qwt_root      = C:/Qwt-6.1.2
    qwt_lib_path  = C:/Qwt-6.1.2/lib
    qwt_inc_path  = C:/Qwt-6.1.2/include
}

include ( $$qwt_root/features/qwt.prf )

QMAKE_CXXFLAGS += -isystem \"$$kdis_inc_path\"

INCLUDEPATH += ../RDM
INCLUDEPATH += ../Visualization
INCLUDEPATH += $$quote($$kdis_inc_path)
INCLUDEPATH += $$quote($$qwt_inc_path)
DEPENDPATH += ../RDM
DEPENDPATH += ../Visualization
DEPENDPATH += $$quote($$kdis_inc_path)
DEPENDPATH += $$quote($$qwt_inc_path)

LIBS += $$quote($$visualization_lib_path/$$visualization_lib_name)
LIBS += $$quote($$rdm_lib_path/$$rdm_lib_name)
LIBS += $$quote($$kdis_lib_path/$$kdis_lib_name)
LIBS -= -lqwt
LIBS += $$quote($$qwt_lib_path/$$qwt_lib_name)
win32:LIBS += -lws2_32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES = main.cpp

