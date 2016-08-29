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


SOURCES += ../../../src/test/server_test_main.cpp
SOURCES  += ../../../src/server/*.cpp
SOURCES  += ../../../src/shared/*.cpp
SOURCES -= ../../../src/server/__main.cpp

HEADERS  += ../../../include/server/*.h
HEADERS  += ../../../include/shared/*.h
