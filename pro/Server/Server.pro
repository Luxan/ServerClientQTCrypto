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
    ../../src/server/*.cpp \
    ../../src/server/crypto_processor.cpp

HEADERS  += ../../include/shared/*.h \
    ../../include/server/*.h \
    ../../include/shared/user_credentials.h \
    ../../include/shared/user_relations.h

FORMS    += ../../forms/server/mainwindow.ui

unix|win32: LIBS += -L$$PWD/../../lib/crypto++/installed/lib/ -lcryptopp

INCLUDEPATH += $$PWD/../../lib/crypto++/installed/include
DEPENDPATH += $$PWD/../../lib/crypto++/installed/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../lib/crypto++/installed/lib/cryptopp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../lib/crypto++/installed/lib/libcryptopp.a
