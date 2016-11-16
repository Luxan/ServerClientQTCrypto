/**
\author Sergey Gorokh (ESEGORO)
*/
#include <QApplication>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include "../../include/server/mainwindow.h"
#include "../../include/server/modules/database.h"
#include "../../include/server/modules/epoll_tcpchannel.h"

#include "../../include/server/modules/message_processor.h"
#include "../../include/server/modules/crypto_processor.h"
#include "../../include/server/modules/message_collector.h"
#include "../../include/server/slog.h"
#include "../../include/shared/user.h"
#include "../../include/shared/crypto/crypto.h"
#include "../../include/shared/crypto/certificate.h"
#include "../../include/server/controllers/controller_gui_messagecollector.h"
#include "../../include/server/controllers/controller_gui_messageprocessor.h"
#include "../../include/server/controllers/controller_gui_tcpchannel.h"
#include "../../include/server/controllers/controller_gui_database.h"
#include "../../include/server/controllers/controller_gui_decryptionprocessor.h"
#include "../../include/server/controllers/controller_gui_encryptionprocessor.h"
#include "../../include/server/controllers/controller_messagecollector_messageprocessor.h"
#include "../../include/server/controllers/controller_messagecollector_decryptionprocessor.h"
#include "../../include/server/controllers/controller_messagecollector_encryptionprocessor.h"
#include "../../include/server/controllers/controller_messageprocessor_database.h"
#include "../../include/server/controllers/controller_tcpchannel_encryptionprocessor.h"
#include "../../include/server/controllers/controller_tcpchannel_decryptionprocessor.h"
#include "../../include/server/controllers/controller_decryptionprocessor_threadworker.h"
#include "../../include/server/controllers/controller_encryptionprocessor_threadworker.h"
#include "../../include/server/controllers/controller_messageprocessor_threadworker.h"
#include "../../include/shared/configuration.h"

