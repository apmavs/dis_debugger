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
CONFIG   += staticlib

TEMPLATE = lib

win32 {
    CONFIG(debug, debug|release){
        rdm_lib_path  = ../../build/RDM/debug
        rdm_lib_name  = librdm.a
        kdis_lib_path = C:/Program Files (x86)/KDIS/lib/debug
        kdis_lib_name = libkdisd.a
        qwt_lib_name  = libqwtd.a
    }
    CONFIG(release, debug|release){
        rdm_lib_path  = ../../build/RDM/release
        rdm_lib_name  = librdm.a
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
INCLUDEPATH += $$quote($$kdis_inc_path)
INCLUDEPATH += $$quote($$qwt_inc_path)
DEPENDPATH += ../RDM
DEPENDPATH += $$quote($$kdis_inc_path)
DEPENDPATH += $$quote($$qwt_inc_path)

PRE_TARGETDEPS += $$quote($$kdis_lib_path)/$$kdis_lib_name
PRE_TARGETDEPS += $$quote($$qwt_lib_path)/$$qwt_lib_name
LIBS += $$quote($$rdm_lib_path/$$rdm_lib_name)
LIBS += $$quote($$kdis_lib_path/$$kdis_lib_name)
LIBS -= -lqwt
LIBS += $$quote($$qwt_lib_path/$$qwt_lib_name)
win32:LIBS += -lws2_32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += MainWindow.cpp \
    EntityDataList.cpp \
    DatumItem.cpp \
    WatchList.cpp \
    EntityDatumItem.cpp \
    WatchDatumItem.cpp \
    PlotGroupBox.cpp \
    PlotCurveItem.cpp \
    PlotWidget.cpp \
    TextEntryPopup.cpp

HEADERS  += MainWindow.h \
    EntityDataList.h \
    DatumItem.h \
    WatchList.h \
    EntityDatumItem.h \
    WatchDatumItem.h \
    PlotGroupBox.h \
    PlotCurveItem.h \
    PlotWidget.h \
    TextEntryPopup.h

FORMS    += MainWindow.ui \
    PlotWidget.ui \
    PlotGroupBox.ui \
    TextEntryPopup.ui
