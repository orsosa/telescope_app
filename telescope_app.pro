#-------------------------------------------------
#
# Project created by QtCreator 2016-02-18T00:01:30
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = telescope_app
TEMPLATE = app

include("$$system(root-config --incdir)/rootcint.pri")

INCLUDEPATH +=$$system(root-config --incdir)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
