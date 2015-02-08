#-------------------------------------------------
#
# Project created by QtCreator 2014-10-11T22:13:52
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

DEFINES += DIS_VERSION=6

TARGET = dis_debugger_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

win32 {
    kdis_inc_path = C:/Program Files (x86)/KDIS/include
    CONFIG(debug, debug|release){
        QMAKE_LFLAGS += -g
        kdis_lib_path = C:/Program Files (x86)/KDIS/lib/debug
        kdis_lib_name = libkdisd.a
    }
    CONFIG(release, debug|release){
        kdis_lib_path = C:/Program Files (x86)/KDIS/lib/release
        kdis_lib_name = libkdis.a
    }
    INCLUDEPATH += ../..
}

QMAKE_CXXFLAGS += -isystem \"$$kdis_inc_path\"

INCLUDEPATH += $$quote($$kdis_inc_path)
DEPENDPATH += $$quote($$kdis_inc_path)

INCLUDEPATH += ../RDM

CONFIG(debug, debug|release){
    LIBS += -L../../gtest/debug
    LIBS += ../../build/RDM/debug/librdm.a
}
CONFIG(release, debug|release){
    LIBS += -L../../gtest/release
    LIBS += ../../build/RDM/release/librdm.a
}

LIBS += -lgtest
LIBS += -lgtest_main
LIBS += $$quote($$kdis_lib_path/$$kdis_lib_name)
win32:LIBS += -lws2_32

SOURCES += main.cpp \
    NetworkPduSourceTest.cpp \
    DatumDefTest.cpp \
    DataModelControllerTest.cpp \
    DatumIdentifierTest.cpp \
    DatumInfoTest.cpp
