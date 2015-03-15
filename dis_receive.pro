#-------------------------------------------------
#
# Project created by QtCreator 2014-10-02T23:16:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

TEMPLATE = subdirs

SUBDIRS += main \
    RDM \
    test \
    Visualization

Visualization.depends = RDM

main.depends = RDM
main.depends = Visualization

test.depends = RDM
test.depends = Visualization

