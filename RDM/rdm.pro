#-------------------------------------------------
#
# Project created by QtCreator 2014-10-02T23:16:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

DEFINES += DIS_VERSION=6

TARGET = rdm
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = lib

CONFIG += staticlib

INCLUDEPATH += ../../KDIS/include

SOURCES += NetworkPduSource.cpp \
    DataModelController.cpp \
    PduSource.cpp


LIBS += "../../KDIS/lib/libkdis.a"
LIBS += "-lws2_32"

HEADERS += \
    NetworkPduSource.h \
    DataModelController.h \
    PduSource.h \
    PduObserver.h
