#include <sstream>

#include "../../include/server/controller_messageprocessor_messageworker.h"
#include "../../include/server/message_processor.h"
#include "../../include/server/message_worker.h"
#include "../../include/server/slog.h"

/**
\see interface_communication_controller.h
*/
void Controller_MessageProcessor_MessageWorker::CheckModule1Events(void *module1, void *module2)
{
    //MessageProcessor *messageProcessor = (MessageProcessor *)module1;
    MessageWorker *eventGiver = (MessageWorker *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        std::ostringstream ss;
        switch (i->getEvent())
        {
        case eSystemEvent::ResponseStartMessageWorker:
            ss << ((ImpulseSignal *)i)->getThreadID();
            SLog::logInfo() << "MessageWorker is started. id: " + ss.str();
            deleteAndNext = true;
            break;
        case eSystemEvent::ResponseSleepMessageWorker:
            ss << ((ImpulseSignal *)i)->getThreadID();
            SLog::logInfo() << "MessageWorker is sleeped. id: " + ss.str();
            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorMessageWorker:
            SLog::logError() << "MessageWorker Error: " + ((ImpulseError *)i)->getError();
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

/**
\see interface_communication_controller.h
*/
void Controller_MessageProcessor_MessageWorker::CheckModule2Events(void *module1, void *module2)
{
    MessageWorker *messageWorker = (MessageWorker *)module1;
    MessageProcessor *eventGiver = (MessageProcessor *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);

    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::RequestSleepMessageWorker:
            if (((ImpulseSignal *)i)->getThreadID() != messageWorker->getThreadID())
            {
                break;
            }
            messageWorker->RequestSleep();

            deleteAndNext = true;
            break;
        case eSystemEvent::RequestStartMessageWorker:
            if (((ImpulseSignal *)i)->getThreadID() != messageWorker->getThreadID())
            {
                break;
            }
            messageWorker->RequestStart();

            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorMessageProcessor:
            SLog::logError() << "MessageProcessor Error: " << ((ImpulseError *)i)->getError();
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
