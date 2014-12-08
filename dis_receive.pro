#-------------------------------------------------
#
# Project created by QtCreator 2014-10-02T23:16:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

TEMPLATE = subdirs

SUBDIRS += RDM \
    test \
    Visualization

test.depends = RDM

