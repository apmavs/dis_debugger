#-------------------------------------------------
#
# Project created by QtCreator 2014-10-11T22:13:52
#
#-------------------------------------------------

QT       += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
        qwt_lib_name  = libqwtd.a
    }
    CONFIG(release, debug|release){
        kdis_lib_path = C:/Program Files (x86)/KDIS/lib/release
        kdis_lib_name = libkdis.a
        qwt_lib_name  = libqwt.a
    }
    INCLUDEPATH += ../..
    qwt_root      = C:/Qwt-6.1.2
    qwt_lib_path  = C:/Qwt-6.1.2/lib
    qwt_inc_path  = C:/Qwt-6.1.2/include
}

include ( $$qwt_root/features/qwt.prf )

QMAKE_CXXFLAGS += -isystem \"$$kdis_inc_path\"

INCLUDEPATH += $$quote($$kdis_inc_path)
INCLUDEPATH += $$quote($$qwt_inc_path)
DEPENDPATH += $$quote($$kdis_inc_path)
DEPENDPATH += $$quote($$qwt_inc_path)

PRE_TARGETDEPS += $$quote($$qwt_lib_path)/$$qwt_lib_name

INCLUDEPATH += ../RDM \
               ../Visualization

CONFIG(debug, debug|release){
    LIBS += -L../../gtest/debug
    LIBS += ../../build/RDM/debug/librdm.a
    LIBS += ../../build/Visualization/debug/libdis_visualization.a
}
CONFIG(release, debug|release){
    LIBS += -L../../gtest/release
    LIBS += ../../build/RDM/release/librdm.a
    LIBS += ../../build/Visualization/release/libdis_visualization.a
}

LIBS += -lgtest
LIBS += -lgtest_main
LIBS += $$quote($$kdis_lib_path/$$kdis_lib_name)
LIBS -= -lqwt
LIBS += $$quote($$qwt_lib_path/$$qwt_lib_name)
win32:LIBS += -lws2_32

SOURCES += main.cpp \
    NetworkPduSourceTest.cpp \
    DatumDefTest.cpp \
    DataModelControllerTest.cpp \
    DatumIdentifierTest.cpp \
    DatumInfoTest.cpp \
    DatumValueTest.cpp \
    EntityDatumItemTest.cpp \
    WatchDatumItemTest.cpp \
    PlotCurveItemTest.cpp

HEADERS +=
