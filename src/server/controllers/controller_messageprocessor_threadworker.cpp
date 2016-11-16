#include <sstream>

#include "../../../include/server/controllers/controller_messageprocessor_threadworker.h"
#include "../../../include/server/modules/message_processor.h"
#include "../../../include/server/thread_worker.h"
#include "../../include/server/login_server/server_logger.h"


/**
\see interface_communication_controller.h
*/
void Controller_MessageProcessor_ThreadWorker::CheckModule1Events(void *module1, void *module2)
{
    //MessageProcessor *messageProcessor = (MessageProcessor *)module1;
    ThreadWorker *eventGiver = (ThreadWorker *)module2;
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
            LOG_INFO("MessageWorker is started. id: " + ss.str());
            deleteAndNext = true;
            break;
        case eSystemEvent::ResponseSleepMessageWorker:
            ss << ((ImpulseSignal *)i)->getThreadID();
            LOG_INFO("MessageWorker is sleeped. id: " + ss.str());
            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorMessageWorker:
            LOG_ERROR("MessageWorker Error: " + ((ImpulseError *)i)->getError());
            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorExecutionTask:
            LOG_ERROR("Got MessageProcessor_ThreadWorker error:" + ((ImpulseError *)i)->getError());
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

/**
\see interface_communication_controller.h
*/
void Controller_MessageProcessor_ThreadWorker::CheckModule2Events(void *module1, void *module2)
{
    ThreadWorker *messageWorker = (ThreadWorker *)module1;
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

void Controller_MessageProcessor_ThreadWorker::setMessageProcessorObj(MessageProcessor * module)
{
    setModule1Obj(module);
}

void Controller_MessageProcessor_ThreadWorker::setThreadWorkerObj(ThreadWorker * module)
{
    setModule2Obj(module);
}
