#include "../../include/server/interfaces/interface_communication_controller.h"
#include "../../include/server/controllers/controller_encryptionprocessor_threadworker.h"
#include "../../include/server/thread_worker.h"
#include "../../include/server/modules/crypto_processor.h"
#include "../../include/server/impulse.h"
#include "../../include/server/slog.h"

void Controller_EncryptionProcessor_ThreadWorker::CheckModule1Events(void *module1, void *module2)
{
    //EncryptionProcessor *ep = (EncryptionProcessor *)module1;
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
        case eSystemEvent::ResponseStartEncryptionWorker:
            ss << ((ImpulseSignal *)i)->getThreadID();
            SLog::logInfo() << "EncryptionWorker is started. id: " + ss.str();
            deleteAndNext = true;
            break;
        case eSystemEvent::ResponseSleepEncryptionWorker:
            ss << ((ImpulseSignal *)i)->getThreadID();
            SLog::logInfo() << "EncryptionWorker is sleeped. id: " + ss.str();
            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorEncryptionWorker:
            SLog::logError() << "EncryptionWorker Error: " + ((ImpulseError *)i)->getError();
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

void Controller_EncryptionProcessor_ThreadWorker::CheckModule2Events(void *module1, void * module2)
{
    ThreadWorker *encryptionWorker = (ThreadWorker *)module1;
    EncryptionProcessor *eventGiver = (EncryptionProcessor *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::RequestSleepEncryptionWorker:
            if (((ImpulseSignal *)i)->getThreadID() != encryptionWorker->getThreadID())
            {
                break;
            }
            encryptionWorker->RequestSleep();

            deleteAndNext = true;
            break;
        case eSystemEvent::RequestStartEncryptionWorker:
            if (((ImpulseSignal *)i)->getThreadID() != encryptionWorker->getThreadID())
            {
                break;
            }
            encryptionWorker->RequestStart();

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
