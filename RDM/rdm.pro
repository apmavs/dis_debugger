#-------------------------------------------------
#
# Project created by QtCreator 2014-10-02T23:16:32
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

DEFINES += DIS_VERSION=6

TARGET = rdm
CONFIG   += console
CONFIG   -= app_bundle

win32:kdis_inc_path = C:/Program Files (x86)/KDIS/include

QMAKE_CXXFLAGS += -isystem \"$$kdis_inc_path\"

INCLUDEPATH += $$quote($$kdis_inc_path)

TEMPLATE = lib

CONFIG += staticlib

SOURCES += NetworkPduSource.cpp \
    DataModelController.cpp \
    PduSource.cpp \
    DatumDef.cpp \
    DatumIdentifier.cpp \
    DatumValue.cpp \
    DatumInfo.cpp \
    VehicleMetadataLoader.cpp \
    PduDeconstructor.cpp \
    PduDef.cpp \
    DatumDefId.cpp \
    BaseDef.cpp

HEADERS += \
    NetworkPduSource.h \
    DataModelController.h \
    PduSource.h \
    PduObserver.h \
    DatumDef.h \
    DatumIdentifier.h \
    DatumValue.h \
    DatumInfo.h \
    VehicleMetadataLoader.h \
    PduDeconstructor.h \
    PduDef.h \
    DatumDefId.h \
    BaseDef.h \
    DatumObserver.h

win32 {
    LIBS += "-lws2_32"
}
