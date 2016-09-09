#include "../../include/server/crypto_processor.h"

CryptoProcessor::CryptoProcessor(ThreadConfiguration &conf,
                                 int numberOfWorkers,
                                 eSystemEvent errorEvent,
                                 eSystemEvent requestStartWorkerEvent,
                                 eSystemEvent requestSleepWorkerEvent,
                                 eSystemEvent responseSleepWorkerEvent,
                                 eSystemEvent responseStartWorkerEvent):
    Processor(conf),
    errorEvent(errorEvent),
    requestStartWorkerEvent(requestStartWorkerEvent),
    requestSleepWorkerEvent(requestSleepWorkerEvent)
{
    ThreadConfiguration workersConf;
    workersConf.loopSleepTime = 10;
    workersConf.unsleepReactionTime = 1000;
    workersConf.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    workersConf.responseSleepEvent = responseSleepWorkerEvent;
    workersConf.responseStartEvent = responseStartWorkerEvent;

    CreateWorkers(workersConf, numberOfWorkers);
}

void CryptoProcessor::RequestStart()
{
    if (isRunning())
    {
        std::string error = "Server is already running.";
        AddImpulseToQueue(new ImpulseError(errorEvent, error));
        return;
    }

    sendEventToWorkers(requestStartWorkerEvent);

    unSleepThread();
}

void CryptoProcessor::RequestSleep()
{
    if (!isRunning())
    {
        std::string error = "Server is already stopped.";
        AddImpulseToQueue(new ImpulseError(errorEvent, error));
        return;
    }

    sendEventToWorkers(requestSleepWorkerEvent);

    sleepThread();
}

void EncryptionProcessor::EncryptPackage(PackageBuffer *m)
{
    CryptoContext *c = new CryptoContext();
    c->m = m;
    CryptoTask *task = new CryptoTask(c);

    AddTask((Task*)task);
}

void DecryptionProcessor::DecryptPackage(PackageBuffer *m)
{
    CryptoContext *c = new CryptoContext();
    c->m = m;
    CryptoTask *task = new CryptoTask(c);

    AddTask((Task*)task);
}

void CryptoProcessor::dowork()
{
    //TODO implement
}
