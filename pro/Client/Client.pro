#-------------------------------------------------
#
# Project created by QtCreator 2016-07-27T17:05:49
#
#-------------------------------------------------

QT += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../../bin/Client/Client
TEMPLATE = app
CONFIG += c++11
OBJECTS_DIR = ../../obj/Client

SOURCES  += ../../src/shared/*.cpp \
    ../../src/client/*.cpp

HEADERS  +=  ../../include/client/*.h \
    ../../include/shared/*.h \
    ../../include/shared/packages/*.h \
    ../../include/shared/messages/*.h \
    ../../include/shared/crypto/*.h

FORMS    += \  
    ../../forms/client/loginwindow.ui \
    ../../forms/client/mainwindow.ui
