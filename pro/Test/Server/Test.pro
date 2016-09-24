#-------------------------------------------------
#
# Project created by QtCreator 2016-07-27T17:05:49
#
#-------------------------------------------------

QT += testlib

TARGET = ../../../bin/Test/Server/Test
TEMPLATE = app
CONFIG += c++11
OBJECTS_DIR = ../../../obj/Test/Server/
MOC_DIR = ../../../obj/Test/Server/moc
QMAKE_CXXFLAGS += -std=c++0x
QT += testlib

unix:QMAKE_RPATHDIR += $$PWD/../../../lib/crypto++/installed/lib/

SOURCES += ../../../src/test/server_test_main.cpp \
    ../../../src/shared/*.cpp

HEADERS  += ../../../include/shared/*.h \
    ../../../include/test/*.h \
    ../../../include/test/test_shared/*.h

unix|win32: LIBS += -L$$PWD/../../../lib/crypto++/installed/lib/ -lcryptopp

INCLUDEPATH += $$PWD/../../../lib/crypto++/installed/include
DEPENDPATH += $$PWD/../../../lib/crypto++/installed/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../lib/crypto++/installed/lib/cryptopp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../../lib/crypto++/installed/lib/libcryptopp.a
