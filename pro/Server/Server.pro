#-------------------------------------------------
#
# Project created by QtCreator 2016-06-28T12:10:55
#
#-------------------------------------------------

QT += core gui
QT += widgets
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++0x

TARGET = ../../bin/Server/Server
TEMPLATE = app
CONFIG += c++11
OBJECTS_DIR = ../../obj/Server
MOC_DIR = ../../obj/Server/moc
UI_DIR = ../../forms/server/

SOURCES += ../../src/shared/*.cpp \
    ../../src/server/*.cpp \
    ../../src/server/modules/*.cpp \
    ../../src/server/interfaces/*.cpp \
    ../../src/server/controllers/*.cpp

HEADERS  += ../../include/shared/*.h \
    ../../include/server/*.h \
    ../../include/server/controllers/*.h \
    ../../include/server/interfaces/*.h \
    ../../include/server/modules/*.h \
    ../../include/shared/packages/*.h \
    ../../include/shared/messages/*.h \
    ../../include/shared/crypto/key.h \
    ../../include/shared/crypto/*.h

FORMS    += ../../forms/server/mainwindow.ui

unix|win32: LIBS += -L$$PWD/../../lib/crypto++/installed/lib/ -lcryptopp

INCLUDEPATH += $$PWD/../../lib/crypto++/installed/include
DEPENDPATH += $$PWD/../../lib/crypto++/installed/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../lib/crypto++/installed/lib/cryptopp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../lib/crypto++/installed/lib/libcryptopp.a