uint8_t googlecert[] = {
0x30,0x82,0x03,0x21,0x30,0x82,0x02,0x8A,0xA0,0x03,0x02,0x01,0x02,0x02,0x10,0x4B,
0xA5,0xAE,0x59,0xDE,0xDD,0x1C,0xC7,0x80,0x7C,0x89,0x22,0x91,0xF0,0xE2,0x43,0x30,
0x0D,0x06,0x09,0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x04,0x05,0x00,0x30,0x4C,
0x31,0x0B,0x30,0x09,0x06,0x03,0x55,0x04,0x06,0x13,0x02,0x5A,0x41,0x31,0x25,0x30,
0x23,0x06,0x03,0x55,0x04,0x0A,0x13,0x1C,0x54,0x68,0x61,0x77,0x74,0x65,0x20,0x43,
0x6F,0x6E,0x73,0x75,0x6C,0x74,0x69,0x6E,0x67,0x20,0x28,0x50,0x74,0x79,0x29,0x20,
0x4C,0x74,0x64,0x2E,0x31,0x16,0x30,0x14,0x06,0x03,0x55,0x04,0x03,0x13,0x0D,0x54,
0x68,0x61,0x77,0x74,0x65,0x20,0x53,0x47,0x43,0x20,0x43,0x41,0x30,0x1E,0x17,0x0D,
0x30,0x36,0x30,0x35,0x31,0x35,0x32,0x33,0x31,0x38,0x31,0x31,0x5A,0x17,0x0D,0x30,
0x37,0x30,0x35,0x31,0x35,0x32,0x33,0x31,0x38,0x31,0x31,0x5A,0x30,0x68,0x31,0x0B,
0x30,0x09,0x06,0x03,0x55,0x04,0x06,0x13,0x02,0x55,0x53,0x31,0x13,0x30,0x11,0x06,
0x03,0x55,0x04,0x08,0x13,0x0A,0x43,0x61,0x6C,0x69,0x66,0x6F,0x72,0x6E,0x69,0x61,
0x31,0x16,0x30,0x14,0x06,0x03,0x55,0x04,0x07,0x13,0x0D,0x4D,0x6F,0x75,0x6E,0x74,
0x61,0x69,0x6E,0x20,0x56,0x69,0x65,0x77,0x31,0x13,0x30,0x11,0x06,0x03,0x55,0x04,
0x0A,0x13,0x0A,0x47,0x6F,0x6F,0x67,0x6C,0x65,0x20,0x49,0x6E,0x63,0x31,0x17,0x30,
0x15,0x06,0x03,0x55,0x04,0x03,0x13,0x0E,0x77,0x77,0x77,0x2E,0x67,0x6F,0x6F,0x67,
0x6C,0x65,0x2E,0x63,0x6F,0x6D,0x30,0x81,0x9F,0x30,0x0D,0x06,0x09,0x2A,0x86,0x48,
0x86,0xF7,0x0D,0x01,0x01,0x01,0x05,0x00,0x03,0x81,0x8D,0x00,0x30,0x81,0x89,0x02,
0x81,0x81,0x00,0xE6,0xC5,0xC6,0x8D,0xCD,0x0B,0xA3,0x03,0x04,0xDC,0xAE,0xCC,0xC9,
0x46,0xBE,0xBD,0xCC,0x9D,0xBC,0x73,0x34,0x48,0xFE,0xD3,0x75,0x64,0xD0,0xC9,0xC9,
0x76,0x27,0x72,0x0F,0xA9,0x96,0x1A,0x3B,0x81,0xF3,0x14,0xF6,0xAE,0x90,0x56,0xE7,
0x19,0xD2,0x73,0x68,0xA7,0x85,0xA4,0xAE,0xCA,0x24,0x14,0x30,0x00,0xBA,0xE8,0x36,
0x5D,0x81,0x73,0x3A,0x71,0x05,0x8F,0xB1,0xAF,0x11,0x87,0xDA,0x5C,0xF1,0x3E,0xBF,
0x53,0x51,0x84,0x6F,0x44,0x0E,0xB7,0xE8,0x26,0xD7,0x2F,0xB2,0x6F,0xF2,0xF2,0x5D,
0xDF,0xA7,0xCF,0x8C,0xA5,0xE9,0x1E,0x6F,0x30,0x48,0x94,0x21,0x0B,0x01,0xAD,0xBA,
0x0E,0x71,0x01,0x0D,0x10,0xEF,0xBF,0xEE,0x2C,0xD3,0x8D,0xFE,0x54,0xA8,0xFE,0xD3,
0x97,0x8F,0xCB,0x02,0x03,0x01,0x00,0x01,0xA3,0x81,0xE7,0x30,0x81,0xE4,0x30,0x28,
0x06,0x03,0x55,0x1D,0x25,0x04,0x21,0x30,0x1F,0x06,0x08,0x2B,0x06,0x01,0x05,0x05,
0x07,0x03,0x01,0x06,0x08,0x2B,0x06,0x01,0x05,0x05,0x07,0x03,0x02,0x06,0x09,0x60,
0x86,0x48,0x01,0x86,0xF8,0x42,0x04,0x01,0x30,0x36,0x06,0x03,0x55,0x1D,0x1F,0x04,
0x2F,0x30,0x2D,0x30,0x2B,0xA0,0x29,0xA0,0x27,0x86,0x25,0x68,0x74,0x74,0x70,0x3A,
0x2F,0x2F,0x63,0x72,0x6C,0x2E,0x74,0x68,0x61,0x77,0x74,0x65,0x2E,0x63,0x6F,0x6D,
0x2F,0x54,0x68,0x61,0x77,0x74,0x65,0x53,0x47,0x43,0x43,0x41,0x2E,0x63,0x72,0x6C,
0x30,0x72,0x06,0x08,0x2B,0x06,0x01,0x05,0x05,0x07,0x01,0x01,0x04,0x66,0x30,0x64,
0x30,0x22,0x06,0x08,0x2B,0x06,0x01,0x05,0x05,0x07,0x30,0x01,0x86,0x16,0x68,0x74,
0x74,0x70,0x3A,0x2F,0x2F,0x6F,0x63,0x73,0x70,0x2E,0x74,0x68,0x61,0x77,0x74,0x65,
0x2E,0x63,0x6F,0x6D,0x30,0x3E,0x06,0x08,0x2B,0x06,0x01,0x05,0x05,0x07,0x30,0x02,
0x86,0x32,0x68,0x74,0x74,0x70,0x3A,0x2F,0x2F,0x77,0x77,0x77,0x2E,0x74,0x68,0x61,
0x77,0x74,0x65,0x2E,0x63,0x6F,0x6D,0x2F,0x72,0x65,0x70,0x6F,0x73,0x69,0x74,0x6F,
0x72,0x79,0x2F,0x54,0x68,0x61,0x77,0x74,0x65,0x5F,0x53,0x47,0x43,0x5F,0x43,0x41,
0x2E,0x63,0x72,0x74,0x30,0x0C,0x06,0x03,0x55,0x1D,0x13,0x01,0x01,0xFF,0x04,0x02,
0x30,0x00,0x30,0x0D,0x06,0x09,0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x04,0x05,
0x00,0x03,0x81,0x81,0x00,0x57,0x4B,0xBC,0xA4,0x43,0xE7,0xE0,0x01,0x92,0xA0,0x96,
0x35,0xF9,0x18,0x08,0x88,0x1D,0x7B,0x70,0x19,0x8F,0xF9,0x36,0xB2,0x05,0x3A,0x05,
0xCA,0x14,0x59,0x4D,0x24,0x0E,0xE5,0x8A,0xAF,0x4E,0x87,0x5A,0xF7,0x1C,0x2A,0x96,
0x8F,0xCB,0x61,0x40,0x9E,0xD2,0xB4,0x38,0x40,0x21,0x24,0xC1,0x4F,0x1F,0xCB,0x13,
0x4A,0x8F,0x95,0x02,0xDF,0x91,0x3D,0xD6,0x40,0xEB,0x11,0x6F,0x9B,0x10,0xA1,0x6F,
0xCE,0x91,0x5E,0x30,0xF6,0x6D,0x13,0x5E,0x15,0xA4,0x2E,0xC2,0x18,0x9E,0x00,0xC3,
0xD8,0x32,0x67,0x47,0xFC,0xB8,0x1E,0x9A,0xD9,0x9A,0x8E,0xCC,0xFF,0x7C,0x12,0xB7,
0x03,0xBF,0x52,0x20,0xCF,0x21,0xF4,0xF3,0x77,0xDD,0x12,0x15,0xF0,0x94,0xFA,0x90,
0xD5,0xE3,0x59,0x68,0x81
};

