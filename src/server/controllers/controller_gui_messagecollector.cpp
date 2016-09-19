/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/controllers/controller_gui_messagecollector.h"
#include "../../include/server/mainwindow.h"
#include "../../include/server/modules/message_collector.h"
#include "../../include/server/impulse.h"
#include "../../include/server/slog.h"

void Controller_GUI_MessageCollector::CheckModule1Events(void *module1, void *module2)
{
    MainWindow *mainWindow = (MainWindow *)module1;
    MessageCollector *eventGiver = (MessageCollector *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::ResponseStartMessageCollector:
            SLog::logInfo() << "MessageCollector Started.";
            mainWindow->setChildEnabled(ChildController::EnabledFlag::MessageCollector, true);
            if (mainWindow->isAllChildsEnabled())
            {
                mainWindow->enableGui();
            }

            deleteAndNext = true;
            break;
        case eSystemEvent::ResponseSleepMessageCollector:
            SLog::logInfo() << "MessageCollector Stopped.";
            mainWindow->setChildEnabled(ChildController::EnabledFlag::MessageCollector, false);
            if (mainWindow->isAllChildsDisabled())
            {
                mainWindow->enableGui();
            }

            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorMessageCollector:
            SLog::logError() << "MessageCollector Error: " + ((ImpulseError *)i)->getError();
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

void Controller_GUI_MessageCollector::CheckModule2Events(void *module1, void * module2)
{
    MessageCollector *messageCollector = (MessageCollector *)module1;
    MainWindow *eventGiver = (MainWindow *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::RequestStartMessageCollector:
            messageCollector->RequestStart();
            deleteAndNext = true;
            break;
        case eSystemEvent::RequestSleepMessageCollector:
            messageCollector->RequestStop();
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

void Controller_GUI_MessageCollector::setMainWindowObj(MainWindow * module)
{
    setModule1Obj(module);
}

void Controller_GUI_MessageCollector::setMessageCollectorObj(MessageCollector * module)
{
    setModule2Obj(module);
}
