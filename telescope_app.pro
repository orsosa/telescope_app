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
    /home/hodoscopio/qcustomplot/qcustomplot.cpp \
    gatedialog.cpp \
    npointsdialog.cpp \
    azimuthdialog.cpp \
    distancedialog.cpp \
    polardialog.cpp

HEADERS  += mainwindow.h \
    /home/hodoscopio/qcustomplot/qcustomplot.h \
    gatedialog.h \
    npointsdialog.h \
    azimuthdialog.h \
    distancedialog.h \
    polardialog.h

INCLUDEPATH += /home/hodoscopio/qcustomplot

FORMS    += mainwindow.ui \
    gatedialog.ui \
    npointsdialog.ui \
    azimuthdialog.ui \
    distancedialog.ui \
    polardialog.ui
