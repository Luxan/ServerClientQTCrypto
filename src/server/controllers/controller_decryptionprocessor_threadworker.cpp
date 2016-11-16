/**
\author Sergey Gorokh (ESEGORO)
*/

#include <sstream>

#include "../../../include/server/interfaces/interface_communication_controller.h"
#include "../../../include/server/controllers/controller_decryptionprocessor_threadworker.h"
#include "../../../include/server/thread_worker.h"
#include "../../../include/server/modules/crypto_processor.h"
#include "../../../include/server/impulse.h"
#include "../../include/server/login_server/server_logger.h"

void Controller_DecryptionProcessor_ThreadWorker::CheckModule1Events(void *module1, void *module2)
{
    //DecryptionProcessor *decryptionProcessor = (DecryptionProcessor *)module1;
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
        case eSystemEvent::ResponseStartDecryptionWorker:
            ss << ((ImpulseSignal *)i)->getThreadID();
            LOG_INFO("DecryptionWorker is started. id: " + ss.str());
            deleteAndNext = true;
            break;
        case eSystemEvent::ResponseSleepEncryptionWorker:
            ss << ((ImpulseSignal *)i)->getThreadID();
            LOG_INFO("DecryptionWorker is sleeped. id: " + ss.str());
            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorDecryptionWorker:
            LOG_ERROR("DecryptionWorker Error: " + ((ImpulseError *)i)->getError());
            //decryptionProcessor->AddImpulseToQueue(ImpulseError(eSystemEvent::ErrorReceivedBadPackage, );
            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorExecutionTask:
            LOG_ERROR("Got DecryptionProcessor_ThreadWorker error:" + ((ImpulseError *)i)->getError());
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

void Controller_DecryptionProcessor_ThreadWorker::CheckModule2Events(void *module1, void * module2)
{
    ThreadWorker *decryptionWorker = (ThreadWorker *)module1;
    DecryptionProcessor *eventGiver = (DecryptionProcessor *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::RequestSleepDecryptionWorker:
            if (((ImpulseSignal *)i)->getThreadID() != decryptionWorker->getThreadID())
            {
                break;
            }
            decryptionWorker->RequestSleep();

            deleteAndNext = true;
            break;
        case eSystemEvent::RequestStartDecryptionWorker:
            if (((ImpulseSignal *)i)->getThreadID() != decryptionWorker->getThreadID())
            {
                break;
            }
            decryptionWorker->RequestStart();

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

void Controller_DecryptionProcessor_ThreadWorker::setDecryptionProcessorObj(DecryptionProcessor * module)
{
    setModule1Obj(module);
}

void Controller_DecryptionProcessor_ThreadWorker::setThreadWorkerObj(ThreadWorker * module)
{
    setModule2Obj(module);
}
