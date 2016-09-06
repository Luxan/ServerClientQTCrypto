#-------------------------------------------------
#
# Project created by QtCreator 2016-06-28T12:10:55
#
#-------------------------------------------------

QT += core gui
QT += widgets
QT += sql

CONFIG += c++11
CONFIG += -lcrypto


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../../bin/Server/Server
TEMPLATE = app
OBJECTS_DIR = ../../obj/Server


SOURCES += ../../src/shared/*.cpp \
    ../../src/server/*.cpp

HEADERS  += ../../include/shared/*.h \
    ../../include/server/*.h

FORMS    += ../../forms/server/mainwindow.ui
