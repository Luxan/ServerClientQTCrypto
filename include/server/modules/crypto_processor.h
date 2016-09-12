#pragma once

#include "../interfaces/processor.h"
#include "../../shared/packages/package.h"
#include "../systemevents.h"


class CryptoProcessor : public Processor
{
protected:
    eSystemEvent errorEvent;
    eSystemEvent requestStartWorkerEvent;
    eSystemEvent requestSleepWorkerEvent;
    /**
    \see interface_thread.h
    */
    void dowork();
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    CryptoProcessor(ThreadConfiguration &conf,
                    int numberOfWorkers,
                    eSystemEvent errorEvent,
                    eSystemEvent requestStartWorkerEvent,
                    eSystemEvent requestSleepWorkerEvent,
                    eSystemEvent responseSleepWorkerEvent,
                    eSystemEvent responseStartWorkerEvent);


    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void RequestStart();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void RequestSleep();
};

class EncryptionProcessor : public CryptoProcessor
{
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    EncryptionProcessor(ThreadConfiguration conf, int numberOfWorkers):
        CryptoProcessor(conf,
                        numberOfWorkers,
                        eSystemEvent::ErrorEncryptionProcessor,
                        eSystemEvent::RequestStartEncryptionWorker,
                        eSystemEvent::RequestSleepEncryptionWorker,
                        eSystemEvent::ResponseStartEncryptionWorker,
                        eSystemEvent::ResponseSleepEncryptionWorker)
    {}    
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void EncryptPackage(PackageWrapper *m);
    /**
    \see interface_thread.h
    */
    void dowork();
};

class DecryptionProcessor : public CryptoProcessor
{
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    DecryptionProcessor(ThreadConfiguration conf, int numberOfWorkers):
        CryptoProcessor(conf,
                        numberOfWorkers,
                        eSystemEvent::ErrorDecryptionProcessor,
                        eSystemEvent::RequestStartDecryptionWorker,
                        eSystemEvent::RequestSleepDecryptionWorker,
                        eSystemEvent::ResponseStartDecryptionWorker,
                        eSystemEvent::ResponseSleepDecryptionWorker)
    {}

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void DecryptPackage(PackageWrapper *m);
    /**
    \see interface_thread.h
    */
    void dowork();
};
