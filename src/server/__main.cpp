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
#include "../../include/server/database.h"
#include "../../include/server/epoll_tcpchannel.h"

#include "../../include/server/message_processor.h"
#include "../../include/server/crypto_processor.h"
#include "../../include/server/message_collector.h"
#include "../../include/server/slog.h"
#include "../../include/shared/user.h"
#include "../../include/shared/crypto.h"
#include "../../include/server/controllers/controller_gui_messagecollector.h"
#include "../../include/server/controllers/controller_gui_messageprocessor.h"
#include "../../include/server/controllers/controller_gui_tcpchannel.h"
#include "../../include/server/controllers/controller_messagecollector_messageprocessor.h"
#include "../../include/server/controllers/controller_messageprocessor_messageworker.h"
#include "../../include/shared/configuration.h"

void signal_callback_handler(int signum)
{
    printf("Caught signal %d\n",signum);
    // Cleanup and close up stuff here
    // Terminate program
    exit(signum);
}

void addUsersTest()
{
    //DataBase::GetDataBase().addUser(std::make_shared<User>(1, "login1", "password"));
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    // Register signal and signal handler
    signal(SIGINT, signal_callback_handler);

    QApplication a(argc, argv);

    //TODO Generation of keys()setting up

    Controller_GUI_MessageCollector cgmc;
    Controller_GUI_MessageProcessor cgmp;
    Controller_GUI_tcpChannel cgtc;
    //Controller_MessageCollector_MessageProcessor cmcmp;
    //Controller_tcpChannel_MessageCollector ctcms;

    MainWindow w;

    //create MessageProcessor
    ThreadConfiguration conf1;
    conf1.loopSleepTime = 10;
    conf1.unsleepReactionTime = 1000;
    conf1.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf1.responseSleepEvent = eSystemEvent::ResponseSleepMessageProcessor;
    conf1.responseStartEvent = eSystemEvent::ResponseStartMessageProcessor;
    MessageProcessor mp(conf1, 10);

    //create MessageCollector
    ThreadConfiguration conf2;
    conf2.loopSleepTime = 10;
    conf2.unsleepReactionTime = 1000;
    conf2.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf2.responseSleepEvent = eSystemEvent::ResponseSleepMessageCollector;
    conf2.responseStartEvent = eSystemEvent::ResponseStartMessageCollector;
    MessageCollector collector(conf2);

    //create EpollTCPChannel
    ThreadConfiguration conf3;
    conf3.loopSleepTime = 0;
    conf3.unsleepReactionTime = 1000;
    conf3.sleepLoopMode = ThreadConfiguration::doNotSleepInsideLoop;
    conf3.responseSleepEvent = eSystemEvent::ResponseSleepTcpChannel;
    conf3.responseStartEvent = eSystemEvent::ResponseStartTcpChannel;
    EpollTCPChannel server(conf3, globalConfiguration.serverPort, 64);

    //create DataBase
    ThreadConfiguration conf4;
    conf4.loopSleepTime = 10;
    conf4.unsleepReactionTime = 1000;
    conf4.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf4.responseSleepEvent = eSystemEvent::ResponseSleepDatabase;
    conf4.responseStartEvent = eSystemEvent::ResponseStartDatabase;
    DataBase db(conf4);

    //create CryptoProcessor
    ThreadConfiguration conf5;
    conf5.loopSleepTime = 10;
    conf5.unsleepReactionTime = 1000;
    conf5.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf5.responseSleepEvent = eSystemEvent::ResponseSleepEncryptionProcessor;
    conf5.responseStartEvent = eSystemEvent::ResponseStartEncryptionProcessor;
    EncryptionProcessor encr(conf5);

    ThreadConfiguration conf6;
    conf6.loopSleepTime = 10;
    conf6.unsleepReactionTime = 1000;
    conf6.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf6.responseSleepEvent = eSystemEvent::ResponseSleepDecryptionProcessor;
    conf6.responseStartEvent = eSystemEvent::ResponseStartDecryptionProcessor;
    EncryptionProcessor decr(conf6);
    Sha256Hasher hasher;
    Cipher rsaCipher;
    server.setCrypto(&hasher, &rsaCipher);

    SLog::logInfo().setGuiLevel(&w);

    cgmc.setModule1Obj(&w);
    cgmc.setModule2Obj(&collector);

    cgmp.setModule1Obj(&w);
    cgmp.setModule2Obj(&mp);

    cgtc.setModule1Obj(&w);
    cgtc.setModule2Obj(&server);

    cmcmp.setModule1Obj(&collector);
    cmcmp.setModule2Obj(&mp);

    ctcms.setModule1Obj(&server);
    ctcms.setModule2Obj(&collector);

    w.AddEventController(&cgmc);
    w.AddEventController(&cgmp);
    w.AddEventController(&cgtc);

    mp.AddEventController(&cgmp);
    mp.AddEventController(&cmcmp);

    collector.AddEventController(&cgmc);
    collector.AddEventController(&cmcmp);
    collector.AddEventController(&ctcms);

    server.AddEventController(&cgtc);
    server.AddEventController(&ctcms);

    w.show();

    int i = a.exec();

    DataBase::ReleaseResources();

    return i;
}
