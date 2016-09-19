#-------------------------------------------------
#
# Project created by QtCreator 2016-07-27T17:05:49
#
#-------------------------------------------------

QT += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++0x

TARGET = ../../bin/Client/Client
TEMPLATE = app
CONFIG += c++11
OBJECTS_DIR = ../../obj/Client
MOC_DIR = ../../obj/Client/moc
UI_DIR = ../../forms/client/

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

unix|win32: LIBS += -L$$PWD/../../lib/crypto++/installed/lib/ -lcryptopp

INCLUDEPATH += $$PWD/../../lib/crypto++/installed/include
DEPENDPATH += $$PWD/../../lib/crypto++/installed/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../lib/crypto++/installed/lib/cryptopp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../lib/crypto++/installed/lib/libcryptopp.a
