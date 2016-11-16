/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../../include/server/controllers/controller_gui_tcpchannel.h"
#include "../../../include/server/login_server/mainwindow.h"
#include "../../../include/server/interfaces/interface_tcpchannel.h"
#include "../../../include/server/impulse.h"
#include "../../include/server/login_server/server_logger.h"


void Controller_GUI_tcpChannel::CheckModule1Events(void *module1, void * module2)
{
    MainWindow *mainWindow = (MainWindow *)module1;
    InterfaceTcpChannel *eventGiver = (InterfaceTcpChannel *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::ResponseStartTcpChannel:
            LOG_INFO("tcpChannel Started.");
            mainWindow->setChildEnabled(ChildController::EnabledFlag::TCP_Channel, true);
            if (mainWindow->isAllChildsEnabled())
            {
                mainWindow->enableGui();
            }
            deleteAndNext = true;
            break;
        case eSystemEvent::ResponseSleepTcpChannel:
            LOG_INFO("tcpChannel Stopped.");
            mainWindow->setChildEnabled(ChildController::EnabledFlag::TCP_Channel, false);
            if (mainWindow->isAllChildsDisabled())
            {
                mainWindow->enableGui();
            }
            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorTcpChannel:
            LOG_ERROR("tcpChannel Error: " + ((ImpulseError*)i)->getError());
            deleteAndNext = true;
            break;
        case eSystemEvent::Undefined:
            LOG_ERROR("Got Undefined event!");
            deleteAndNext = true;
            break;
        default:
            break;
        }

        if (deleteAndNext)
        {
            todelete = i;
            i = eventGiver->eraseImpulseAndGetNext(i);
            delete todelete;
            deleteAndNext = false;
        }
        else
        {
            i = eventGiver->getNextImpulse(i);
        }
    }
}

void Controller_GUI_tcpChannel::CheckModule2Events(void *module1, void *module2)
{
    InterfaceTcpChannel *tcpChannel = (InterfaceTcpChannel *)module1;
    MainWindow *eventGiver = (MainWindow *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::RequestStartTcpChannel:
            tcpChannel->RequestStart();
            deleteAndNext = true;
            break;
        case eSystemEvent::RequestSleepTcpChannel:
            tcpChannel->RequestStop();
            deleteAndNext = true;
            break;
        case eSystemEvent::Undefined:
            LOG_ERROR("Got Undefined event!");
            deleteAndNext = true;
            break;
        default:
            break;
        }

        if (deleteAndNext)
        {
            todelete = i;
            i = eventGiver->eraseImpulseAndGetNext(i);
            delete todelete;
            deleteAndNext = false;
        }
        else
        {
            i = eventGiver->getNextImpulse(i);
        }
    }
}

void Controller_GUI_tcpChannel::setMainWindowObj(MainWindow * module)
{
    setModule1Obj(module);
}

void Controller_GUI_tcpChannel::setTCPChannelObj(InterfaceTcpChannel * module)
{
    setModule2Obj(module);
}