namespace controllers
{
//controller gui - module
Controller_GUI_MessageCollector cgmc;
Controller_GUI_MessageProcessor cgmp;
Controller_GUI_tcpChannel cgtc;
Controller_GUI_Database cgd;
Controller_GUI_DecryptionProcessor cgdp;
Controller_GUI_EncryptionProcessor cgep;

//controllers module module
Controller_MessageCollector_MessageProcessor cmcmp;
Controller_MessageCollector_DecryptionProcessor cmcdp;
Controller_MessageCollector_EncryptionProcessor cmcep;
Controller_MessageProcessor_Database cmpd;
Controller_TCPChannel_DecryptionProcessor ctcdp;
Controller_TCPChannel_EncryptionProcessor ctcep;

//controller module worker thread
Controller_MessageProcessor_ThreadWorker mpth;
Controller_EncryptionProcessor_ThreadWorker enth;
Controller_DecryptionProcessor_ThreadWorker deth;

void initializeControllers();

}

namespace modules
{
MessageProcessor *mp;
MessageCollector *collector;
EpollTCPChannel *server;
DataBase *db;
EncryptionProcessor *encr;
DecryptionProcessor *decr;
MainWindow *w;

void zero()
{
    mp = nullptr;
    collector = nullptr;
    server = nullptr;
    db = nullptr;
    encr = nullptr;
    decr = nullptr;
    w = nullptr;
}

void initializeMessageProcessor()
{
    ThreadConfiguration conf1;
    conf1.loopSleepTime = 10;
    conf1.unsleepReactionTime = 1001;
    conf1.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf1.responseSleepEvent = eSystemEvent::ResponseSleepMessageProcessor;
    conf1.responseStartEvent = eSystemEvent::ResponseStartMessageProcessor;
    modules::mp = new MessageProcessor(conf1, &controllers::mpth, 5);
    modules::mp->startMainLoop();

    modules::mp->AddEventController(&controllers::cgmp);
    modules::mp->AddEventController(&controllers::cmcmp);
    modules::mp->AddEventController(&controllers::cmpd);
}

void initializeMessageCollector()
{
    //create MessageCollector
    ThreadConfiguration conf2;
    conf2.loopSleepTime = 10;
    conf2.unsleepReactionTime = 1002;
    conf2.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf2.responseSleepEvent = eSystemEvent::ResponseSleepMessageCollector;
    conf2.responseStartEvent = eSystemEvent::ResponseStartMessageCollector;
    modules::collector = new MessageCollector(conf2);
    modules::collector->startMainLoop();

    modules::collector->AddEventController(&controllers::cgmc);
    modules::collector->AddEventController(&controllers::cmcmp);
    modules::collector->AddEventController(&controllers::cmcdp);
    modules::collector->AddEventController(&controllers::cmcep);
}

void initializeEpollTCPChannel(Certificate * certificate)
{
    //create EpollTCPChannel
    ThreadConfiguration conf3;
    conf3.loopSleepTime = 0;
    conf3.unsleepReactionTime = 1003;
    conf3.sleepLoopMode = ThreadConfiguration::doNotSleepInsideLoop;
    conf3.responseSleepEvent = eSystemEvent::ResponseSleepTcpChannel;
    conf3.responseStartEvent = eSystemEvent::ResponseStartTcpChannel;
    modules::server = new EpollTCPChannel(conf3, globalConfiguration.serverPort, 64, certificate);
    modules::server->startMainLoop();

    modules::server->AddEventController(&controllers::cgtc);
    modules::server->AddEventController(&controllers::ctcdp);
    modules::server->AddEventController(&controllers::ctcep);
}

void initializeDataBase()
{
    //create DataBase
    ThreadConfiguration conf4;
    conf4.loopSleepTime = 10;
    conf4.unsleepReactionTime = 1004;
    conf4.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf4.responseSleepEvent = eSystemEvent::ResponseSleepDatabase;
    conf4.responseStartEvent = eSystemEvent::ResponseStartDatabase;
    modules::db = new DataBase(conf4, "user.db");
    modules::db->startMainLoop();

    modules::db->AddEventController(&controllers::cgd);
    modules::db->AddEventController(&controllers::cmpd);
}

void initializeEncryptionProcessor()
{
    //create CryptoProcessor
    ThreadConfiguration conf5;
    conf5.loopSleepTime = 10;
    conf5.unsleepReactionTime = 1005;
    conf5.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf5.responseSleepEvent = eSystemEvent::ResponseSleepEncryptionProcessor;
    conf5.responseStartEvent = eSystemEvent::ResponseStartEncryptionProcessor;
    modules::encr = new EncryptionProcessor(conf5, &controllers::enth, 5);
    modules::encr->startMainLoop();

    modules::encr->AddEventController(&controllers::cgep);
    modules::encr->AddEventController(&controllers::cmcep);
    modules::encr->AddEventController(&controllers::ctcep);
}

void initializeDecryptionProcessor()
{
    ThreadConfiguration conf6;
    conf6.loopSleepTime = 10;
    conf6.unsleepReactionTime = 1006;
    conf6.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf6.responseSleepEvent = eSystemEvent::ResponseSleepDecryptionProcessor;
    conf6.responseStartEvent = eSystemEvent::ResponseStartDecryptionProcessor;
    modules::decr = new DecryptionProcessor(conf6, &controllers::deth, 5);
    modules::decr->startMainLoop();

    modules::decr->AddEventController(&controllers::cmcdp);
    modules::decr->AddEventController(&controllers::ctcdp);
    modules::decr->AddEventController(&controllers::cgdp);

}

void initializeMainWindow()
{
    modules::w = new MainWindow();
    //set controllers to modules
    modules::w->AddEventController(&controllers::cgmc);
    modules::w->AddEventController(&controllers::cgmp);
    modules::w->AddEventController(&controllers::cgtc);
    modules::w->AddEventController(&controllers::cgd);
    modules::w->AddEventController(&controllers::cgdp);
    modules::w->AddEventController(&controllers::cgep);

    modules::w->show();
}

void deinitializeAll()
{
    if (mp != nullptr)
    {
        mp->terminate();
    }
    if (collector != nullptr)
    {
        collector->terminate();
    }
    if (server != nullptr)
    {
        server->terminate();
    }
    if (db != nullptr)
    {
        db->terminate();
    }
    if (encr != nullptr)
    {
        encr->terminate();
    }
    if (decr != nullptr)
    {
        decr->terminate();
    }

    if (mp != nullptr)
    {
        delete mp;
    }
    if (collector != nullptr)
    {
        delete collector;
    }
    if (server != nullptr)
    {
        delete server;
    }
    if (db != nullptr)
    {
        delete db;
    }
    if (encr != nullptr)
    {
        delete encr;
    }
    if (decr != nullptr)
    {
        delete decr;
    }
    if (w != nullptr)
    {
        delete w;
    }
}

}

