/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../../include/server/controllers/controller_gui_messageprocessor.h"
#include "../../../include/server/login_server/mainwindow.h"
#include "../../../include/server/modules/message_processor.h"
#include "../../../include/server/impulse.h"
#include "../../include/server/login_server/server_logger.h"


void Controller_GUI_MessageProcessor::CheckModule1Events(void *module1, void *module2)
{
    MainWindow *mainWindow = (MainWindow *)module1;
    MessageProcessor *eventGiver = (MessageProcessor *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::ResponseStartMessageProcessor:
            LOG_INFO("MessageProcessor Started.");
            mainWindow->setChildEnabled(ChildController::EnabledFlag::MessageProcessor, true);
            if (mainWindow->isAllChildsEnabled())
            {
                mainWindow->enableGui();
            }
            deleteAndNext = true;
            break;
        case eSystemEvent::ResponseSleepMessageProcessor:
            LOG_INFO("MessageProcessor Stopped.");
            mainWindow->setChildEnabled(ChildController::EnabledFlag::MessageProcessor, false);
            if (mainWindow->isAllChildsDisabled())
            {
                mainWindow->enableGui();
            }
            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorMessageProcessor:
            LOG_ERROR("MessageProcessor Error: " + ((ImpulseError *)i)->getError());
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

void Controller_GUI_MessageProcessor::CheckModule2Events(void *module1, void *module2)
{
    MessageProcessor *messageProcessor = (MessageProcessor *)module1;
    MainWindow *eventGiver = (MainWindow *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::RequestStartMessageProcessor:
            messageProcessor->RequestStart();
            deleteAndNext = true;
            break;
        case eSystemEvent::RequestSleepMessageProcessor:
            messageProcessor->RequestSleep();
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

void Controller_GUI_MessageProcessor::setMainWindowObj(MainWindow * module)
{
    setModule1Obj(module);
}

void Controller_GUI_MessageProcessor::setMessageProcessorObj(MessageProcessor * module)
{
    setModule2Obj(module);
}
