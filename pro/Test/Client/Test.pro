#-------------------------------------------------
#
# Project created by QtCreator 2016-07-27T17:05:49
#
#-------------------------------------------------

QT += testlib

TARGET = ../../../bin/Test/Client/Test
TEMPLATE = app
CONFIG += c++11
OBJECTS_DIR = ../../../obj/Test/Client/


SOURCES += ../../../src/test/client_test_main.cpp
SOURCES  += ../../../src/client/*.cpp
SOURCES  += ../../../src/shared/*.cpp
SOURCES -= ../../../src/client/main.cpp

HEADERS  += ../../../include/client/*.h
HEADERS  += ../../../include/shared/*.h