void controllers::initializeControllers()
{
    std::cout << "Cleaning all threads. Please Wait..." << std::endl;
    //controller gui - module
    if (modules::w != nullptr)
        cgmc.setMainWindowObj(modules::w);
    if (modules::collector != nullptr)
        cgmc.setMessageCollectorObj(modules::collector);

    if (modules::w != nullptr)
        cgmp.setMainWindowObj(modules::w);
    if (modules::mp != nullptr)
        cgmp.setMessageProcessorObj(modules::mp);

    if (modules::w != nullptr)
        cgtc.setMainWindowObj(modules::w);
    if (modules::server != nullptr)
        cgtc.setTCPChannelObj(modules::server);

    if (modules::w != nullptr)
        cgd.setMainWindowObj(modules::w);
    if (modules::db != nullptr)
        cgd.setDataBaseObj(modules::db);

    if (modules::w != nullptr)
        cgdp.setMainWindowObj(modules::w);
    if (modules::decr != nullptr)
        cgdp.setDecryptionProcessorObj(modules::decr);

    if (modules::w != nullptr)
        cgep.setMainWindowObj(modules::w);
    if (modules::encr != nullptr)
        cgep.setEncryptionProcessorObj(modules::encr);

    //controller module - module
    if (modules::collector != nullptr)
        cmcmp.setMessageCollectorObj(modules::collector);
    if (modules::mp != nullptr)
        cmcmp.setMessageProcessorObj(modules::mp);

    if (modules::collector != nullptr)
        cmcdp.setMessageCollectorObj(modules::collector);
    if (modules::decr != nullptr)
        cmcdp.setDecryptionProcessorObj(modules::decr);

    if (modules::collector != nullptr)
        cmcep.setMessageCollectorObj(modules::collector);
    if (modules::encr != nullptr)
        cmcep.setEncryptionProcessorObj(modules::encr);

    if (modules::mp != nullptr)
        cmpd.setMessageProcessorObj(modules::mp);
    if (modules::db != nullptr)
        cmpd.setDataBaseObj(modules::db);

    if (modules::server != nullptr)
        ctcdp.setTCPChannelObj(modules::server);
    if (modules::decr != nullptr)
        ctcdp.setDecryptionProcessorObj(modules::decr);

    if (modules::server != nullptr)
        ctcep.setTCPChannelObj(modules::server);
    if (modules::encr != nullptr)
        ctcep.setEncryptionProcessorObj(modules::encr);
}

