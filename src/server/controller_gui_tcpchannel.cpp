/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/controller_gui_tcpchannel.h"
#include "../../include/server/mainwindow.h"
#include "../../include/server/tcpchannel.h"
#include "../../include/server/impulse.h"
#include "../../include/server/slog.h"

void Controller_GUI_tcpChannel::CheckModule1Events(void *module1, void * module2)
{
    MainWindow *mainWindow = (MainWindow *)module1;
    TcpChannel *eventGiver = (TcpChannel *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::ResponseStartTcpChannel:
            SLog::logInfo() << "tcpChannel Started.";
            mainWindow->setChildEnabled(ChildController::EnabledFlag::TCP_Channel, true);
            if (mainWindow->isAllChildsEnabled())
            {
                mainWindow->enableGui();
            }
            deleteAndNext = true;
            break;
        case eSystemEvent::ResponseSleepTcpChannel:
            SLog::logInfo() << "tcpChannel Stopped.";
            mainWindow->setChildEnabled(ChildController::EnabledFlag::TCP_Channel, false);
            if (mainWindow->isAllChildsDisabled())
            {
                mainWindow->enableGui();
            }
            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorTcpChannel:
            SLog::logError() << "tcpChannel Error: " + ((ImpulseError*)i)->getError();
            deleteAndNext = true;
            break;
        case eSystemEvent::Undefined:
            SLog::logError() << "Got Undefined event!";
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
    TcpChannel *tcpChannel = (TcpChannel *)module1;
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
        case eSystemEvent::ErrorGui:
            SLog::logError() << "Gui Error: " << ((ImpulseError*)i)->getError();
            deleteAndNext = true;
            break;
        case eSystemEvent::Undefined:
            SLog::logError() << "Got Undefined event!";
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
