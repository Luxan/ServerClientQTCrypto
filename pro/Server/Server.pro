#-------------------------------------------------
#
# Project created by QtCreator 2016-06-28T12:10:55
#
#-------------------------------------------------

QT += core gui
QT += widgets
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../../bin/Server/Server
TEMPLATE = app
CONFIG += c++11
OBJECTS_DIR = ../../obj/Server

SOURCES += ../../src/shared/*.cpp \
    ../../src/server/*.cpp

HEADERS  += ../../include/shared/*.h \
    ../../include/server/*.h \
    ../../include/server/controller_gui_cryptoprocessor.h \
    ../../include/server/controller_gui_database.h \
    ../../include/server/controller_cryptoprocessor_threadworker.h \
    ../../include/server/controller_encryptionprocessor_threadworker.h \
    ../../include/server/controller_decryptionprocessor_threadworker.h \
    ../../include/server/controllers/controller_todelete.h \
    ../../include/server/controllers/controller_tcpchannel_encryptionprocessor.h \
    ../../include/server/controllers/controller_tcpchannel_decryptionprocessor.h \
    ../../include/server/controllers/controller_messageprocessor_threadworker.h \
    ../../include/server/controllers/controller_messageprocessor_database.h \
    ../../include/server/controllers/controller_messagecollector_messageprocessor.h \
    ../../include/server/controllers/controller_messagecollector_encryptionprocessor.h \
    ../../include/server/controllers/controller_messagecollector_decryptionprocessor.h \
    ../../include/server/controllers/controller_gui_tcpchannel.h \
    ../../include/server/controllers/controller_gui_messageprocessor.h \
    ../../include/server/controllers/controller_gui_messagecollector.h \
    ../../include/server/controllers/controller_gui_encryptionprocessor.h \
    ../../include/server/controllers/controller_gui_decryptionprocessor.h \
    ../../include/server/controllers/controller_gui_database.h \
    ../../include/server/controllers/controller_encryptionprocessor_threadworker.h \
    ../../include/server/controllers/controller_decryptionprocessor_threadworker.h \
    ../../include/server/interfaces/interface_thread.h \
    ../../include/server/interfaces/interface_tcpchannel.h \
    ../../include/server/interfaces/interface_communication.h \
    ../../include/server/interfaces/interface_communication_controller.h

FORMS    += ../../forms/server/mainwindow.ui

unix|win32: LIBS += -L$$PWD/../../lib/crypto++/installed/lib/ -lcryptopp

INCLUDEPATH += $$PWD/../../lib/crypto++/installed/include
DEPENDPATH += $$PWD/../../lib/crypto++/installed/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../lib/crypto++/installed/lib/cryptopp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../lib/crypto++/installed/lib/libcryptopp.a
