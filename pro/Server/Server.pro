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
    ../../src/server/epoll_tcpchannel.cpp

HEADERS  += ../../include/shared/*.h \
    ../../include/server/*.h \
    ../../include/server/processor.h \
    ../../include/shared/hash.h \
    ../../include/server/epoll_tcpchannel.h \
    ../../include/shared/cipher.h \
    ../../include/shared/hasher.h

FORMS    += ../../forms/server/mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/openssl/install/lib/release/ -lssl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/openssl/install/lib/debug/ -lssl
else:unix: LIBS += -L$$PWD/../../lib/openssl/install/lib/ -lssl

INCLUDEPATH += $$PWD/../../lib/openssl/install/include
DEPENDPATH += $$PWD/../../lib/openssl/install/include
