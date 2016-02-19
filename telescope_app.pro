#-------------------------------------------------
#
# Project created by QtCreator 2016-02-18T00:01:30
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = telescope_app
TEMPLATE = app thread

#include("$$system(root-config --incdir)/rootcint.pri")

#INCLUDEPATH +=$$system(root-config --incdir)

SOURCES += main.cpp\
        mainwindow.cpp \
    ../../qcustomplot/qcustomplot.cpp \
    defaultdialog.cpp \
    npointsdiag.cpp

HEADERS  += mainwindow.h \
    ../../qcustomplot/qcustomplot.h \
    defaultdialog.h \
    npointsdiag.h

INCLUDEPATH += /home/orsosa/qcustomplot

FORMS    += mainwindow.ui \
    defaultdialog.ui \
    npointsdiag.ui
