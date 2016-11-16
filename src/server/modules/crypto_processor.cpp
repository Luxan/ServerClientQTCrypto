#include "../../../include/server/modules/crypto_processor.h"

CryptoProcessor::CryptoProcessor(ThreadConfiguration &conf,
                                 InterfaceCommunicationController *controller,
                                 int numberOfWorkers,
                                 eSystemEvent errorEvent,
                                 eSystemEvent requestStartWorkerEvent,
                                 eSystemEvent requestSleepWorkerEvent):
    Processor(conf, controller, numberOfWorkers),
    errorEvent(errorEvent),
    requestStartWorkerEvent(requestStartWorkerEvent),
    requestSleepWorkerEvent(requestSleepWorkerEvent)
{}

void CryptoProcessor::RequestStart()
{
    if (isRunning())
    {
        std::string error = "CryptoProcessor is already running.";
        AddImpulseToQueue(new ImpulseError(errorEvent, error));
        return;
    }

    sendEventToWorkers(requestStartWorkerEvent);

    unSleepThread();
}

void CryptoProcessor::RequestStop()
{
    if (!isRunning())
    {
        std::string error = "CryptoProcessor is already stopped.";
        AddImpulseToQueue(new ImpulseError(errorEvent, error));
        return;
    }

    sendEventToWorkers(requestSleepWorkerEvent);

    sleepThread();
}

void EncryptionProcessor::EncryptPackageWithRSA(PackageWrapper *m, Key * rsaPublicKey)
{
    cipherBankLock.lock();
    std::map<SessionID, Cipher*>::iterator iter = cipherBank.find(m->sessionID);
    if (iter == cipherBank.end())
    {
        RSACipher * cipher = new RSACipher(rsaPublicKey, rsaPrivateKey);
        cipherBank[m->sessionID] = cipher;
    }
    Cipher * cipher = iter->second;
    cipherBankLock.unlock();

    CryptoContext *c = new CryptoContext(m, cipher, true);
    CryptoTask * task = new CryptoTask(c);

    AddTask((Task*)task);
}

void EncryptionProcessor::EncryptPackage(PackageWrapper *m)
{
    cipherBankLock.lock();
    std::map<SessionID, Cipher*>::iterator iter = cipherBank.find(m->sessionID);
    if (iter == cipherBank.end())
    {
        delete m;
        return;
    }
    Cipher * cipher = iter->second;
    cipherBankLock.unlock();
    CryptoContext *c = new CryptoContext(m, cipher, true);
    CryptoTask *task = new CryptoTask(c);

    AddTask((Task*)task);
}

void DecryptionProcessor::DecryptPackage(PackageWrapperEncoded *m)
{
    cipherBankLock.lock();
    std::map<SessionID, Cipher*>::iterator iter = cipherBank.find(m->sessionID);
    if (iter == cipherBank.end())
    {
        delete m;
        return;
    }
    Cipher * cipher = iter->second;
    cipherBankLock.unlock();
    CryptoContext *c = new CryptoContext(m, cipher, false);
    CryptoTask *task = new CryptoTask(c);

    AddTask((Task*)task);
}

void EncryptionProcessor::dowork()
{
    CryptoTask * task = (CryptoTask * )GetNextDoneTask();
    if (task == nullptr)
        return;

    CryptoContext * c = (CryptoContext *)task->getContext();

    AddImpulseToQueue(new ImpulsePackage(eSystemEvent::PackageToSend, c->extractWrapper()));
}

void DecryptionProcessor::dowork()
{
    CryptoTask * task = (CryptoTask * )GetNextDoneTask();
    if (task == nullptr)
        return;

    CryptoContext * c = (CryptoContext *)task->getContext();

    AddImpulseToQueue(new ImpulsePackage(eSystemEvent::PackageReceived, c->extractWrapper()));
}

DecryptionProcessor::DecryptionProcessor(ThreadConfiguration conf, Controller_DecryptionProcessor_ThreadWorker *controller, int numberOfWorkers):
    CryptoProcessor(conf,
                    controller,
                    numberOfWorkers,
                    eSystemEvent::ErrorDecryptionProcessor,
                    eSystemEvent::RequestStartDecryptionWorker,
                    eSystemEvent::RequestSleepDecryptionWorker)
{
    this->AddEventController(controller);

    ThreadConfiguration workersConf;
    workersConf.loopSleepTime = 10;
    workersConf.unsleepReactionTime = 1020;
    workersConf.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    workersConf.responseSleepEvent = eSystemEvent::ResponseStartDecryptionWorker;
    workersConf.responseStartEvent = eSystemEvent::ResponseSleepDecryptionWorker;

    controller->setDecryptionProcessorObj(this);

    for (int i = 0; i < numberOfWorkers; i++)
    {
        ThreadWorker *worker = CreateWorker(workersConf);
        controller->setThreadWorkerObj(worker);
        worker->AddEventController(controller);
    }
}

EncryptionProcessor::EncryptionProcessor(ThreadConfiguration conf, Controller_EncryptionProcessor_ThreadWorker *controller, int numberOfWorkers, Key * rsaPrivateKey):
    CryptoProcessor(conf,
                    controller,
                    numberOfWorkers,
                    eSystemEvent::ErrorEncryptionProcessor,
                    eSystemEvent::RequestStartEncryptionWorker,
                    eSystemEvent::RequestSleepEncryptionWorker),
    rsaPrivateKey(rsaPrivateKey)
{
    this->AddEventController(controller);

    ThreadConfiguration workersConf;
     workersConf.loopSleepTime = 10;
     workersConf.unsleepReactionTime = 1030;
     workersConf.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
     workersConf.responseSleepEvent = eSystemEvent::ResponseSleepEncryptionWorker;
     workersConf.responseStartEvent = eSystemEvent::ResponseStartEncryptionWorker;

     controller->setEncryptionProcessorObj(this);

     for (int i = 0; i < numberOfWorkers; i++)
     {
         ThreadWorker *worker = CreateWorker(workersConf);
         controller->setThreadWorkerObj(worker);
         worker->AddEventController(controller);
     }
}
