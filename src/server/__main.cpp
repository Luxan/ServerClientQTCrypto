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
#include "../../include/server/tcpchannel.h"
#include "../../include/server/message_processor.h"
#include "../../include/server/message_collector.h"
#include "../../include/server/slog.h"
#include "../../include/shared/crypto.h"
#include "../../include/server/controller_gui_messagecollector.h"
#include "../../include/server/controller_gui_messageprocessor.h"
#include "../../include/server/controller_gui_tcpchannel.h"
#include "../../include/server/controller_messagecollector_messageprocessor.h"
#include "../../include/server/controller_tcpchannel_messagecollector.h"
#include "../../include/server/controller_messageprocessor_messageworker.h"
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
    DataBase::GetDataBase().addUser(std::make_shared<User>(1, "login1", "password"));
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    // Register signal and signal handler
    signal(SIGINT, signal_callback_handler);

    QApplication a(argc, argv);
    CryptographyBase::InitializeOpenSSL();

    DataBase::LoadResources("database.txt");
    addUsersTest(); //remove TODO
    //TODO Generation of keys()setting up

    Controller_GUI_MessageCollector cgmc;
    Controller_GUI_MessageProcessor cgmp;
    Controller_GUI_tcpChannel cgtc;
    //Controller_MessageCollector_MessageProcessor cmcmp;
    //Controller_tcpChannel_MessageCollector ctcms;

    MainWindow w;

    ThreadConfiguration conf1;
    conf1.loopSleepTime = 10;
    conf1.unsleepReactionTime = 1000;
    conf1.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf1.responseSleepEvent = eSystemEvent::ResponseSleepMessageProcessor;
    conf1.responseStartEvent = eSystemEvent::ResponseStartMessageProcessor;

    MessageProcessor mp(conf1, 10);

    ThreadConfiguration conf2;
    conf2.loopSleepTime = 10;
    conf2.unsleepReactionTime = 1000;
    conf2.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    conf2.responseSleepEvent = eSystemEvent::ResponseSleepMessageCollector;
    conf2.responseStartEvent = eSystemEvent::ResponseStartMessageCollector;
    MessageCollector collector(conf2);

    ThreadConfiguration conf3;
    conf3.loopSleepTime = 0;
    conf3.unsleepReactionTime = 1000;
    conf3.sleepLoopMode = ThreadConfiguration::doNotSleepInsideLoop;
    conf3.responseSleepEvent = eSystemEvent::ResponseSleepTcpChannel;
    conf3.responseStartEvent = eSystemEvent::ResponseStartTcpChannel;
    TcpChannel server(conf3, globalConfiguration::serverPort, 64);

    Sha256Hasher hasher;
    RSACipher rsaCipher;
    server.setCrypto(&hasher, &rsaCipher);

    SLog::logInfo().setGuiLevel(&w);

    cgmc.setModule1Obj(&w);
    cgmc.setModule2Obj(&collector);

    cgmp.setModule1Obj(&w);
    cgmp.setModule2Obj(&mp);

    cgtc.setModule1Obj(&w);
    cgtc.setModule2Obj(&server);

    //cmcmp.setModule1Obj(&collector);
    //cmcmp.setModule2Obj(&mp);

//    ctcms.setModule1Obj(&server);
//    ctcms.setModule2Obj(&collector);

    w.AddEventController(&cgmc);
    w.AddEventController(&cgmp);
    w.AddEventController(&cgtc);

    mp.AddEventController(&cgmp);
//    mp.AddEventController(&cmcmp);

    collector.AddEventController(&cgmc);
//    collector.AddEventController(&cmcmp);
//    collector.AddEventController(&ctcms);

    server.AddEventController(&cgtc);
//    server.AddEventController(&ctcms);

    w.show();

    int i = a.exec();

    DataBase::ReleaseResources();

    return i;
}