void signal_callback_handler(int signum)
{
    modules::deinitializeAll();

    exit(signum);
}

void addUsersTest()
{
    //DataBase::GetDataBase().addUser(std::make_shared<User>(1, "login1", "password"));
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    modules::zero();
    // Register signal and signal handler
    QApplication a(argc, argv);

    Certificate certificate(new Buffer(googlecert, sizeof(googlecert)));

    //modules
    modules::initializeMainWindow();


    SLog::logInfo().setGuiLevel(modules::w);

    if (signal(SIGHUP, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGHUP";
    if (signal(SIGINT, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGINT";
    if (signal(SIGQUIT, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGQUIT";
    if (signal(SIGILL, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGILL";
    if (signal(SIGTRAP, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGTRAP";
    if (signal(SIGABRT, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGABRT";
    if (signal(SIGIOT, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGIOT";
    if (signal(SIGBUS, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGBUS";
    if (signal(SIGFPE, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGFPE";
    if (signal(SIGKILL, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGKILL";
    if (signal(SIGSEGV, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGSEGV";
    if (signal(SIGTERM, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGTERM";
    if (signal(SIGSTKFLT, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGSTKFLT";
    if (signal(SIGTSTP, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGTSTP";
    if (signal(SIGPWR, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGPWR";
    if (signal(SIGSYS, signal_callback_handler) == SIG_ERR)
            SLog::logWarn() << " Can't catch SIGSYS";

    modules::initializeDecryptionProcessor();
    modules::initializeEncryptionProcessor();
    modules::initializeMessageProcessor();
    modules::initializeMessageCollector();
    modules::initializeDataBase();
    modules::initializeEpollTCPChannel(&certificate);
    controllers::initializeControllers();

    int i = a.exec();
    modules::deinitializeAll();
    return i;
}
