#include "../../include/server/server_initializer.h"

void modules::zero()
{
    server = nullptr;
    w = nullptr;
}

void modules::initializeEpollTCPChannel(Buffer * certificate, Buffer * privateKey)
{
    //create EpollTCPChannel
    ThreadConfiguration conf3;
    conf3.loopSleepTime = 0;
    conf3.unsleepReactionTime = 1003;
    conf3.sleepLoopMode = ThreadConfiguration::doNotSleepInsideLoop;
    conf3.responseSleepEvent = eSystemEvent::ResponseSleepTcpChannel;
    conf3.responseStartEvent = eSystemEvent::ResponseStartTcpChannel;
    modules::server = new EpollChannel(conf3, globalConfiguration.serverPort, 64, certificate, privateKey);
    modules::server->startMainLoop();

    modules::server->AddEventController(&controllers::cgtc);
}

void modules::initializeMainWindow()
{
    modules::w = new MainWindow();
    //set controllers to modules
    modules::w->AddEventController(&controllers::cgtc);

    modules::w->show();
}

void modules::deinitializeAll()
{
    if (server != nullptr)
    {
        server->terminate();
        delete server;
    }
    if (w != nullptr)
    {
        delete w;
    }
}

void controllers::initializeControllers()
{
    std::cout << "Cleaning all threads. Please Wait..." << std::endl;

    if (modules::w != nullptr)
        cgtc.setMainWindowObj(modules::w);
    if (modules::server != nullptr)
        cgtc.setTCPChannelObj(modules::server);
}
